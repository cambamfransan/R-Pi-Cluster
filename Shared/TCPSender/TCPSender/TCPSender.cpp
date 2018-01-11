#include "TCPSender.hpp"
#include "Logger/Logger.hpp"
#include "ProtoFiles/MsgToSend.pb.h"

#include <qbytearray.h>

TCPSender::TCPSender(QHostAddress ip, qint16 port)
  : QObject(nullptr), m_pServer(new QTcpServer(this))
{
  std::cout << "Constructing" << std::endl;
  if (ip == QHostAddress("0.0.0.0"))
  {
    std::cout << "waiting for connection" << std::endl;
    connect(m_pServer, &QTcpServer::newConnection, this, &TCPSender::connection);
    if (!m_pServer->listen())
    {
      std::cout << "not listening" << std::endl;
      return;
    }
    std::cout << "now running on port: " << m_pServer->serverPort()
              << std::endl;
  }
  else
  {
    std::cout << "Waiting to connect" << std::endl;
    m_pSocket = new QTcpSocket(this);
    m_pSocket->connectToHost(ip, port);
  }
  connect(m_pSocket, &QIODevice::readyRead, this, &TCPSender::readStream);
}

TCPSender::~TCPSender()
{
  delete m_pServer;
}

qint64 TCPSender::send(msg::MsgToSend msg,
                       int convId,
                       std::chrono::seconds timeout,
                       bool requireResponse,
                       QHostAddress receiver,
                       quint16 port)
{
  return m_pSocket->write(QByteArray(
    msg.SerializeAsString().c_str(), msg.SerializeAsString().size()));
}

qint64 TCPSender::send(std::string msg,
                       int convId,
                       std::chrono::seconds timeout,
                       bool requireResponse,
                       QHostAddress receiver,
                       quint16 port)
{
  return m_pSocket->write(QByteArray(msg.c_str(), msg.size()));
}

void TCPSender::connection()
{
  std::cout << "Connection Successful" << std::endl;
  m_pSocket = m_pServer->nextPendingConnection();
  std::cout << "wrote " << std::to_string(m_pSocket->write("YAY?"))
           << std::endl;
}

void TCPSender::readStream()
{
  auto s = m_pSocket->readAll();
  std::cout << "This is one step closer." << std::endl;
  std::cout << s.toStdString() << std::endl;
}
