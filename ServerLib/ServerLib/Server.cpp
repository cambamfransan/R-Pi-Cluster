#include "Server.hpp"
#include "Logger/Logger.hpp"
#include "Messages/JSONHelpers.hpp"
#include "Messages/JSONParser.hpp"
#include "Messages/MakeJSONMsgs.hpp"
#include "Messages/MakeMsgs.hpp"
#include "Messages/MapHelpers.hpp"
#include "Msgs/ReceiveUpdateAck.hpp"
#include <iostream>
#include <qprocess.h>
#include <qthreadpool.h>
#include <qtimer.h>
#include <thread>

Server::Server(std::string arg)
  : m_pSender(std::make_shared<TCPSenderServer>()),
    m_pWebSender(std::make_shared<TCPSenderWeb>()),
    // m_clientsMutex(std::make_shared<std::mutex>()),
    // m_clientIds(
    // std::make_shared<std::map<int,
    // std::chrono::steady_clock::time_point>>()),
    // m_clientInfosMutex(),
    // m_clientInfos(),
    m_myId(1),
    m_pServerManager(std::make_shared<manager::ServerManager>(m_myId, m_pSender, m_pWebSender, arg)),
    connectedToWeb(false),
#if (TESTING_GUIS == 1)
    m_window(new MainWindow()),
#endif
    // m_outMessagesMutex(),
    //    m_outMessages(),
    //    m_webOutMessagesMutex(),
    //    m_webOutMessages(),
    //    m_inputMessages(),
    // m_nextPriority(1),
    m_pTimer(new QTimer(this)),
    m_database(arg)
{
#if (TESTING_GUIS == 1)
  m_window->show();
  connect(m_window, &MainWindow::signalClicked, this, &Server::clicked);
  m_window->setPort(m_pSender->getServerPort());
#endif
  connect(m_pSender.get(),
          &TCPSenderServer::newConnection,
          this,
          &Server::newConnection);
  std::cout << "Server Port: " << m_pSender->getServerPort() << std::endl;
  connect(m_pSender.get(),
          &TCPSenderServer::msgReceived,
          this,
          &Server::recieveMessage);
  connect(m_pSender.get(),
          &TCPSenderServer::lostConnection,
          this,
          &Server::lostConnection);
  std::cout << "Web Port: " << m_pWebSender->getServerPort() << std::endl;
  QString program = "node";
  QStringList arguments;
  arguments << "../../R-Pi-Cluster/Web/main.js"
            << QString::number(m_pWebSender->getServerPort())
            << QString::fromStdString(m_pSender->getServerIpAddress())
            << QString::number(m_pSender->getServerPort());

  QProcess* myProcess = new QProcess(this);
  myProcess->startDetached(program, arguments);

  connect(m_pWebSender.get(),
          &TCPSenderWeb::newConnection,
          this,
          &Server::newWebConnection);
  connect(m_pWebSender.get(),
          &TCPSenderWeb::msgReceived,
          this,
          &Server::receiveMessageWeb);
  connect(m_pWebSender.get(),
          &TCPSenderWeb::lostConnection,
          this,
          &Server::lostConnectionWeb);

  qRegisterMetaType<std::vector<manager::Task>>("std::vector<manager::Task>");
  // connect ServerManager
  connect(m_pServerManager.get(), &manager::ServerManager::tasksToSend, this, &Server::sendTasksToSend);

  // connect Timer
  connect(m_pTimer, &QTimer::timeout, this, &Server::sendTimedMsgs);
  m_pTimer->start(1000);
}

Server::~Server() {}

void Server::send(msg::MsgToSend* pMsg,
                  int convId,
                  std::chrono::seconds timeout,
                  bool requireResponse,
                  int endpointId)
{
  //  if (requireResponse)
  //  {
  //    {
  //      std::lock_guard<std::mutex> lock(m_outMessagesMutex);
  //      m_outMessages[pMsg->basicmsg().toid()][pMsg->basicmsg().convid()] =
  //        Conversation{
  //          pMsg, convId, timeout, std::chrono::steady_clock::now(),
  //          endpointId};
  //    }
  //  }
  Logger::info("Sent: " + std::to_string(m_pSender->send(pMsg, endpointId)));
}

void Server::sendToWeb(const std::string& msg,
                       int convId,
                       std::chrono::seconds timeout,
                       bool requireResponse)
{
  //  if (requireResponse)
  //  {
  //    std::lock_guard<std::mutex> lock(m_webOutMessagesMutex);
  //    m_webOutMessages[convId] =
  //      JSONConversation{msg, convId, timeout,
  //      std::chrono::steady_clock::now()};
  //  }
  m_pWebSender->send(msg + "~");
}

