#include "Pi.hpp"

manager::Pi::Pi()
  : m_ipAddress(),
  m_port(),
  m_username(),
  m_password(),
  m_priority(),
  m_clientId(),
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
    m_tasks()
{
}

manager::Pi::~Pi()
{
  // I don't think i need to do anything here yet
}

void manager::Pi::replaceTasks(std::vector<manager::Task> tasks)
{
  m_tasks = tasks;
}

std::string manager::Pi::toString()
{
  return m_ipAddress + "~" + std::to_string(m_port) + "~" + m_username + "~" +
         m_password + "~" + std::to_string(m_priority) +
         "~" + std::to_string(m_clientId);
}

manager::Pi manager::Pi::fromString(std::string str) 
{
  auto spot = str.find("~");
  std::string address(str.substr(0, spot));
  str.erase(0, spot+1);

  spot = str.find("~");
  int port(std::stoi(str.substr(0, spot)));
  str.erase(0, spot+1);

  spot = str.find("~");
  std::string username(str.substr(0, spot));
  str.erase(0, spot+1);

  spot = str.find("~");
  std::string password(str.substr(0, spot));
  str.erase(0, spot+1);

  spot = str.find("~");
  int priority(std::stoi(str.substr(0, spot)));
  str.erase(0, spot+1);

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

std::string manager::Pi::getIpAddress()
{
  return m_ipAddress;
}

int manager::Pi::getPort()
{
  return m_port;
}

std::string manager::Pi::getUsername()
{
  return m_username;
}

std::string manager::Pi::getPassword()
{
  return m_password;
}

int manager::Pi::getPriority()
{
  return m_priority;
}

int manager::Pi::getClientId()
{
  return m_clientId;
}

std::vector<manager::Task> manager::Pi::getTasks()
{
  return m_tasks;
}
