#ifndef SERVER_MANAGER_H
#define SERVER_MANAGER_H

#include "JobManager.hpp"
#include "Pi/PiServerManager.hpp"
#include "ProtoFiles/Results.pb.h"
#include "TCPSender/TCPSenderServer.hpp"
#include "TCPSender/TCPSenderWeb.hpp"
#include <string>

namespace manager
{
  class ServerManager
  {
  public:
    ServerManager(int id,
                  std::shared_ptr<TCPSenderServer> pServerSender,
                  std::shared_ptr<TCPSenderWeb> pWebSender,
                  std::string database);

    ~ServerManager();

    void receiveMsg(msg::MsgToSend* pMsg);

  private:

    void addResults(msg::MsgToSend* pMsg);

    void addPi(std::string ip, int port);

    void addJob(int id, int size, int pri, int taskpb, std::string gitUrl);

    //TODO add controls for job manager and pi manager


    int m_myId;
    manager::JobManager m_jobManager;
    manager::PiServerManager m_piManager;
    std::shared_ptr<TCPSenderServer> m_pServerSender;
    std::shared_ptr<TCPSenderWeb> m_pWebSender;
  };
} // namespace manager

#endif
