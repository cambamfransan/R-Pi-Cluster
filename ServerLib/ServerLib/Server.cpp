#include "Server.hpp"

Server::Server()
  : m_pSender(std::make_shared<TCPSenderServer>()), m_window(new MainWindow())
{
  m_window->show();
  connect(m_pSender.get(), &TCPSenderServer::newConnection, this, &Server::newConnection);
  m_window->setPort(m_pSender->getServerPort());
  connect(m_window, &MainWindow::signalClicked, this, &Server::clicked);
  connect(m_pSender.get(), &TCPSenderServer::msgReceived, this, &Server::recieveMessage);
}

Server::~Server()
{

}

void Server::send(std::string msg)
{
  m_pSender->send(msg, 1, std::chrono::seconds(1), false, m_clientIds[0]);
}

void Server::newConnection(int id)
{
  std::cout << "Adding Connection" << std::endl;
  m_clientIds.push_back(id);
  auto t = m_pSender->getSocket(id);
  m_window->addConnection(t->peerAddress(), t->peerPort());
}

void Server::clicked(std::string msg)
{
  msg::MsgToSend* pMsg = new msg::MsgToSend();
  pMsg->mutable_test()->add_teststring(msg);
  std::cout << "Sending" << std::endl;
  for(auto&& id : m_clientIds)
    m_pSender->send(pMsg, 1, std::chrono::seconds(1), false, id);
}

void Server::recieveMessage(msg::MsgToSend* msg, QHostAddress ip, qint16 port)
{
  m_window->receivedMsg(msg->DebugString(), ip, port);
}
