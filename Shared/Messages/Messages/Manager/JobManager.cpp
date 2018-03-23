#include "JobManager.hpp"
#include <algorithm>

manager::JobManager::JobManager(std::string database)
  : m_jobs(), m_nextJobId(0), m_database(database), m_curJob(0), m_curJobDone(0)
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
  m_jobs[newJob.getJobId()] = newJob;
  if (m_jobs.size() == 1) m_curJob = jobId;
  return jobId;
}

void manager::JobManager::addJobResults(int id,
                                        std::vector<manager::Result> results)
{
  m_jobs[id].addResults(results);
}

std::vector<manager::Task> manager::JobManager::getTasks(int amount)
{
  std::vector<manager::Task> toReturn;
  while (toReturn.size() < amount)
  {
    int priority(m_jobs[m_curJob].getPriority());
    if (m_curJobDone > priority)
    {
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
  m_jobs.erase(id);
}
void manager::JobManager::pauseJob(int id)
{
  m_jobs[id].setStatus(Status::PAUSE);
}
void manager::JobManager::playJob(int id)
{
  m_jobs[id].setStatus(Status::PAUSE);
}
void manager::JobManager::stopJob(int id)
{
  m_jobs[id].setStatus(Status::PAUSE);
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
  m_jobs[id].setTasksPerBundle(taskPerBundle);
}
void manager::JobManager::modifyPriority(int id, int priority)
{
  m_jobs[id].setPriority(priority);
}

std::string manager::JobManager::m_cloneScript =
  ".//..//..//R-Pi-Cluster/Scripts/cloneUrl.sh";
