#ifndef CLIENT_RESULTS_MANAGER_H
#define CLIENT_RESULTS_MANAGER_H

#include "Results.hpp"
#include <map>

namespace manager
{

  class ClientResultsManager
  {
  public:
    ClientResultsManager(std::string database);
    ~ClientResultsManager();

    void addResults(int id, std::vector<Result> results);

  private:
    std::string m_database;
    std::map<int, ResultsManager> m_results;
  };
} // namespace manager
#endif