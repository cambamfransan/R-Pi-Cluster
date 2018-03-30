#include "ExecuteManager.hpp"
#include "Execute.hpp"
#include <algorithm>

namespace
{
  // const std::string STATUS = "STATUS";
  // const std::string PRIORITY = "PRIORITY";
  // const std::string TASKS_PER_BUNDLE = "TASKS_PER_BUNDLE";
  // const std::string NAME = "NAME";
}

manager::ExecuteManager::ExecuteManager(std::string database)
  : m_jobs(),
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
            m_database + std::to_string(job.getJobId()) + "_bld",
            static_cast<int>(job.getStatus()),
            job.getPriority(),
            job.getName()};
}

void manager::ExecuteManager::addTasks(msg::TaskMsg* pTask)
{
  auto pTasks = pTask->task();
  for (const auto& pT : pTasks)
  {
    manager::Task task(pT.jobid(), pT.pagenumber(), pT.id(), pT.toexecute());
    auto id = m_jobs[pT.jobid()];
    manager::Execute* toEx = new manager::Execute(
      m_jobs[pT.jobid()], task, m_pResultsMutex, m_pResults);
    m_pThreadPool->start(toEx);
  }
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
