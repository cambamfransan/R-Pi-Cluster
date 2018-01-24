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
}

void Client::recieveMessage(std::string msg, QHostAddress ip, qint16 port)
{
  m_window->receivedMsg(msg, ip, port);
}

void Client::clicked(std::string msg)
{
  std::cout << "Sending" << std::endl;
  m_pSender->send(msg, 1, std::chrono::seconds(1), false);
}
