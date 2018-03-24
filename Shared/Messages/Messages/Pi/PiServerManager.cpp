#include "PiServerManager.hpp"

manager::PiServerManager::PiServerManager()
  : m_nextPiId(1), m_lastPriority(1), m_pisMutex(), m_pis()
{
}

manager::PiServerManager::~PiServerManager() {}

int manager::PiServerManager::addPi(std::string ip, int port)
{
  manager::Pi pi(ip, port, "", "", m_lastPriority++, m_nextPiId++);
  {
    std::lock_guard<std::mutex> lock(m_pisMutex);
    m_pis[pi.getClientId()] = pi;
  }

  return m_nextPiId;
}

void manager::PiServerManager::removePi(int id)
{
  m_lastPriority--;
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