void Server::newConnection(int id)
{
  std::cout << "Adding Connection" << std::endl;
  {
    //(*m_clientIds)[id] = std::chrono::steady_clock::now();
  }
  auto t = m_pSender->getSocket(id);
#if (TESTING_GUIS == 1)
  m_window->addConnection(t->peerAddress(), t->peerPort());
#endif

  // m_nextPriority++;
  m_pServerManager->addPi(
    id, t->peerAddress().toString().toStdString(), t->peerPort());
  auto convId = m_pSender->getNextConvId();
  sendToWeb(json::makeJsonNewClient(convId, id, t->peerAddress().toString().toStdString()),
            convId,
            std::chrono::seconds(2),
            false);
}

void Server::newWebConnection()
{
  connectedToWeb = true;
  std::cout << "Adding Web Connection" << std::endl;
}

void Server::clicked(std::string msg)
{
  msg::MsgToSend* pMsg =
    make_msgs::makeTestMsg(m_myId, 0, m_pSender->getNextConvId(), msg);
  // std::lock_guard<std::mutex> gaurd(*m_clientsMutex);
  // for (auto&& id : *m_clientIds) // TODO implement test function
  //{
  // pMsg->mutable_basicmsg()->set_toid(id.first);
  // send(pMsg, 1, std::chrono::seconds(1), false, id.first);
  //}
}

void Server::recieveMessage(msg::MsgToSend* pMsg, QHostAddress ip, qint16 port)
{
//  int id = make_msgs::getMapId(
//    pMsg->basicmsg().fromid(), pMsg->basicmsg().convid()); // TODO
//  if (m_inputMessages.find(id) == m_inputMessages.end())
//    m_inputMessages[id] = std::chrono::steady_clock::now();
//  else
//  {
//    Logger::error("Duplicate: " + pMsg->DebugString());
//    return;
//  }
/*  {
    Logger::info("checking for if needed response");
    std::lock_guard<std::mutex> lock(m_outMessagesMutex);
    auto itr = m_outMessages.find(pMsg->basicmsg().fromid());
    if (itr != m_outMessages.end())
    {
      Logger::info("Looking for convId: " +
                   std::to_string(pMsg->basicmsg().convid()));
      auto itr2 = itr->second.find(pMsg->basicmsg().convid());
      Logger::info("Found client2");
      if (itr2 != itr->second.end())
      {
        Logger::info("Found convId");
        itr->second.erase(itr2);
      }
      else
      {
        Logger::info("Did not Found convId");
      }
    }
  }*/
#if (TESTING_GUIS == 1)
  m_window->receivedMsg(pMsg->DebugString(), ip, port);
#endif
  m_pServerManager->updateAck(pMsg->basicmsg().fromid());
  switch (pMsg->basicmsg().msgtype())
  {
  case msg::ProtoType::UPDATE_ACK:
    // receive_msgs::UpdateAckTask* heart =
    //  new receive_msgs::UpdateAckTask(*pMsg, m_clientsMutex, m_clientIds);
    // QThreadPool::globalInstance()->start(heart);
    break;
  case msg::ProtoType::ID_MSG_ACK:
    Logger::info("Pi is ready to go");
    break;
  case msg::ProtoType::TASK_MSG_ACK:
    Logger::info("Tasks received");
    break;
  case msg::ProtoType::RESULTS:
    m_pServerManager->addResults(pMsg);
    Logger::info("Results added");
    break;
  default:
    Logger::info("Did not catch message type: " +
                 std::to_string(pMsg->basicmsg().msgtype()));
  }
}

