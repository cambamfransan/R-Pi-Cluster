//#ifndef JOB_MANAGER_H
//#define JOB_MANAGER_H
//
//#include <string>
//#include "Tasks.hpp"
//#include "Results.hpp"
//
//namespace manager
//{
//  class JobManager
//  {
//  public:
//    JobManager(int id, int size, std::string taskList, std::string database = "");
//    ~JobManager();
//
//    void addResults(std::vector<manager::Result> results);
//    std::vector<manager::Task> getTasks(int amount);
//
//  private:
//    int m_myId;
//    manager::TaskManager m_taskManager;
//    manager::ResultsManager m_resultManager;
//    std::string m_database;
//
//  };
//
//} // namespace manager
//
//#endif