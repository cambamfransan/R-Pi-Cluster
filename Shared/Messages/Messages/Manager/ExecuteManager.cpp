#include "ExecuteManager.hpp"
#include "Execute.hpp"
#include <algorithm>
#include "Logger/Logger.hpp"
#include <mutex>
#include "Messages/MakeMsgs.hpp"
#include <iostream>

namespace
{
  // const std::string STATUS = "STATUS";
  // const std::string PRIORITY = "PRIORITY";
  // const std::string TASKS_PER_BUNDLE = "TASKS_PER_BUNDLE";
  // const std::string NAME = "NAME";
}

manager::ExecuteManager::ExecuteManager(std::string database)
  : m_jobs(),
    m_waitingJobs(),
    m_database(database),
    m_size(0),
    m_pResultsMutex(std::make_shared<std::mutex>()),
    m_pResults(std::make_shared<std::map<int, std::vector<manager::Result>>>()),
    m_pThreadPool(new QThreadPool())
{
  qRegisterMetaType<std::vector<manager::Result>>("std::vector<manager::Result>>");
}

manager::ExecuteManager::~ExecuteManager()
{
  // Can remove files
}

void manager::ExecuteManager::addJob(int id, int size, int pri, int tpb, std::string url, std::string database)
{
  auto pJob = std::make_shared<Job>(id, size, pri, tpb, url, database);
  m_jobs[id] = pJob;
  Logger::info("Job added: " + std::to_string(id));
  if(m_waitingJobs.find(id) != m_waitingJobs.end())
  {
    if(m_waitingJobs[id].size() % tpb != 0)
    {
      m_jobs[id]->setStatus(Status::COMPLETED);
    }
    for(const auto& task : m_waitingJobs[id])
    {
      addTasksToQueue(task);
    }
  }
}

void manager::ExecuteManager::addTasks(std::vector<manager::Task> tasks)
{
  Logger::info("Adding Tasks");
  std::map<int, std::vector<manager::Task>> tasksReceived;
  for (const auto& t : tasks)
  {
    if(m_jobs.find(t.jobId) != m_jobs.end())
    {
      tasksReceived[t.jobId].push_back(t);
      addTasksToQueue(t);
      Logger::info("Added task to queue");
    }
    else 
    {
      Logger::info("Added task to waiting queue");
      m_waitingJobs[t.jobId].push_back(t);
    }
  }
  for(const auto& job : tasksReceived)
  {
    if(job.second.size() % m_jobs[job.first]->getTasksPerBundle() != 0)
    {
      Logger::info("setting job as done" + std::to_string(job.second.size()) + 
           " " + std::to_string(m_jobs[job.first]->getTasksPerBundle()));
      m_jobs[job.first]->setStatus(Status::COMPLETED);
    }
  }
}

void manager::ExecuteManager::addTasksToQueue(manager::Task task)
{
  Logger::info("Adding Task to Queue: " + task.toExecute);
  manager::Execute* toEx = new manager::Execute(
      task.jobId, m_jobs[task.jobId]->getBldLoc(),
    task, m_pResultsMutex, m_pResults);
  connect(toEx, &manager::Execute::endTask, this, &ExecuteManager::endTask);
  m_pThreadPool->start(toEx);
}

void manager::ExecuteManager::removeJob(int id)
{
  m_jobs.erase(id);
}

void manager::ExecuteManager::modifyJob(int id,
                                        std::string field,
                                        std::string value)
{
  if (field == manager::STATUS)
  {
    m_jobs[id]->setStatus(static_cast<Status>(std::stoi(value)));
  }
  else if (field == manager::PRIORITY)
  {
    m_jobs[id]->setPriority(std::stoi(value));
  }
  if (field == manager::NAME)
  {
    m_jobs[id]->setName(value);
  }
  if (field == manager::TASKS_PER_BUNDLE)
  {
    m_jobs[id]->setTasksPerBundle(std::stoi(value));
  }
}

void manager::ExecuteManager::endTask(int jobId)
{
  //todo: What if job ends
  if(m_jobs[jobId]->getTasksPerBundle() <= (*m_pResults)[jobId].size() || m_jobs[jobId]->getStatus() == Status::COMPLETED)
  {
    int tpb;
    if(m_jobs[jobId]->getStatus() == Status::COMPLETED) tpb = 1;
    else tpb = m_jobs[jobId]->getTasksPerBundle();
    std::vector<manager::Result> results;
    std::lock_guard<std::mutex> lock(*m_pResultsMutex);
    for(int i = 0; i < tpb; i++)
    {
      results.push_back(m_pResults->operator[](jobId)[0]);
      (*m_pResults)[jobId].erase((*m_pResults)[jobId].begin());
    }
    emit sendResults(results);
  }
}

void manager::ExecuteManager::addResults(int id, std::vector<Result> results)
{
  m_jobs[id]->addResults(results);
  for(const auto& res : results)
  {
    m_jobs[id]->removeTask(res.first);
  }
  Logger::info("Removed Tasks");
}

