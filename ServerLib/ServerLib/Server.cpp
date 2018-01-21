#include "Server.hpp"

Server::Server()
  : m_pSender(std::make_shared<TCPSender>(QHostAddress("0.0.0.0"))), m_window(new MainWindow())
{
  m_window->show();
  connect(m_pSender.get(), &TCPSender::newConnection, this, &Server::newConnection);
  m_window->setPort(m_pSender->getServerPort());
  connect(m_window, &MainWindow::signalClicked, this, &Server::clicked);
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
  for(auto&& id : m_clientIds)
    m_pSender->send(msg, 1, std::chrono::seconds(1), false, id);
}
