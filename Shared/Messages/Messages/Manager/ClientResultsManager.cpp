#include "ClientResultsManager.hpp"

manager::ClientResultsManager::ClientResultsManager(std::string database)
  : m_database(database), m_results()
{
}

manager::ClientResultsManager::~ClientResultsManager()
{
  // Delete results
}

void manager::ClientResultsManager::addResults(int id,
                                               std::vector<Result> results)
{
  if (m_results.find(id) == m_results.end()) m_results[id].init(id);
  m_results[id].addResult(results);
}
