#ifndef CLIENT_MANAGER_H
#define CLIENT_MANAGER_H

#include "ClientResultsManager.hpp"
#include "ExecuteManager.hpp"
#include "Messages/Pi/PiClientManager.hpp"
#include "TCPSender/TCPSenderClient.hpp"
#include "TCPSender/TCPSenderWeb.hpp"
#include <string>

namespace manager
{
  class ClientManager
  {
  public:
    ClientManager(std::shared_ptr<TCPSenderClient> pSenderClient,
                  std::string database);
    ~ClientManager();

    void update(const msg::Update pMsg);

    void execute(msg::TaskMsg* pMsg);

  private:
    std::shared_ptr<TCPSenderClient> m_pSenderClient;
    std::string m_database;
    manager::ExecuteManager m_executeManager;
    manager::PiClientManager m_piManager;
    manager::ClientResultsManager m_resultsManager;
  };
} // namespace manager

#endif
