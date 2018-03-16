#ifndef RESULTS_CLASS_H
#define RESULTS_CLASS_H

#include "Tasks.hpp"
#include <string>
#include <set>

typedef std::pair<manager::Task, std::string> Result;

namespace manager
{
  class Results
  {
  public:
    Results(int id);
    ~Results();

    void addResult(Result result);
    
    std::string getResults();

  private:
    int m_myId;
    int m_size;
    std::set<std::string> m_resultFiles;
    std::string m_basePath;

  };
} // namespace manager
#endif