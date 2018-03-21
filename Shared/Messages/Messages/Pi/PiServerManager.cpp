//#include "Job.hpp"
//
//manager::JobManager::JobManager(int id,
//                                int size,
//                                std::string taskList,
//                                std::string database = "")
//  : m_myId(id),
//    m_taskManager(id, size, taskList),
//    m_resultManager(id),
//    m_database(database)
//{
//  system(std::string("mkdir " + m_database + std::to_string(m_myId)).c_str());
//}
//
//manager::JobManager::~JobManager()
//{
//  // can remove folder
//}
//
//void manager::JobManager::addResults(std::vector<manager::Result> results)
//{
//  m_resultManager.addResult(results);
//}
//
//std::vector<manager::Task> manager::JobManager::getTasks(int amount)
//{
//  m_taskManager.getNextTasks(amount);
//}
