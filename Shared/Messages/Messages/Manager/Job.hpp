#ifndef JOB_H
#define JOB_H

#include <string>
#include "Tasks.hpp"
#include "Results.hpp"

namespace manager
{
  const std::string STATUS = "STATUS";
  const std::string PRIORITY = "PRIORITY";
  const std::string TASKS_PER_BUNDLE = "TASKS_PER_BUNDLE";
  const std::string NAME = "NAME";

  struct ModifiedJob
  {
    int id;
    std::string field;
    std::string value;
  };

  enum class Status
  {
    PLAY = 1,
    PAUSE = 2,
    STOP = 3
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
    void setName(std::string name);
    std::string getUrl();

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
