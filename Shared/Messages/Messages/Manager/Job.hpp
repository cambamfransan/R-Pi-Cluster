#ifndef JOB_H
#define JOB_H

#include <string>
#include "Tasks.hpp"
#include "Results.hpp"

namespace manager
{
  enum class Status
  {
    PLAY,
    PAUSE,
    STOP
  };


  class Job
  {
  public:
    Job(); // I don't like this
    Job(int id, int size, int pri, int taskpb, std::string gitUrl, std::string database = "", std::string script = ".//..//..//R-Pi-Cluster/Scripts/cloneUrl.sh");
    ~Job();

    void addResults(std::vector<manager::Result> results);
    std::vector<manager::Task> getTasks(int amount);
    std::string getName();
    std::string getExec();
    int getPriority();
    void setPriority(int priority);
    int getTasksPerBundle();
    void setTasksPerBundle(int bundle);
    int getJobId();
    Status getStatus();
    void setStatus(Status s);

  private:
    int m_myId;
    int m_priority;
    int m_taskPerBundle;
    std::string m_gitUrl;
    manager::TaskManager m_taskManager;
    manager::ResultsManager m_resultManager;
    std::string m_database;
    std::string m_name;
    std::string m_toExec;
    Status m_status;

  };
} // namespace manager

#endif
