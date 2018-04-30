#include "Job.hpp"
#include <fstream>
#include <iostream>
#include "Logger/Logger.hpp"

manager::Job::Job()
  : m_myId(-1),
  m_priority(-1),
  m_taskPerBundle(-1),
  m_gitUrl(""),
  m_taskManager(),
  m_resultManager(),
  m_database(""),
  m_name(),
  m_toExec(),
  m_status(manager::Status::PLAY)
{}


manager::Job::Job(int id,
                  int size,
                  int pri,
                  int taskpb,
                  std::string gitUrl,
                  std::string database,
                  std::string cloneScript)
  : m_myId(id),
    m_priority(pri),
    m_taskPerBundle(taskpb),
    m_gitUrl(gitUrl),
    m_database(database + "/" + std::to_string(m_myId)),
    m_taskManager(id, size, m_database),
    m_name(),
    m_toExec(),
    m_status(manager::Status::PLAY)
{
  m_name = gitUrl;
  size_t spot;
  while ((spot = m_name.find("/")) != std::string::npos)
  {
    m_name.erase(0, spot + 1);
  }
  m_name.erase(m_name.size() - 4);
  system(std::string("dir").c_str());
  system(std::string(cloneScript + " " + m_database + " " + m_gitUrl + " " + m_name).c_str());
  m_taskManager.populateFields(m_database + "/" + m_name + "/TasksList.txt");
  std::ifstream fin(m_database + "/" + m_name + "/Exec.txt");
  std::getline(fin, m_toExec);
  m_resultManager = manager::ResultsManager(id, m_database);
}

manager::Job::~Job()
{
  // can remove folder
}

void manager::Job::addResults(std::vector<manager::Result> results)
{
  m_resultManager.addResult(results);
}

std::vector<manager::Task> manager::Job::getTasks(int amount)
{
  auto toReturn = m_taskManager.getNextTasks(amount);
  if(toReturn.size() != amount)
    m_status = manager::Status::COMPLETED;
  return toReturn;
}

int manager::Job::getJobId()
{
  return m_myId;
}

manager::Status manager::Job::getStatus()
{
  return m_status;
}
void manager::Job::setStatus(manager::Status s)
{
  m_status = s;
}

std::string manager::Job::getName()
{
  return m_name;
}

std::string manager::Job::getExec()
{
  return m_toExec;
}

int manager::Job::getPriority()
{
  return m_priority;
}

void manager::Job::setPriority(int priority)
{
  m_priority = priority;
}


int manager::Job::getTasksPerBundle()
{
  return m_taskPerBundle;
}

void manager::Job::setTasksPerBundle(int bundle)
{
  m_taskPerBundle = bundle;
}

void manager::Job::setName(std::string name)
{
  m_name = name;
}

std::string manager::Job::getUrl()
{
  return m_gitUrl;
}

int manager::Job::getSize()
{
  return m_taskManager.getSize();
}

int manager::Job::getProgress()
{
  Logger::info("Progress: " + std::to_string(m_resultManager.getTotal()));
  return (100 * m_resultManager.getTotal()) / m_taskManager.getTotal();
}

bool manager::Job::isDone()
{
  return m_status==manager::Status::COMPLETED;
}

std::string manager::Job::getBldLoc()
{
  return m_database + "/_bld/" + m_toExec;
}

bool manager::Job::removeTasks(std::vector<Task> tasks)
{
  m_taskManager.removeFromResults(tasks);
}

bool manager::Job::removeTask(Task task)
{
  m_taskManager.removeFromResults(task);
}

