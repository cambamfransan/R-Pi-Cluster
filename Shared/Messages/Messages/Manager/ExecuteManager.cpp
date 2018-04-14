#include "ExecuteManager.hpp"
#include "Execute.hpp"
#include <algorithm>
#include "Logger/Logger.hpp"
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
    m_pResultsMutex(),
    m_pResults(),
    m_pThreadPool(new QThreadPool())
{
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
            m_database + std::to_string(job.getJobId()) + "/_bld",
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
