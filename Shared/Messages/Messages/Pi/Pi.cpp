#include "Pi.hpp"
#include <algorithm>
#include <iostream>

manager::Pi::Pi()
  : m_ipAddress(),
    m_port(),
    m_username(),
    m_password(),
    m_priority(),
    m_clientId(),
    m_threads(),
    m_lastCom(std::chrono::steady_clock::now()),
    m_tasks()
{
}

manager::Pi::Pi(std::string ip,
                int port,
                std::string user,
                std::string pass,
                int priority,
                int id)
  : m_ipAddress(ip),
    m_port(port),
    m_username(user),
    m_password(pass),
    m_priority(priority),
    m_clientId(id),
    m_threads(4),
    m_lastCom(std::chrono::steady_clock::now()),
    m_tasks()
{
}

manager::Pi::Pi(const Pi& pi)
  : m_ipAddress(pi.getIpAddress()),
    m_port(pi.getPort()),
    m_username(pi.getUsername()),
    m_password(pi.getPassword()),
    m_priority(pi.getPriority()),
    m_clientId(pi.getClientId()),
    m_threads(pi.getThreads()),
    m_lastCom(std::chrono::steady_clock::now()),
    m_tasks()
{
}

manager::Pi::~Pi()
{
  std::cout << "it deconstructed" << std::endl;
  // I don't think i need to do anything here yet
}

void manager::Pi::replaceTasks(std::vector<manager::Task> completed,
                               std::vector<manager::Task> tasks)
{
  std::vector<int> toDelete;
  for (auto&& comp : completed)
  {
    m_tasks.erase(std::find(m_tasks.begin(), m_tasks.end(), comp));
  }
  m_tasks.insert(std::end(m_tasks), std::begin(tasks), std::end(tasks));
}

std::string manager::Pi::toString()
{
  return m_ipAddress + "~" + std::to_string(m_port) + "~" + m_username + "~" +
         m_password + "~" + std::to_string(m_priority) + "~" +
         std::to_string(m_clientId);
}

manager::Pi manager::Pi::fromString(std::string str)
{
  auto spot = str.find("~");
  std::string address(str.substr(0, spot));
  str.erase(0, spot + 1);

  spot = str.find("~");
  int port(std::stoi(str.substr(0, spot)));
  str.erase(0, spot + 1);

  spot = str.find("~");
  std::string username(str.substr(0, spot));
  str.erase(0, spot + 1);

  spot = str.find("~");
  std::string password(str.substr(0, spot));
  str.erase(0, spot + 1);

  spot = str.find("~");
  int priority(std::stoi(str.substr(0, spot)));
  str.erase(0, spot + 1);

  spot = str.find("~");
  int clientId(std::stoi(str.substr(0, spot)));

  return Pi(address, port, username, password, priority, clientId);
}

bool manager::Pi::operator==(const Pi& pi)
{
  // this could be more efficient
  return m_ipAddress == pi.m_ipAddress && m_port == pi.m_port &&
         m_username == pi.m_username && m_password == pi.m_password &&
         m_priority == pi.m_priority && m_clientId == pi.m_clientId;
}

bool manager::Pi::operator!=(const Pi& pi)
{
  // this could be more efficient
  return !(m_ipAddress == pi.m_ipAddress && m_port == pi.m_port &&
           m_username == pi.m_username && m_password == pi.m_password &&
           m_priority == pi.m_priority && m_clientId == pi.m_clientId);
}

std::string manager::Pi::getIpAddress() const
{
  return m_ipAddress;
}

int manager::Pi::getPort() const
{
  return m_port;
}

std::string manager::Pi::getUsername() const
{
  return m_username;
}

std::string manager::Pi::getPassword() const
{
  return m_password;
}

int manager::Pi::getPriority() const
{
  return m_priority;
}

int manager::Pi::getClientId() const
{
  return m_clientId;
}

std::vector<manager::Task> manager::Pi::getTasks() const
{
  return m_tasks;
}

int manager::Pi::getThreads() const
{
  return m_threads;
}

void manager::Pi::decrementPriority()
{
  m_priority--;
}

void manager::Pi::changePriority(int priority)
{
  m_priority = priority;
}

void manager::Pi::changeThreads(int threads)
{
  m_threads = threads;
}

int manager::Pi::getAmountToSend() const
{
  return m_threads * 3 - m_tasks.size();
}

std::chrono::steady_clock::time_point manager::Pi::getlastCom() const
{
  return m_lastCom;
}

void manager::Pi::updateAck()
{
  m_lastCom = std::chrono::steady_clock::now();
}
