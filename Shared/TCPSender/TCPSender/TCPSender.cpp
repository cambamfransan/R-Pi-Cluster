#include "TCPSender.hpp"
#include "Logger/Logger.hpp"
#include "ProtoFiles/MsgToSend.pb.h"

#include <qbytearray.h>

TCPSender::TCPSender(QHostAddress ip, qint16 port)
    : QObject(nullptr), m_pServer(new QTcpServer(this))
{
  m_pServer = new QTcpServer(this);
  connect(m_pServer, &QTcpServer::newConnection, this, &TCPSender::connection);
  if (!m_pServer->listen()) {
    std::cout << "not listening" << std::endl;
  }
  std::cout << "now running on port: " << m_pServer->serverPort() << std::endl;
}

TCPSender::~TCPSender() { delete m_pServer; }

qint64 TCPSender::send(msg::MsgToSend msg, int convId,
                       std::chrono::seconds timeout, bool requireResponse,
                       QHostAddress receiver, quint16 port)
{
  return m_pSocket->write(QByteArray(msg.SerializeAsString().c_str(),
                                     msg.SerializeAsString().size()));
}

qint64 TCPSender::send(std::string msg, int convId,
                       std::chrono::seconds timeout, bool requireResponse,
                       QHostAddress receiver, quint16 port)
{
  return m_pSocket->write(QByteArray(msg.c_str(), msg.size()));
}

void TCPSender::connection()
{
  m_pSocket = m_pServer->nextPendingConnection();
  std::cout << "wrote " << std::to_string(m_pSocket->write("YAY?"))
            << std::endl;
  std::cout << "Connection Successful" << std::endl;
}
