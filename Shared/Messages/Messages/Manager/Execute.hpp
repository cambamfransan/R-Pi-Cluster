#ifndef EXECUTE_H
#define EXECUTE_H

#include <qrunnable.h>
#include <string>
#include "Job.hpp"
#include "Tasks.hpp"
#include <mutex>
#include <qrunnable.h>
#include <memory>
#include <mutex>
#include <QObject>

namespace manager
{
  class Execute : public QObject, public QRunnable
  {
  Q_OBJECT

  signals:
    void endTask(int jobId);

  public:
    Execute(JobInfo job, Task toExec, std::shared_ptr<std::mutex> pMutex, std::shared_ptr<std::map<int, std::vector<Result>>> pResults);
    ~Execute();

    void run();

  private:
    manager::JobInfo m_jobInfo;
    Task m_task;
    std::shared_ptr<std::mutex> m_pMutex;
    std::shared_ptr<std::map<int, std::vector<Result>>> m_pResults;

  };

} // namespace manager

#endif