void Server::receiveMessageWeb(std::string msg)
{
  rapidjson::Document doc;
  doc.Parse(msg.c_str());
  json::JSONParser parser(doc);
  auto map = parser.getMap();
  Logger::info("received: " + msg);
  /*  if (m_inputMessages.find(id) == m_inputMessages.end())
      m_inputMessages[id] = std::chrono::steady_clock::now();
    else
    {
      Logger::error("Duplicate: " + msg);
      return;
    }*/
  /*  {
      Logger::info("checking for if needed response");
      std::lock_guard<std::mutex> lock(m_webOutMessagesMutex);
      auto itr = m_webOutMessages.find(id);
      if (itr != m_webOutMessages.end())
      {
        Logger::info("Found convId");
        m_webOutMessages.erase(itr);
      }
      else
      {
        Logger::info("Did not find convId");
      }
    }*/
  std::string type = map["/MsgType"];
  if (type == "HeartbeatAck")
  {
    //   std::cout << "Received HeartBeat Ack" << std::endl;
  }
  else if (type == "AddJob")
  {
    int convId = stoi(map["/convId"]);

    // future can make size configurable
    Logger::info("Need to add a job");

    int pri(std::stoi(map["/priority"])); 
    int tpb(std::stoi(map["/taskPerBundle"]));
    std::string remote(map["/remote"]);
    std::string name(map["/name"]);

    int jobId = m_pServerManager->getNextJobId();
    sendToWeb(json::makeJsonAddJobAck(convId, jobId, remote, name, pri, tpb), convId, std::chrono::seconds(3), false);
    Logger::info("Sent new Job Ack!");
    std::thread thr([this, pri, tpb, remote, jobId](){
        m_pServerManager->addJob(100, 
            pri,
            tpb,
            remote, jobId);
        });
    thr.detach();
  }
}

void Server::lostConnection(int id)
{
  // std::lock_guard<std::mutex> lock(m_outMessagesMutex);
  // if (auto itr = m_outMessages.find(id) != m_outMessages.end())
  //   m_outMessages.erase(itr);
  m_pServerManager->removePi(id);
  Logger::info("erased successfully client with Id" + std::to_string(id));
  int next(m_pWebSender->nextConvId());
  sendToWeb(json::makeJsonLostClient(next, id), next, std::chrono::seconds(2), false);  
}

void Server::lostConnectionWeb()
{
  std::cout << "erasedWeb" << std::endl;
  std::cout << "erasedWeb successful" << std::endl;
}

void Server::sendTimedMsgs()
{
  static int times(0);
  // send updates
  // if (times == 3)
  //{
  // std::cout << "Timed" << std::endl;
  m_pServerManager->sendUpdates();
  //}

  m_pServerManager->removeUnresponsive();

  // Resend Msgs
  /*  {
      std::lock_guard<std::mutex> lock(m_outMessagesMutex);
      for (auto&& client : m_outMessages)
      {
        for (auto&& conv : client.second)
        {
          if (conv.second.timeout + conv.second.timeSend <
              std::chrono::steady_clock::now())
          {
            if (conv.second.msg->basicmsg().attempt() > 3)
            {
              // TODO cfrandsen: disconnect pi
            }
            Logger::error("Resending message from conversation: " +
                          std::to_string(conv.second.convId));
            conv.second.msg->mutable_basicmsg()->set_attempt(
              conv.second.msg->basicmsg().attempt() + 1);
            conv.second.timeSend = std::chrono::steady_clock::now();
            m_pSender->send(conv.second.msg, conv.second.endpointId);
          }
        }
      }
    }*/

  // send heartbeats
  if (connectedToWeb)
  {
    int next(m_pWebSender->nextConvId());
    sendToWeb(
      json::makeJsonHeartbeat(next), next, std::chrono::seconds(2), true);

    next = m_pWebSender->nextConvId();
    sendToWeb(
        json::makeJsonProgress(next, m_pServerManager->getProgress()), next, std::chrono::seconds(3), false);
  }

  // if (times == 3)
  //{
  /*std::lock_guard<std::mutex> lock(m_outMessagesMutex);
  for (auto&& conv : m_webOutMessages)
  {
    if (conv.second.timeout + conv.second.timeSend <
        std::chrono::steady_clock::now())
    {
      std::cout << conv.second.timeout.count() * 1000000000 +
                     conv.second.timeSend.time_since_epoch().count()
                << std::endl;
      std::cout <<
  std::chrono::steady_clock::now().time_since_epoch().count()
                << std::endl;
      // check tries??
      Logger::error("Resending message from conversation: " +
                    std::to_string(conv.second.convId));
      conv.second.timeSend = std::chrono::steady_clock::now();
      m_pWebSender->send(conv.second.msg + "~");
    }
  }*/
  // times = 0;
  //}

  //  times++;
  // std::cout << "Done Timed" << std::endl;
}

void Server::sendTasksToSend(std::vector<manager::Task> tasks, int id)
{
  auto convId = m_pSender->getNextConvId();
  send(make_msgs::makeTaskMsg(m_myId, id, convId, tasks), convId, std::chrono::seconds(3), false, id);
}

