#include "Tasks.hpp"
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <qdir.h>

#include <qfile.h>

manager::Task::Task(int page, int task, std::string execute)
  : pageNumber(page), taskId(task), toExecute(execute)
{
}

manager::TaskFile::TaskFile(int page, int task, std::string name)
  : pageNumber(page), nextTaskId(task), pageName(name)
{

}

manager::Tasks::Tasks(int id,
                      int size,
                      std::string TasksList)
  : m_myId(id),
    m_maxSize(size),
    m_valid(false),
    m_taskFiles()
{
  std::ifstream input(TasksList);
  std::string nextLine;
  if (!input) return;

  system(std::string("mkdir " + std::to_string(m_myId)).c_str());
  system(std::string("cd " + std::to_string(1) + " && mkdir Tasks").c_str());

  for (int i = 0; !input.eof(); i++)
  {
    std::string nextFile(std::to_string(m_myId) + "/Tasks/" +
                         std::to_string(i) + ".txt");
    m_taskFiles.push_back(TaskFile(i, 0, nextFile));
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
    remove(m_taskFiles[i].pageName.c_str());
}

std::vector<manager::Task> manager::Tasks::getNextTasks(int howManyTasks)
{
  if (!m_valid) return std::vector<Task>();
  std::vector<int> toDelete;
  std::vector<Task> forReturn;
  for (size_t i = 0; i < m_taskFiles.size() && forReturn.size() < howManyTasks;
       i++)
  {
    std::ifstream input(m_taskFiles[i].pageName);
    if (!input) continue;
    std::string str((std::istreambuf_iterator<char>(input)),
                    std::istreambuf_iterator<char>());
    input.close();

    while (str.size() != 0 && forReturn.size() < howManyTasks)
    {
      auto spot = str.find("\n");
      std::string inputString(str.substr(0, spot));
      str.erase(0, spot + 1);
      forReturn.emplace_back(m_taskFiles[i].pageNumber, m_taskFiles[i].pageNumber*m_maxSize + m_taskFiles[i].nextTaskId++, inputString);
    }
    if (str.size() == 0)
    {
      if (remove(m_taskFiles[i].pageName.c_str()) != 0)
        perror("Error deleting file");
      toDelete.push_back(i);
    }
    else
    {
      std::ofstream output(m_taskFiles[i].pageName);
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
  std::string nextFile(std::to_string(m_myId) + "/Tasks/" +
                       std::to_string(task.pageNumber) + ".txt");

  std::ifstream input(nextFile);
  if (!input) return false;
  std::string str(
    (std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
  input.close();

  auto spot = str.find(task.toExecute);
  if (spot == std::string::npos) return false;
  str.erase(spot, task.toExecute.size()+1);

  std::ofstream output(nextFile);
  output << str;
  output.close();

  return true;
}
