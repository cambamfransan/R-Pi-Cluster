#include "Results.hpp"
#include <fstream>

manager::Results::Results(int id)
  : m_myId(id), m_resultFiles(), m_basePath(std::to_string(m_myId)+"/Results")
{
  system(std::string("mkdir " + std::to_string(m_myId)).c_str());
  system(std::string("cd " + std::to_string(m_myId) + "&& mkdir Results").c_str());
}

manager::Results::~Results()
{
  // delete files
}

void manager::Results::addResult(Result result)
{
  // Need to fix this
  std::string outFilePath(m_basePath + "/" + std::to_string(result.first.pageNumber));
  std::ofstream output(outFilePath, std::ios::app);
  output << result.first.toExecute << ":~:" << result.second << std::endl;
  output.close();
  m_resultFiles.insert(outFilePath);
}

std::string manager::Results::getResults()
{
  return "";
}