#include "JobManager.hpp"
#include <algorithm>

manager::JobManager::JobManager(std::string database)
  : m_jobs(),
    m_nextJobId(0),
    m_database(database),
    m_curJob(0),
    m_curJobDone(0),
    m_jobsMutex(),
    m_updateMutex(),
    m_newJobs(),
    m_lostJobs(),
    m_newResults(),
    m_modifiedJobs()
{
}

manager::JobManager::~JobManager()
{
  // Can remove files
}

int manager::JobManager::addJob(int size,
                                int pri,
                                int taskpb,
                                std::string gitUrl)
{
  int jobId = m_nextJobId++;
  Job newJob(jobId, size, pri, taskpb, gitUrl, m_database, m_cloneScript);
  {
    std::lock_guard<std::mutex> lock(m_jobsMutex);
    m_jobs[newJob.getJobId()] = newJob;
    if (m_jobs.size() == 1) m_curJob = jobId;
  }
  {
    std::lock_guard<std::mutex> lock(m_updateMutex);
    m_newJobs.push_back(newJob);
  }

  return jobId;
}

void manager::JobManager::addJobResults(int id,
                                        std::vector<manager::Result> results)
{
  {
    std::lock_guard<std::mutex> lock(m_jobsMutex);
    m_jobs[id].addResults(results);
  }
  {
    std::lock_guard<std::mutex> lock(m_updateMutex);
    m_newResults.push_back(results);
  }
}

std::vector<manager::Task> manager::JobManager::getTasks(int amount)
{
  std::lock_guard<std::mutex> lock(m_jobsMutex);
  std::vector<manager::Task> toReturn;
  size_t am(static_cast<size_t>(amount));
  while (toReturn.size() < amount)
  {
    if (m_jobs.empty()) return toReturn;
    int priority(m_jobs[m_curJob].getPriority());
    if (m_curJobDone > priority)
    {
      if (m_jobs.empty()) return toReturn;
      // find next job
      m_curJobDone = 0;
      auto itrCurJob = ++m_jobs.find(m_curJob);
      if (itrCurJob == m_jobs.end())
      {
        m_curJob = m_jobs.begin()->first;
      }
      else
      {
        m_curJob = itrCurJob->first;
      }
    }

    auto nextBatch =
      m_jobs[m_curJob].getTasks(std::min(amount, priority - m_curJobDone));
    m_curJobDone += nextBatch.size();
  }

  return toReturn;
}

void manager::JobManager::removeJob(int id)
{
  {
    std::lock_guard<std::mutex> lock(m_jobsMutex);
    m_jobs.erase(id);
  }
  {
    std::lock_guard<std::mutex> lock(m_updateMutex);
    m_lostJobs.push_back(id);
  }
}

void manager::JobManager::pauseJob(int id)
{
  {
    std::lock_guard<std::mutex> lock(m_jobsMutex);
    m_jobs[id].setStatus(Status::PAUSE);
  }
  {
    std::lock_guard<std::mutex> lock(m_updateMutex);
    m_modifiedJobs.push_back(manager::ModifiedJob{ id, STATUS, std::string("PAUSE") });
  }
}

void manager::JobManager::playJob(int id)
{
  {
    std::lock_guard<std::mutex> lock(m_jobsMutex);
    m_jobs[id].setStatus(Status::PLAY);
  }
  {
    std::lock_guard<std::mutex> lock(m_updateMutex);
    m_modifiedJobs.push_back(manager::ModifiedJob{ id, STATUS, std::string("PLAY") });
  }
}

void manager::JobManager::stopJob(int id)
{
  {
    std::lock_guard<std::mutex> lock(m_jobsMutex);
    m_jobs[id].setStatus(Status::STOP);
  }
  {
    std::lock_guard<std::mutex> lock(m_updateMutex);
    m_modifiedJobs.push_back(manager::ModifiedJob{ id, STATUS, "STOP" });
  }
}

void manager::JobManager::setName(int id, std::string name)
{
  {
    std::lock_guard<std::mutex> lock(m_jobsMutex);
    m_jobs[id].setName(name);
  }
  {
    std::lock_guard<std::mutex> lock(m_updateMutex);
    m_modifiedJobs.push_back(manager::ModifiedJob{ id, NAME, name });
  }
}

std::string manager::JobManager::getJobName(int id)
{
  return m_jobs[id].getName();
}

std::string manager::JobManager::getJobExec(int id)
{
  return m_jobs[id].getExec();
}

void manager::JobManager::modifyTasksPerBundle(int id, int taskPerBundle)
{
  {
    std::lock_guard<std::mutex> lock(m_jobsMutex);
    m_jobs[id].setTasksPerBundle(taskPerBundle);
  }
  {
    std::lock_guard<std::mutex> lock(m_updateMutex);
    m_modifiedJobs.push_back(manager::ModifiedJob{
      id, TASKS_PER_BUNDLE, std::to_string(taskPerBundle) });
  }
}

void manager::JobManager::modifyPriority(int id, int priority)
{
  {
    std::lock_guard<std::mutex> lock(m_jobsMutex);
    m_jobs[id].setPriority(priority);
  }
  {
    std::lock_guard<std::mutex> lock(m_updateMutex);
    m_modifiedJobs.push_back(manager::ModifiedJob{ id, TASKS_PER_BUNDLE, std::to_string(priority) });
  }
}

manager::JobUpdateStruct manager::JobManager::getUpdates()
{
  std::lock_guard<std::mutex> lock(m_updateMutex);
  manager::JobUpdateStruct job{m_newJobs, m_lostJobs, m_newResults, m_modifiedJobs};
  m_newJobs.clear();
  m_lostJobs.clear();
  m_newResults.clear();
  m_modifiedJobs.clear();

  return job;
}

std::string manager::JobManager::m_cloneScript =
  ".//..//..//R-Pi-Cluster/Scripts/cloneUrl.sh";
