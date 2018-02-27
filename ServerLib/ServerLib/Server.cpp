#include "Server.hpp"
#include "Logger/Logger.hpp"
#include "Messages/MakeMsgs.hpp"
#include "Messages/MapHelpers.hpp"
#include "Msgs/ReceiveUpdateAck.hpp"
#include <iostream>
#include <qprocess.h>
#include <qthreadpool.h>
#include <qtimer.h>

Server::Server()
  : m_pSender(std::make_shared<TCPSenderServer>()),
    m_pWebSender(std::make_shared<TCPSenderWeb>()),
    m_clientsMutex(std::make_shared<std::mutex>()),
    m_clientIds(
      std::make_shared<std::map<int, std::chrono::steady_clock::time_point>>()),
    m_clientInfosMutex(),
    m_clientInfos(),
    m_myId(1),
    m_window(new MainWindow()),
    m_outMessagesMutex(),
    m_outMessages(),
    m_inputMessages(),
    m_pTimer(new QTimer(this)),
    m_nextPriority(1)
{
  m_window->show();
  connect(m_pSender.get(),
          &TCPSenderServer::newConnection,
          this,
          &Server::newConnection);
  m_window->setPort(m_pSender->getServerPort());
  connect(m_window, &MainWindow::signalClicked, this, &Server::clicked);
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
            << QString::number(m_pWebSender->getServerPort());

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
  if (requireResponse)
  {
    {
      std::lock_guard<std::mutex> lock(m_outMessagesMutex);
      m_outMessages[m_myId][pMsg->basicmsg().convid()] = Conversation{
        pMsg, convId, timeout, std::chrono::steady_clock::now(), endpointId};
    }
  }
  Logger::info("Sent: " + std::to_string(m_pSender->send(pMsg, endpointId)));
}

void Server::sendToWeb(const rapidjson::Document& msg,
                       int convId,
                       std::chrono::seconds timeout,
                       bool requireResponse,
                       int endpointId)
{
  m_pWebSender->send(msg);
}

void Server::newConnection(int id)
{
  std::cout << "Adding Connection" << std::endl;
  {
    std::lock_guard<std::mutex> gaurd(*m_clientsMutex);
    (*m_clientIds)[id] = std::chrono::steady_clock::now();
  }
  auto t = m_pSender->getSocket(id);
  m_window->addConnection(t->peerAddress(), t->peerPort());
  // this will move
  ClientInfo infoToInsert{t->peerAddress().toString().toStdString(),
                          t->peerPort(),
                          "pi",
                          "PiCluster!",
                          m_nextPriority,
                          id};
  m_nextPriority++;
  std::lock_guard<std::mutex> gaurd(m_clientInfosMutex);
  m_clientInfos[id] = infoToInsert;
  if (m_clientInfos.size() > 1)
  {
    for (auto&& info : m_clientInfos)
    {
      if (info.second.priority == m_nextPriority - 2)
      {
        int nextConvId(m_pSender->getNextConvId());
        send(make_msgs::makeIdMsg(m_myId, id, nextConvId),
             nextConvId,
             std::chrono::seconds(1),
             true,
             id);
        return;
      }
    }
    Logger::error("No Id Msg Sent");
  }
  else if (m_clientInfos.size() == 1)
  {
    int nextConvId(m_pSender->getNextConvId());
    send(make_msgs::makeIdMsg(m_myId, infoToInsert.clientId, nextConvId),
         nextConvId,
         std::chrono::seconds(1),
         true,
         id);
  }
}

void Server::newWebConnection()
{
  std::cout << "Adding Web Connection" << std::endl;
}

void Server::clicked(std::string msg)
{
  msg::MsgToSend* pMsg =
    make_msgs::makeTestMsg(m_myId, 0, m_pSender->getNextConvId(), msg);
  std::lock_guard<std::mutex> gaurd(*m_clientsMutex);
  for (auto&& id : *m_clientIds)
  {
    pMsg->mutable_basicmsg()->set_toid(id.first);
    send(pMsg, 1, std::chrono::seconds(1), false, id.first);
  }
}

void Server::recieveMessage(msg::MsgToSend* pMsg, QHostAddress ip, qint16 port)
{
  int id =
    make_msgs::getMapId(pMsg->basicmsg().fromid(), pMsg->basicmsg().convid());
  if (m_inputMessages.find(id) == m_inputMessages.end())
    m_inputMessages[id] = std::chrono::steady_clock::now();
  else
  {
    Logger::error("Duplicate: " + pMsg->DebugString());
    return;
  }
  {
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
  }
  m_window->receivedMsg(pMsg->DebugString(), ip, port);
  int msgId = pMsg->basicmsg().msgtype();
  if (msg::ProtoType::UPDATE_ACK == msgId)
  {
    receive_msgs::UpdateAckTask* heart =
      new receive_msgs::UpdateAckTask(*pMsg, m_clientsMutex, m_clientIds);
    QThreadPool::globalInstance()->start(heart);
  }
}

void Server::receiveMessageWeb(std::string msg)
{
  rapidjson::Document doc;
  doc.Parse(msg.c_str());
}

void Server::lostConnection(int id)
{
  Logger::info("erasing client with Id" + std::to_string(id));
  {
    std::lock_guard<std::mutex> lock(m_outMessagesMutex);
    if (auto itr = m_outMessages.find(id) != m_outMessages.end())
      m_outMessages.erase(itr);
  }
  Logger::info("erasing conversations for client with Id" + std::to_string(id));
  {
    std::lock_guard<std::mutex> gaurd(*m_clientsMutex);
    m_clientIds->erase(m_clientIds->find(id));
  }
  {
    std::lock_guard<std::mutex> gaurd(m_clientInfosMutex);
    for (auto& info : m_clientInfos)
    {
      if (info.second.priority > m_clientInfos[id].priority)
      {
        info.second.priority--;
      }
    }
    m_clientInfos.erase(id);
  }
  Logger::info("erasing successfully client with Id" + std::to_string(id));
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
  if (times == 3)
  {
    std::lock_guard<std::mutex> gaurd(m_clientInfosMutex);
    for (auto&& info : m_clientInfos)
    {
      int nextConvId(m_pSender->getNextConvId());
      auto pMsg = make_msgs::makeUpdateMsg(m_myId,
                                           info.second.clientId,
                                           msg::ProtoType::UPDATE,
                                           nextConvId,
                                           m_clientInfos);

      send(
        pMsg, nextConvId, std::chrono::seconds(1), true, info.second.clientId);
    }
    times = 0;
  }

  // Resend Msgs
  {
    std::lock_guard<std::mutex> lock(m_outMessagesMutex);
    for (auto&& client : m_outMessages)
    {
      for (auto&& conv : client.second)
      {
        if (conv.second.timeout + conv.second.timeSend >
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
  }
  times++;
}
