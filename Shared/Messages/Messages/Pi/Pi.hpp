#ifndef PI_H
#define PI_H

#include <string>
#include "Messages/Manager/Tasks.hpp"

namespace manager
{
  class Pi
  {
  public:
    Pi();
    Pi(std::string ip, int port, std::string user, std::string pass, int priority, int id);
    ~Pi();

    void replaceTasks(std::vector<manager::Task> tasks);
    std::string toString();
    static Pi fromString(std::string);
    bool operator==(const Pi&);
    bool operator!=(const Pi&);

    std::string getIpAddress();
    int getPort();
    std::string getUsername();
    std::string getPassword();
    int getPriority();
    int getClientId();
    std::vector<manager::Task> getTasks();

  private:  
    /** Ip Address of the client */
    std::string m_ipAddress;
    /** port of the clinet */
    int m_port;
    /** username on the pi */
    std::string m_username;
    /** password for the username */
    std::string m_password;
    /** What priority in line */
    int m_priority;
    /** The clients Id*/
    int m_clientId;
    /** Vector of current tasks*/
    std::vector<manager::Task> m_tasks;
  };
} // namespace manager

#endif