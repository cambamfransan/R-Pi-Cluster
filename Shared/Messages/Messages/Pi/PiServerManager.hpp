#ifndef PI_SERVER_MANAGER_H
#define PI_SERVER_MANAGER_H

#include "Messages/Manager/Tasks.hpp"
#include "Pi.hpp"
#include <map>
#include <string>
#include <mutex>

namespace manager
{
  class PiServerManager
  {

  public:
    PiServerManager();
    ~PiServerManager();

    int addPi(std::string ip, int port);
    void removePi(int id);
    void changePiTasks(int id,
                       std::vector<manager::Task> completed,
                       std::vector<manager::Task> tasks);
    int getAmountToSend(int id);
    int waitingPis();
    void updateAck(int id);
    void removeUnresponsive();
    std::pair<std::vector<Pi>, std::vector<int>> getUpdates();
    std::vector<int> getClientIds();

  private:
    int m_nextPiId;
    int m_lastPriority;
    
    std::mutex m_pisMutex;
    std::map<int, manager::Pi> m_pis;
    std::mutex m_updateMutex;
    std::vector<Pi> m_newPis;
    std::vector<int> m_lostPis;
  };

} // namespace manager

#endif
