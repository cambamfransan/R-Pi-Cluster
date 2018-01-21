#include "Client.hpp"

Client::Client(QHostAddress addr, qint16 port)
  : m_pSender(std::make_shared<TCPSender>(addr, port)), m_serverId(0)
{
  connect(m_pSender.get(), &TCPSender::newConnection, this, &Client::newConnection);
}

Client::~Client()
{

}

void Client::newConnection(int id)
{
  m_serverId = id;
}
