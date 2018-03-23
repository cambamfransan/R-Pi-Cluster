#include "PiServerManager.hpp"

manager::PiServerManager::PiServerManager()
  : m_nextPiId(1), m_pis(), m_lastPriority(1)
{
}

manager::PiServerManager::~PiServerManager() {}

int manager::PiServerManager::addPi(std::string ip, int port)
{
  manager::Pi pi(ip, port, "", "", m_lastPriority++, m_nextPiId++);
  m_pis[pi.getClientId()] = pi;
  return m_nextPiId;
}

void manager::PiServerManager::removePi(int id)
{
  m_lastPriority--;
  m_pis.erase(id);
}

void manager::PiServerManager::changePiTasks(
  int id,
  std::vector<manager::Task> completed,
  std::vector<manager::Task> tasks)
{
  m_pis[id].replaceTasks(completed, tasks);
}

int manager::PiServerManager::getAmountToSend(int id)
{
  return m_pis[id].getAmountToSend();
}
