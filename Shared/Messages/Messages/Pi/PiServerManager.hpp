#ifndef PI_SERVER_MANAGER_H
#define PI_SERVER_MANAGER_H

#include "Messages/Manager/Tasks.hpp"
#include "Pi.hpp"
#include <map>
#include <string>
#include <mutex>
#include <memory>

namespace manager
{
  struct PiUpdateStruct
  {
    std::vector<std::shared_ptr<Pi>> newPis;
    std::vector<int> lostPis;
    std::vector<ModifiedPi> modifiedPis;
  };

  const std::string PI_PRIORITY("Priority");
  const std::string PI_THREADS("Threads");

  class PiServerManager
  {

  public:
    PiServerManager();
    ~PiServerManager();

    void addPi(int id, std::string ip, int port);
    void removePi(int id);
    void changePiTasks(int id,
                       std::vector<manager::Task> completed,
                       std::vector<manager::Task> tasks);
    int getAmountToSend(int id);
    int waitingPis();
    void updateAck(int id);
    void removeUnresponsive();
    PiUpdateStruct getUpdates();
    std::vector<int> getClientIds();
    void modifyPriority(int id, int priority);
    void modifyThreads(int id, int threads);

  private:
    int m_lastPriority;
    
    std::mutex m_pisMutex;
    std::map<int, std::shared_ptr<manager::Pi>> m_pis;
    std::mutex m_updateMutex;
    std::vector<std::shared_ptr<Pi>> m_newPis;
    std::vector<int> m_lostPis;
    std::vector<ModifiedPi> m_modifiedPis;
  };

} // namespace manager

#endif
