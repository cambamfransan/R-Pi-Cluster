#include "Server.hpp"
#include "Logger/Logger.hpp"
#include "Messages/MakeMsgs.hpp"
#include "Messages/MapHelpers.hpp"
#include "Msgs/ReceiveHeartBeat.hpp"
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
    m_clientInfos(),
    m_myId(1),
    m_window(new MainWindow()),
    m_outMessagesMutex(),
    m_outMessages(),
    m_inputMessages(),
    m_pHeartBeatTimer(new QTimer(this)),
    m_pUpdateTimer(new QTimer(this)),
    m_pResendTimer(new QTimer(this))
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
  connect(m_pHeartBeatTimer, &QTimer::timeout, this, &Server::sendHeartBeats);
  m_pHeartBeatTimer->start(3000);

  connect(m_pUpdateTimer, &QTimer::timeout, this, &Server::sendUpdates);
  m_pHeartBeatTimer->start(3000);

  connect(m_pResendTimer, &QTimer::timeout, this, &Server::resendMsgs);
  m_pHeartBeatTimer->start(3000);
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
  std::cout << "In Sending" << std::endl;
  m_pSender->send(pMsg, endpointId);
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
  std::lock_guard<std::mutex> gaurd(*m_clientsMutex);
  (*m_clientIds)[id] = std::chrono::steady_clock::now();
  auto t = m_pSender->getSocket(id);
  m_window->addConnection(t->peerAddress(), t->peerPort());
  // this will move
  ClientInfo infoToInsert{t->peerAddress().toString().toStdString(),
                          t->peerPort(),
                          "pi",
                          "PiCluster!",
                          static_cast<int>(m_clientIds->size() + 1),
                          id};
  m_clientInfos.push_back(infoToInsert);
  if (m_clientInfos.size() > 1)
  {
    for (auto&& info : m_clientInfos)
    {
      if (info.priority == id - 1)
      {
        int nextConvId(m_pSender->getNextConvId());
        send(make_msgs::makeIdMsg(
               m_myId, id, nextConvId, info.ipAddress, info.port),
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
    send(make_msgs::makeIdMsg(m_myId,
                              infoToInsert.clientId,
                              nextConvId,
                              m_pSender->getServerIpAddress(),
                              m_pSender->getServerPort()),
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
    std::lock_guard<std::mutex> lock(m_outMessagesMutex);
    if (auto itr = m_outMessages.find(make_msgs::getMapId(
                     pMsg->basicmsg().toid(), pMsg->basicmsg().convid())) ==
                   m_outMessages.end())
      m_outMessages.erase(itr);
  }
  m_window->receivedMsg(pMsg->DebugString(), ip, port);
  int msgId = pMsg->basicmsg().msgtype();
  if (msg::ProtoType::HEART_BEAT_MSG_ACK == msgId)
  {
    receive_msgs::HeartBeatTask* heart =
      new receive_msgs::HeartBeatTask(*pMsg, m_clientsMutex, m_clientIds);
    QThreadPool::globalInstance()->start(heart);
  }
  else if (1 == msgId)
  {
  }
}

void Server::receiveMessageWeb(std::string msg)
{
  rapidjson::Document doc;
  doc.Parse(msg.c_str());
}

void Server::lostConnection(int id)
{
  std::cout << "erased" << std::endl;
  {
    std::lock_guard<std::mutex> lock(m_outMessagesMutex);
    m_outMessages.erase(m_outMessages.find(id));
  }
  {
    std::lock_guard<std::mutex> gaurd(*m_clientsMutex);
    m_clientIds->erase(m_clientIds->find(id));
  }
  std::cout << "erased successful" << std::endl;
}

void Server::lostConnectionWeb()
{
  std::cout << "erasedWeb" << std::endl;
  std::cout << "erasedWeb successful" << std::endl;
}

void Server::sendHeartBeats()
{
  auto pMsg =
    make_msgs::makeBasicMsgToSend(m_myId, 0, msg::ProtoType::HEART_BEAT_MSG, 0);
  std::lock_guard<std::mutex> gaurd(*m_clientsMutex);
  for (auto&& id : *m_clientIds)
  {
    pMsg->mutable_basicmsg()->set_toid(id.first);
    int nextConvId(m_pSender->getNextConvId());
    pMsg->mutable_basicmsg()->set_convid(nextConvId);
    send(pMsg, nextConvId, std::chrono::seconds(1), true, id.first);
  }
}

void Server::sendUpdates()
{
  std::lock_guard<std::mutex> gaurd(*m_clientsMutex);
  int id(0);
  for (auto&& info : m_clientInfos)
  {
    if (info.priority == 1)
    {
      id = info.clientId;
      break;
    }
  }
  int nextConvId(m_pSender->getNextConvId());
  auto pMsg = make_msgs::makeUpdateMsg(
    m_myId, id, msg::ProtoType::UPDATE, nextConvId, m_clientInfos);

  send(pMsg, nextConvId, std::chrono::seconds(1), true, id);
}

void Server::resendMsgs()
{
  std::lock_guard<std::mutex> lock(m_outMessagesMutex);
  for (auto&& client : m_outMessages)
  {
    for (auto&& conv : client.second)
    {
      if (conv.second.timeout + conv.second.timeSend >
          std::chrono::steady_clock::now())
      {
        Logger::error("Resending message from conversation: " +
                      conv.second.convId);
        m_pSender->send(conv.second.msg, conv.second.endpointId);
      }
    }
  }
}
