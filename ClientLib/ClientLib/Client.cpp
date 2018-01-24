#include "Client.hpp"

Client::Client(QHostAddress addr, qint16 port)
  : m_pSender(std::make_shared<TCPSenderClient>(addr, port)),
    m_serverId(1),
    m_window(new MainWindow())
{
  m_window->show();
  connect(
    m_pSender.get(), &TCPSenderClient::newConnection, this, &Client::newConnection);
  connect(m_window, &MainWindow::signalClicked, this, &Client::clicked);
  connect(m_pSender.get(), &TCPSenderClient::msgReceived, this, &Client::recieveMessage);
}

Client::~Client() {}

void Client::newConnection()
{
  m_window->setPort(m_pSender->getLocalPort());
  std::cout << "New Connection" << std::endl;
  m_window->addConnection(m_pSender->getPeerAddress(), m_pSender->getPeerPort());
  std::cout << "Didn't Get here" << std::endl;
}

void Client::recieveMessage(msg::MsgToSend* pMsg, QHostAddress ip, qint16 port)
{
  std::cout << pMsg->DebugString() << std::endl;
  m_window->receivedMsg(pMsg->DebugString(), ip, port);
}

void Client::clicked(std::string msg)
{
  msg::MsgToSend* pMsg = new msg::MsgToSend();
  pMsg->mutable_test()->add_teststring(msg);
  std::cout << "Sending" << std::endl;
  m_pSender->send(pMsg, 1, std::chrono::seconds(1), false);
}
