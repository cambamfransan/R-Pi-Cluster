#include "PiServerManager.hpp"
#include "Logger/Logger.hpp"
#include <iostream>

manager::PiServerManager::PiServerManager()
  : m_lastPriority(1),
    m_pisMutex(),
    m_pis(),
    m_updateMutex(),
    m_newPis(),
    m_lostPis(),
    m_modifiedPis()
{
}

manager::PiServerManager::~PiServerManager() {}

void manager::PiServerManager::addPi(int id, std::string ip, int port)
{
  auto pi = std::make_shared<manager::Pi>(ip, port, "", "", m_lastPriority++, id);
  {
    std::lock_guard<std::mutex> lock(m_pisMutex);
    Logger::info("Adding Pi: " + std::to_string(id));
    m_pis[id] = pi;
  }
  {
    std::lock_guard<std::mutex> lock(m_updateMutex);
    m_newPis.emplace_back(ip, port, "", "", m_lastPriority, id);
  }
}

void manager::PiServerManager::removePi(int id)
{
    std::lock_guard<std::mutex> lock(m_updateMutex);
    m_lostPis.push_back(id);
  m_lastPriority--;
  {
    std::lock_guard<std::mutex> lock(m_pisMutex);
    for (auto&& pi : m_pis)
    {
      if (pi.second->getPriority() > m_pis[id]->getPriority())
      {
        pi.second->decrementPriority();
        m_modifiedPis.push_back(
          manager::ModifiedPi{ pi.first, PI_PRIORITY, std::to_string(pi.second->getPriority()) });
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
  m_pis[id]->replaceTasks(completed, tasks);
}

int manager::PiServerManager::getAmountToSend(int id)
{
  std::lock_guard<std::mutex> lock(m_pisMutex);
  return m_pis[id]->getAmountToSend();
}

int manager::PiServerManager::waitingPis()
{
  std::lock_guard<std::mutex> lock(m_pisMutex);
  for (auto&& pi : m_pis)
  {
    if (pi.second->getTasks().empty())
    {
      return pi.first;
    }
  }
  return -1;
}

void manager::PiServerManager::updateAck(int id)
{
  std::lock_guard<std::mutex> lock(m_pisMutex);
  m_pis[id]->updateAck();
}

void manager::PiServerManager::removeUnresponsive()
{
  std::vector<int> toDelete;
  {
    std::lock_guard<std::mutex> lock(m_pisMutex);
    for (const auto& pi : m_pis)
    {
      if (pi.second->getlastCom() + std::chrono::seconds(5) <
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

manager::PiUpdateStruct manager::PiServerManager::getUpdates()
{
  std::lock_guard<std::mutex> lock(m_updateMutex);
  auto toReturn = PiUpdateStruct{m_newPis, m_lostPis, m_modifiedPis};
  m_newPis.clear();
  m_lostPis.clear();
  m_modifiedPis.clear();
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

void manager::PiServerManager::modifyPriority(int id, int priority)
{
  m_pis[id]->changePriority(priority);
  std::lock_guard<std::mutex> lock(m_updateMutex);
  m_modifiedPis.push_back(
    manager::ModifiedPi{id, PI_PRIORITY, std::to_string(priority)});
}

void manager::PiServerManager::modifyThreads(int id, int threads)
{
  m_pis[id]->changeThreads(threads);
  std::lock_guard<std::mutex> lock(m_updateMutex);
  m_modifiedPis.push_back(
    manager::ModifiedPi{id, PI_THREADS, std::to_string(threads)});
}

std::vector<manager::Pi> manager::PiServerManager::getCurrentState()
{
  std::vector<manager::Pi> forReturn;
  for(const auto& pi : m_pis)
  {
    std::cout << "id: " << pi.first << std::endl;
    forReturn.push_back(pi.second->makeCopy());
  }
  return forReturn;
}
    
