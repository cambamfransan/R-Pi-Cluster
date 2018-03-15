#include "Tasks.hpp"
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <thread>

#include <qfile.h>

manager::Tasks::Tasks(int id,
                      int size,
                      std::string TasksList,
                      std::string destDir)
  : m_myId(id),
    m_maxSize(size),
    m_valid(false),
    m_destDir(destDir),
    m_taskFiles()
{
  //  char cCurrentPath[FILENAME_MAX];
  //  std::cout << _getcwd(cCurrentPath, sizeof(cCurrentPath)) << TasksList
  //            << std::endl;
  std::ifstream input(TasksList);
  std::string nextLine;
  if (!input) return;
  for (int i = 0; !input.eof(); i++)
  {
    std::string nextFile(m_destDir + "/" + std::to_string(m_myId) + "/" +
                         std::to_string(i) + ".txt");
    m_taskFiles.emplace_back(i, nextFile);
    std::ofstream output(nextFile);
    if (output)
    {
      for (int j = 0; j < size; j++)
      {
        if (!std::getline(input, nextLine)) break;
        if (nextLine != "") output << nextLine << "\n";
      }
    }
  }
  m_valid = true;
}

manager::Tasks::~Tasks()
{
  // I should remove the files here
  for (size_t i = 0; i < m_taskFiles.size(); i++)
    remove(m_taskFiles[i].second.c_str());
}

std::vector<Task> manager::Tasks::getNextTasks(int howManyTasks)
{
  if (!m_valid) return std::vector<Task>();
  std::vector<int> toDelete;
  std::vector<Task> forReturn;
  for (size_t i = 0; i < m_taskFiles.size() && forReturn.size() < howManyTasks;
       i++)
  {
    std::ifstream input(m_taskFiles[i].second);
    if (!input) continue;
    std::string str((std::istreambuf_iterator<char>(input)),
                    std::istreambuf_iterator<char>());
    input.close();

    while (str.size() != 0 && forReturn.size() < howManyTasks)
    {
      auto spot = str.find("\n");
      std::string inputString(str.substr(0, spot));
      str.erase(0, spot + 1);
      forReturn.emplace_back(m_taskFiles[i].first, inputString);
    }
    if (str.size() == 0)
    {
      if (remove(m_taskFiles[i].second.c_str()) != 0)
        perror("Error deleting file");
      toDelete.push_back(i);
    }
    else
    {
      std::ofstream output(m_taskFiles[i].second);
      output << str;
    }
  }
  for (size_t i = toDelete.size(); i > 0; i--)
  {
    m_taskFiles.erase(m_taskFiles.begin() + i - 1);
  }
  return forReturn;
}

bool manager::Tasks::removeFromResults(Task task)
{
  std::string nextFile(m_destDir + "/" + std::to_string(m_myId) + "/" +
                       std::to_string(task.first) + ".txt");

  std::ifstream input(nextFile);
  if (!input) return false;
  std::string str(
    (std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
  input.close();

  auto spot = str.find(task.second);
  if (spot == std::string::npos) return false;
  str.erase(spot, task.second.size()+1);

  std::ofstream output(nextFile);
  output << str;
  output.close();

  return true;
}
