#ifndef PI_SERVER_MANAGER_H
#define PI_SERVER_MANAGER_H

#include <string>
#include <map>
#include "Pi.hpp"
#include "Manager/Tasks.hpp"

namespace manager
{
  class PiServerManager
  {

    public:
    PiServerManager();
    ~PiServerManager();

    int addPi(std::string ip, int port);
    void removePi(int id);
    void changePiTasks(int id, std::vector<manager::Task> tasks);

  private:
    int m_nextPiId;
    int m_lastPriority;
    std::map<int, manager::Pi> m_pis;

  };

} // namespace manager

#endif
