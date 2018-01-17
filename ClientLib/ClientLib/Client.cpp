#include "Client.hpp"

Client::Client(QHostAddress addr, qint16 port)
  : m_sender(addr, port)
{

}

Client::~Client()
{

}