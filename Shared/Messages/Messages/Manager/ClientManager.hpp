//#ifndef SERVER_MANAGER_H
//#define SERVER_MANAGER_H
//
//#include <string>
//#include "JobManager.hpp"
//#include "Pi/PiServerManager.hpp"
//#include "TCPSender/TCPSenderServer.hpp"
//#include "TCPSender/TCPSenderWeb.hpp"
//#include "ProtoFiles/Results.pb.h"
//
//namespace manager
//{
//  class ServerManager
//  {
//    public:
//      ServerManager(TCPSenderServer pServerSender, TCPSenderWeb pWebSender);
//      ~ServerManager();
//
//      void addResults(msg::ResultsMsg* pMsg);
//
//      void addPi(std::string ip, int port);
//
//      void addJob(int id, int size, int pri, int taskpb, std::string gitUrl);
//
//    private:
//      manager::JobManager m_jobManager;
//      manager::PiServerManager m_piManager;
//      TCPSenderServer m_pServerSender;
//      TCPSenderWeb m_pWebSender;
//  };
//} // namespace json
//
//#endif
//
