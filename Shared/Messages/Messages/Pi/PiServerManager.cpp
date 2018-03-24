#include "PiServerManager.hpp"
#include "Logger/Logger.hpp"

manager::PiServerManager::PiServerManager()
  : m_nextPiId(2),
    m_lastPriority(1),
    m_pisMutex(),
    m_pis(),
    m_updateMutex(),
    m_newPis(),
    m_lostPis()
{
}

manager::PiServerManager::~PiServerManager() {}

int manager::PiServerManager::addPi(std::string ip, int port)
{
  int nextId(m_nextPiId);
  m_nextPiId++;
  manager::Pi pi(ip, port, "", "", m_lastPriority++, nextId);
  {
    std::lock_guard<std::mutex> lock(m_pisMutex);
    Logger::info("Adding Pi: " + std::to_string(nextId));
    m_pis[nextId] = pi;
  }
  {
    std::lock_guard<std::mutex> lock(m_updateMutex);
    m_newPis.push_back(pi);
  }
  return nextId;
}

void manager::PiServerManager::removePi(int id)
{
  {
    std::lock_guard<std::mutex> lock(m_updateMutex);
    m_lostPis.push_back(id);
  }
  m_lastPriority--;
  {
    std::lock_guard<std::mutex> lock(m_pisMutex);
    for (auto&& pi : m_pis)
    {
      if (pi.second.getPriority() > m_pis[id].getPriority())
      {
        pi.second.decrementPriority();
      }
    }
    m_pis.erase(id);
  }
  Logger::error("Removed pi: " + std::to_string(id));
}

void manager::PiServerManager::changePiTasks(
  int id,
  std::vector<manager::Task> completed,
  std::vector<manager::Task> tasks)
{
  std::lock_guard<std::mutex> lock(m_pisMutex);
  m_pis[id].replaceTasks(completed, tasks);
}

int manager::PiServerManager::getAmountToSend(int id)
{
  std::lock_guard<std::mutex> lock(m_pisMutex);
  return m_pis[id].getAmountToSend();
}

int manager::PiServerManager::waitingPis()
{
  std::lock_guard<std::mutex> lock(m_pisMutex);
  for (auto&& pi : m_pis)
  {
    if (pi.second.getTasks().empty())
    {
      return pi.first;
    }
  }
  return -1;
}

void manager::PiServerManager::updateAck(int id)
{
  std::lock_guard<std::mutex> lock(m_pisMutex);
  m_pis[id].updateAck();
}

void manager::PiServerManager::removeUnresponsive()
{
  std::vector<int> toDelete;
  {
    std::lock_guard<std::mutex> lock(m_pisMutex);
    for (const auto& pi : m_pis)
    {
      if (pi.second.getlastCom() + std::chrono::seconds(5) <
          std::chrono::steady_clock::now())
      {
        toDelete.push_back(pi.first);
      }
    }
  }

  for (const auto& id : toDelete)
  {
    removePi(id);
  }
}

std::pair<std::vector<manager::Pi>, std::vector<int>> manager::PiServerManager::
  getUpdates()
{
  std::lock_guard<std::mutex> lock(m_updateMutex);
  auto toReturn = std::make_pair(m_newPis, m_lostPis);
  m_newPis.clear();
  m_lostPis.clear();
  return toReturn;
}

std::vector<int> manager::PiServerManager::getClientIds()
{
  std::vector<int> toReturn;
  std::lock_guard<std::mutex> lock(m_pisMutex);
  for (const auto& pi : m_pis)
    toReturn.push_back(pi.first);

  return toReturn;
}
