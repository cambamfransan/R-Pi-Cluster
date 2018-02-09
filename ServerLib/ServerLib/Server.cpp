#include "Server.hpp"
#include "Messages/MakeMsgs.hpp"
#include "Messages/MapHelpers.hpp"
#include <qprocess.h>
#include <iostream>

Server::Server()
  : m_pSender(std::make_shared<TCPSenderServer>()),
    m_pWebSender(std::make_shared<TCPSenderWeb>()),
    m_clientIds(),
    m_myId(1),
    m_window(new MainWindow()),
    m_outMessages(),
    m_inputMessages()
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
  arguments << "../../R-Pi-Cluster/Web/main.js" << QString::number(m_pWebSender->getServerPort());

  QProcess *myProcess = new QProcess(this);
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
    m_outMessages[make_msgs::getMapId(m_myId, pMsg->basicmsg().convid())] =
      Conversation{
        pMsg, convId, timeout, std::chrono::steady_clock::now(), endpointId};
  }
  std::cout << "In Sending" << std::endl;
  m_pSender->send(pMsg, endpointId);
}

void Server::sendToWeb(const rapidjson::Document& msg,
  int convId,
  std::chrono::seconds timeout,
  bool requireResponse, int endpointId)
{
  m_pWebSender->send(msg);
}

void Server::newConnection(int id)
{
  std::cout << "Adding Connection" << std::endl;
  m_clientIds.push_back(id);
  auto t = m_pSender->getSocket(id);
  m_window->addConnection(t->peerAddress(), t->peerPort());
}

void Server::newWebConnection()
{
  std::cout << "Adding Web Connection" << std::endl;
}

void Server::clicked(std::string msg)
{
  msg::MsgToSend* pMsg = make_msgs::makeTestMsg(m_myId, 0, m_pSender->getNextConvId(), msg);
  for (auto&& id : m_clientIds)
  {
    pMsg->mutable_basicmsg()->set_toid(id);
    send(pMsg, 1, std::chrono::seconds(1), false, id);
  }
}

void Server::recieveMessage(msg::MsgToSend* pMsg, QHostAddress ip, qint16 port)
{
  std::cout << "ids: " << pMsg->basicmsg().fromid() << " " << pMsg->basicmsg().convid() << std::endl;
  std::cout << "1: " << make_msgs::getMapId(pMsg->basicmsg().fromid(), pMsg->basicmsg().convid())<< std::endl;
  int id = make_msgs::getMapId(pMsg->basicmsg().fromid(), pMsg->basicmsg().convid());
  if (m_inputMessages.find(id) == m_inputMessages.end())
    m_inputMessages[id] = std::chrono::steady_clock::now();
  else
  {
    std::cout << "Duplicate" << std::endl;
    return;
  }
  if (auto itr = m_outMessages.find(make_msgs::getMapId(
                   pMsg->basicmsg().toid(), pMsg->basicmsg().convid())) ==
                 m_outMessages.end())
    m_outMessages.erase(itr);
  std::cout << "2" << std::endl;
  m_window->receivedMsg(pMsg->DebugString(), ip, port);
}

void Server::receiveMessageWeb(std::string msg)
{
  rapidjson::Document doc;
  doc.Parse(msg.c_str());
}

void Server::lostConnection(int id)
{
  std::cout << "erased" << std::endl;
  m_clientIds.erase(std::find(m_clientIds.begin(), m_clientIds.end(), id));
  std::cout << "erased successful" << std::endl;
}

void Server::lostConnectionWeb()
{
  std::cout << "erasedWeb" << std::endl;
  std::cout << "erasedWeb successful" << std::endl;
}
