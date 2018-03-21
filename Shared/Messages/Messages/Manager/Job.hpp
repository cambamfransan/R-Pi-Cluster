#ifndef JOB_MANAGER_H
#define JOB_MANAGER_H

#include <string>
#include "Tasks.hpp"
#include "Results.hpp"

namespace manager
{
  class JobManager
  {
  public:
    JobManager(int id, int size, std::string gitUrl, std::string database = "", std::string script = ".//..//..//R-Pi-Cluster/Scripts/cloneUrl.sh");
    ~JobManager();

    void addResults(std::vector<manager::Result> results);
    std::vector<manager::Task> getTasks(int amount);

  private:
    int m_myId;
    std::string m_gitUrl;
    manager::TaskManager m_taskManager;
    manager::ResultsManager m_resultManager;
    std::string m_database;
    std::string m_name;
    std::string m_toExec;

  };

} // namespace manager

#endif