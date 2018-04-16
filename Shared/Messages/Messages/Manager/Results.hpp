#ifndef RESULTS_CLASS_H
#define RESULTS_CLASS_H

#include "Tasks.hpp"
#include <set>
#include <string>

namespace manager
{
  typedef std::pair<manager::Task, std::string> Result;

  class ResultsManager
  {
  public:
    ResultsManager();
    ResultsManager(int id, std::string database);
    ~ResultsManager();

    void init(int id);

    void addResult(std::vector<Result> results);

    std::string getResults();

    int getTotal();

  private:

    int m_myId;
    int m_size;
    std::set<std::string> m_resultFiles;
    std::string m_basePath;
    int m_total;
  };
} // namespace manager
#endif
