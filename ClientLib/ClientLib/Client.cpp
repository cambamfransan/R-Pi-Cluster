#include "Client.hpp"
#include "Messages/MakeMsgs.hpp"
#include "Messages/MapHelpers.hpp"
#include <iostream>

Client::Client(QHostAddress addr, qint16 port)
  : m_pSender(std::make_shared<TCPSenderClient>(addr, port)),
    m_serverId(1),
    m_myId(0),
    m_window(new MainWindow()),
    m_outMessages(),
    m_inputMessages()
{
  m_window->show();
  connect(m_pSender.get(),
          &TCPSenderClient::newConnection,
          this,
          &Client::newConnection);
  connect(m_window, &MainWindow::signalClicked, this, &Client::clicked);
  connect(m_pSender.get(),
          &TCPSenderClient::msgReceived,
          this,
          &Client::recieveMessage);
  connect(m_pSender.get(), &TCPSenderClient::lostConnection, this, &Client::lostConnection);
}

Client::~Client() {}

void Client::newConnection()
{
  m_window->setPort(m_pSender->getLocalPort());
  std::cout << "New Connection" << std::endl;
  m_window->addConnection(
    m_pSender->getPeerAddress(), m_pSender->getPeerPort());
  std::cout << "Didn't Get here" << std::endl;
}

void Client::recieveMessage(msg::MsgToSend* pMsg, QHostAddress ip, qint16 port)
{
  std::cout << pMsg->DebugString() << std::endl;
  int id = make_msgs::getMapId(
    pMsg->basicmsg().fromid(), pMsg->basicmsg().convid());
  if (m_inputMessages.find(id) == m_inputMessages.end())
    m_inputMessages[id] = std::chrono::steady_clock::now();
  else
    return;
  if (auto itr = m_outMessages.find(make_msgs::getMapId(
    pMsg->basicmsg().toid(),
    pMsg->basicmsg().convid())) == m_outMessages.end())
    m_outMessages.erase(itr);
  m_window->receivedMsg(pMsg->DebugString(), ip, port);

  // This will be taken out sometime
  switch (pMsg->basicmsg().msgtype())
  {
  case msg::ProtoType::ID_MSG:
    m_myId = pMsg->basicmsg().toid();
    m_serverId = pMsg->basicmsg().fromid();
  }
}

void Client::send(msg::MsgToSend* pMsg,
                  int convId,
                  std::chrono::seconds timeout,
                  bool requireResponse)
{
  if (requireResponse)
  {
    m_outMessages[make_msgs::getMapId(m_myId, pMsg->basicmsg().convid())] =
      Conversation{
        pMsg, convId, timeout, std::chrono::steady_clock::now(), m_serverId};
  }
  m_pSender->send(pMsg);
}

void Client::clicked(std::string msg)
{
  msg::MsgToSend* pMsg = make_msgs::makeTestMsg(m_myId, 1, m_pSender->getNextConvId(), msg);
  std::cout << "Sending" << std::endl;
  send(pMsg, 1, std::chrono::seconds(1), false);
}

void Client::lostConnection()
{
  std::cout << "Client lost connection" << std::endl;
}
