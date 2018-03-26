#include "ExecuteManager.hpp"
#include "Execute.hpp"
#include <algorithm>

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
            m_database + std::to_string(job.getJobId()) + "_bld"};
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
