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
      if (m_jobs.empty()) return toReturn;
      m_curJobDone = 0;
      auto itrCurJob = ++m_jobs.find(m_curJob);
      if (itrCurJob == m_jobs.end())
      {
        m_curJob = m_jobs.begin()->first;
        Logger::info("m_curJob: " + std::to_string(m_curJob));
      }
      else
      {
        m_curJob = itrCurJob->first;
        Logger::info("m_curJob1: " + std::to_string(m_curJob));
      }
    }

//    auto nextBatch =
//      m_jobs[m_curJob]->getTasks(std::min(amount, priority - m_curJobDone));
    auto nextBatch =
      m_jobs[m_curJob]->getTasks(m_jobs[m_curJob]->getTasksPerBundle());
    toReturn.insert(toReturn.end(), nextBatch.begin(), nextBatch.end());
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
    m_jobs[id]->setStatus(Status::PAUSE);
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
    m_jobs[id]->setStatus(Status::PLAY);
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
    m_jobs[id]->setStatus(Status::STOP);
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

std::map<int, int> manager::JobManager::getProgress()
{
  std::map<int, int> toReturn;
  for(const auto& job : m_jobs)
  {
    toReturn[job.first] = job.second->getProgress();
  }
  return toReturn;
}

