#include "Results.hpp"
#include <fstream>
#include <iostream>
#include "Logger/Logger.hpp"

manager::ResultsManager::ResultsManager()
  : m_myId(-1), m_resultFiles(), m_basePath()
{
}

manager::ResultsManager::ResultsManager(int id, std::string database)
  : m_myId(id), m_resultFiles(), m_basePath(database + "/Results"), m_total(0)
{
  init(id);
}

manager::ResultsManager::~ResultsManager()
{
  // delete files
  for (const auto& file : m_resultFiles)
    remove(file.c_str());
}

void manager::ResultsManager::init(int id)
{
  m_myId = id;
  if(m_basePath.empty())
    m_basePath = std::to_string(m_myId) + "//Results";
  system(std::string("mkdir " + m_basePath).c_str());
}

void manager::ResultsManager::addResult(std::vector<Result> results)
{
  // this could be more effecient
  for (auto&& result : results)
  {
    // Need to fix this
    std::string outFilePath(m_basePath + "/" +
      std::to_string(result.first.pageNumber) + ".txt");
    std::ofstream output(outFilePath, std::ios::app);
    output << result.first.taskId << ":~:" << result.first.toExecute
      << ":~:" << result.second << std::endl;
    output.close();
    m_resultFiles.insert(outFilePath);
    m_total++;
  }
  Logger::info("total: " + std::to_string(m_total));
}

std::string manager::ResultsManager::getResults()
{
  std::string results;

  for (const auto& file : m_resultFiles)
  {
    std::ifstream input(file);
    results.append((std::istreambuf_iterator<char>(input)),
                   std::istreambuf_iterator<char>());
  }

  return results;
}

int manager::ResultsManager::getTotal()
{
  return m_total;
}

