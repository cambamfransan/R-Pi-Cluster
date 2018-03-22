#ifndef SERVER_MANAGER_H
#define SERVER_MANAGER_H

#include <string>
#include "Job.hpp"
#include "PiServerManager.cpp"
#include "TCPSenderServer.hpp"
#include "TCPSenderWeb.hpp"
#include "ProtoFiles/Results.pb.c"

namespace manager
{
  class ServerManager
  {
    public:
      ServerManager(TCPSenderServer pServerSender, TCPSenderWeb, pWebSender);
      ~ServerManager();

      void addResults(msg::resultsMsg* pMsg);

      void addPi(std::string ip, int port);

      void addJob();

    private:
      manager::JobManager m_jobManager;
      manager::PiServerManager m_piManager;
      TCPSenderServer m_pServerSender;
      TCPSenderWeb m_pWebSender;
  };
} // namespace json

#endif

