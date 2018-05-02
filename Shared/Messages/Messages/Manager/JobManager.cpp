#include "JobManager.hpp"
#include "Logger/Logger.hpp"
#include <algorithm>
#include <iostream>

manager::JobManager::JobManager(std::string database, std::string cloneScript)
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
    m_modifiedJobs(),
    m_cloneScript(cloneScript)
{
  m_database += +"Jobs";
  system(std::string("mkdir " + m_database).c_str());
}

manager::JobManager::~JobManager()
{
  // Can remove files
}

int manager::JobManager::getNextId()
{
  return m_nextJobId++;
}

int manager::JobManager::addJob(int size,
                                int pri,
                                int taskpb,
                                std::string gitUrl,
                                int jobId)
{
  if(jobId == -1) jobId = m_nextJobId++;
  
    auto pNewJob = std::make_shared<Job>(jobId, size, pri, taskpb, gitUrl, m_database, m_cloneScript);
  {
    Logger::info("Added new Job");
    std::lock_guard<std::mutex> lock(m_jobsMutex);
    m_jobs[pNewJob->getJobId()] = pNewJob;
    if (m_jobs.size() == 1) m_curJob = jobId;
  }
  {
    std::lock_guard<std::mutex> lock(m_updateMutex);
    manager::JobInfo info{pNewJob->getJobId(), pNewJob->getTasksPerBundle(),
      pNewJob->getExec(),
      pNewJob->getUrl(),
     static_cast<int>(pNewJob->getStatus()), pNewJob->getPriority(), pNewJob->getName()};
    m_newJobs.push_back(info);
  }
  
  return jobId;
}

void manager::JobManager::addJobResults(int id,
                                        std::vector<manager::Result> results)
{
  {
    std::lock_guard<std::mutex> lock(m_jobsMutex);
    m_jobs[id]->addResults(results);
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
  while (toReturn.size() < am)
  {
    if (m_jobs.empty()) return toReturn;
    int priority(m_jobs[m_curJob]->getPriority());
    if (m_curJobDone >= priority)
    {
      Logger::info("getting new job");
      // find next job
      if (!moreJobsToProcess()) return toReturn;
      m_curJobDone = 0;
      //this can be improved
      std::map<int, std::shared_ptr<Job>>::iterator itrCurJob;
      do 
      {
        itrCurJob = ++m_jobs.find(m_curJob);
        if (itrCurJob == m_jobs.end())
        {
          m_curJob = m_jobs.begin()->first;
          itrCurJob = m_jobs.begin();
          Logger::info("m_curJob: " + std::to_string(m_curJob));
        }
        m_curJob = itrCurJob->first;
      } while(m_jobs[m_curJob]->getStatus() != manager::Status::PLAY);
      Logger::info("m_curJob1: " + std::to_string(m_curJob));
    }

    int tpb = m_jobs[m_curJob]->getTasksPerBundle();
    auto nextBatch =
      m_jobs[m_curJob]->getTasks(tpb);
    if(nextBatch.size() != tpb)
    {
      m_curJobDone = priority;
      std::lock_guard<std::mutex> lock(m_updateMutex);
      m_modifiedJobs.push_back(manager::ModifiedJob{m_curJob, STATUS, "4"});
    }
    toReturn.insert(toReturn.end(), nextBatch.begin(), nextBatch.end());
    m_curJobDone += nextBatch.size();
  }

  return toReturn;
}

bool manager::JobManager::moreJobsToProcess()
{
  for(const auto& job : m_jobs)
  {
    if(!job.second->isDone())
      return true;
  }
  return false;
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
    m_jobs[id]->setStatus(Status::PAUSE);
  }
  {
    std::lock_guard<std::mutex> lock(m_updateMutex);
    m_modifiedJobs.push_back(manager::ModifiedJob{ id, STATUS, "2"});
  }
}

void manager::JobManager::playJob(int id)
{
  {
    std::lock_guard<std::mutex> lock(m_jobsMutex);
    m_jobs[id]->setStatus(Status::PLAY);
  }
  {
    std::lock_guard<std::mutex> lock(m_updateMutex);
    m_modifiedJobs.push_back(manager::ModifiedJob{ id, STATUS, "1"});
  }
}

void manager::JobManager::stopJob(int id)
{
  {
    std::lock_guard<std::mutex> lock(m_jobsMutex);
    m_jobs[id]->setStatus(Status::STOP);
  }
  {
    std::lock_guard<std::mutex> lock(m_updateMutex);
    m_modifiedJobs.push_back(manager::ModifiedJob{ id, STATUS, "3" });
  }
}

void manager::JobManager::setName(int id, std::string name)
{
  {
    std::lock_guard<std::mutex> lock(m_jobsMutex);
    m_jobs[id]->setName(name);
  }
  {
    std::lock_guard<std::mutex> lock(m_updateMutex);
    m_modifiedJobs.push_back(manager::ModifiedJob{ id, NAME, name });
  }
}

std::string manager::JobManager::getJobName(int id)
{
  return m_jobs[id]->getName();
}

std::string manager::JobManager::getJobExec(int id)
{
  return m_jobs[id]->getExec();
}

void manager::JobManager::modifyTasksPerBundle(int id, int taskPerBundle)
{
  {
    std::lock_guard<std::mutex> lock(m_jobsMutex);
    m_jobs[id]->setTasksPerBundle(taskPerBundle);
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
    m_jobs[id]->setPriority(priority);
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

std::vector<manager::JobInfo> manager::JobManager::getCurrentJobs()
{
  std::vector<manager::JobInfo> forReturn;
  for(const auto& job : m_jobs)
  {
    forReturn.push_back({job.first, job.second->getTasksPerBundle(),
        job.second->getExec(),
        job.second->getUrl(),
        static_cast<int>(job.second->getStatus()),
          job.second->getPriority(),
          job.second->getName()});

  }
  return forReturn;
}

std::map<int, int> manager::JobManager::getProgress()
{
  std::map<int, int> toReturn;
  for(const auto& job : m_jobs)
  {
    toReturn[job.first] = job.second->getProgress();
  }
  return toReturn;
}

std::vector<int> manager::JobManager::getJobIds()
{
  std::vector<int> toReturn;
  for(const auto& job : m_jobs)
  {
    toReturn.push_back(job.first);
  }
  return toReturn;
}

std::vector<std::pair<int, std::string>> manager::JobManager::getResults(int id)
{
  return m_jobs[id]->getResults();
}

void manager::JobManager::addTasks(std::vector<Task> tasks)
{
  for(const auto& task : tasks)
  {
    m_jobs[task.jobId]->addTempTask(task);
  }
}

