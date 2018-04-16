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

void manager::ExecuteManager::addJob(manager::Job job)
{
  m_jobs[job.getJobId()] =
    JobInfo{job.getJobId(),
            job.getTasksPerBundle(),
           m_database + std::to_string(job.getJobId()) + "/_bld/" + job.getExec(),
            job.getUrl(),
            static_cast<int>(job.getStatus()),
            job.getPriority(),
            job.getName()};
  Logger::info("Job added: " + std::to_string(job.getJobId()));
  if(m_waitingJobs.find(job.getJobId()) != m_waitingJobs.end())
  {
    for(const auto& task : m_waitingJobs[job.getJobId()])
    {
      addTasksToQueue(task);
    }
  }
}

void manager::ExecuteManager::addTasks(std::vector<manager::Task> tasks)
{
  Logger::info("Adding Tasks");
  for (const auto& t : tasks)
  {
    if(m_jobs.find(t.jobId) != m_jobs.end()){
      addTasksToQueue(t);
      Logger::info("Added task to queue");
    }
    else 
    {
      Logger::info("Added task to waiting queue");
      m_waitingJobs[t.jobId].push_back(t);
    }
  }
}

void manager::ExecuteManager::addTasksToQueue(manager::Task task)
{
  Logger::info("Adding Task to Queue: " + task.toExecute);
  manager::Execute* toEx = new manager::Execute(
    m_jobs[task.jobId], task, m_pResultsMutex, m_pResults);
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
    m_jobs[id].status = std::stoi(value);
  }
  else if (field == manager::PRIORITY)
  {
    m_jobs[id].priority = std::stoi(value);
  }
  if (field == manager::NAME)
  {
    m_jobs[id].name = value;
  }
  if (field == manager::TASKS_PER_BUNDLE)
  {
    m_jobs[id].tasksPerBundle = std::stoi(value);
  }
}

void manager::ExecuteManager::endTask(int jobId)
{
  std::cout << "End task reached" << jobId << std::endl;
  //todo: What if job ends
  if(m_jobs[jobId].tasksPerBundle < (*m_pResults)[jobId].size())
  {
    std::cout <<"Sending Results" << std::endl;
    int tpb(m_jobs[jobId].tasksPerBundle);
    std::vector<manager::Result> results;
    std::lock_guard<std::mutex> lock(*m_pResultsMutex);
    for(int i = 0; i < tpb; i++)
    {
      results.push_back(m_pResults->operator[](jobId)[0]);
      (*m_pResults)[jobId].erase((*m_pResults)[jobId].begin());
      std::cout << results[i].second << std::endl;
    }
    emit sendResults(results);
  }
}

