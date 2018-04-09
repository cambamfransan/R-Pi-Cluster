#include "Tasks.hpp"
#include "Logger/Logger.hpp"
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <cstring>

#include <qfile.h>

manager::Task::Task(int job, int page, int task, std::string execute)
  : jobId(job), pageNumber(page), taskId(task), toExecute(execute)
{
}


bool manager::Task::operator==(const Task& t)
{
  return jobId == t.jobId && pageNumber == t.pageNumber && taskId == t.taskId;
}

manager::TaskFile::TaskFile(int page, int task, std::string name)
  : pageNumber(page), nextTaskId(task), pageName(name)
{

}

manager::TaskManager::TaskManager(int id,
                      int size,
  std::string database,
                      std::string tasksList)
  : m_myId(id),
    m_maxSize(size),
    m_valid(false),
    m_database(database + "/Tasks/"),
    m_taskFiles()
{
  if (tasksList == "")return;
  populateFields(tasksList);
}

manager::TaskManager::TaskManager()
{
}

void manager::TaskManager::populateFields(std::string tasksList)
{
  system(std::string("mkdir " + m_database).c_str());
  std::ifstream input(tasksList);
  std::string nextLine;
  if (!input)
  {
    std::cerr << "Error code: " << std::strerror(errno) << std::endl;
    return;
  }

  for(int i = 0; !input.eof(); i++)
  {
    std::string nextFile(m_database + std::to_string(i) + ".txt");
    m_taskFiles.push_back(TaskFile(i, 0, nextFile));
    Logger::info("Adding file: " + nextFile);
    std::ofstream output(nextFile);
    for (int j = 0; j < m_maxSize; j++)
    {
      if (!std::getline(input, nextLine)) break;
      if (nextLine != "") output << nextLine << "\n";
    }
    output.close();
  }
  m_valid = true;
}

manager::TaskManager::~TaskManager()
{
  // I should remove the files here
  for (size_t i = 0; i < m_taskFiles.size(); i++)
    remove(m_taskFiles[i].pageName.c_str());
}

std::vector<manager::Task> manager::TaskManager::getNextTasks(int howManyTasks)
{
  if (!m_valid) return std::vector<Task>();
  std::vector<int> toDelete;
  std::vector<Task> forReturn;
  size_t amount(static_cast<int>(howManyTasks));
  for (size_t i = 0; i < m_taskFiles.size() && forReturn.size() < amount;
       i++)
  {
    std::ifstream input(m_taskFiles[i].pageName);
    if (!input) continue;
    std::string str((std::istreambuf_iterator<char>(input)),
                    std::istreambuf_iterator<char>());
    input.close();

    while (str.size() != 0 && forReturn.size() < amount)
    {
      auto spot = str.find("\n");
      std::string inputString(str.substr(0, spot));
      str.erase(0, spot + 1);
      forReturn.emplace_back(m_myId, m_taskFiles[i].pageNumber, m_taskFiles[i].pageNumber*m_maxSize + m_taskFiles[i].nextTaskId++, inputString);
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

bool manager::TaskManager::removeFromResults(Task task)
{
  std::string nextFile(m_database + std::to_string(task.pageNumber) + ".txt");

  std::ifstream input(nextFile);
  if (!input){
    return false;
  } 
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

int manager::TaskManager::getSize()
{
  return m_maxSize;
}
