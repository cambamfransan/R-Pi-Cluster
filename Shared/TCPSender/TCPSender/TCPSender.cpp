#include "TCPSender.hpp"
#include "Logger/Logger.hpp"
#include "ProtoFiles/MsgToSend.pb.h"

#include <qbytearray.h>

TCPSender::TCPSender(QHostAddress ip, qint16 port)
  : QObject(nullptr),
    m_pServer(new QTcpServer(this)),
    m_pSockets(),
    m_nextId(1),
    m_myId(0)
{
  if (ip == QHostAddress("0.0.0.0"))
  {
    std::cout << "waiting for connection" << std::endl;
    if (!m_pServer->listen())
    {
      std::cout << "not listening" << std::endl;
      return;
    }
    connect(m_pServer.get(),
            &QTcpServer::newConnection,
            this,
            &TCPSender::connection);
    m_myId = m_nextId++;
  }
  else
  {
    m_connectingSocket = std::make_shared<QTcpSocket>(this);
    connect(m_connectingSocket.get(),
            &QTcpSocket::connected,
            this,
            &TCPSender::connection);
    m_connectingSocket->connectToHost(ip, port);
  }
}

TCPSender::~TCPSender() {}

qint64 TCPSender::send(msg::MsgToSend msg,
                       int convId,
                       std::chrono::seconds timeout,
                       bool requireResponse,
                       int endpointId)
{
  auto t = m_pSockets.find(endpointId);
  if (t != m_pSockets.end())
    return t->second->write(QByteArray(
      msg.SerializeAsString().c_str(), msg.SerializeAsString().size()));
  else
  {
    std::cout << "Endpoint Does not exist" << std::endl;
    return 0;
  }
}

qint64 TCPSender::send(std::string msg,
                       int convId,
                       std::chrono::seconds timeout,
                       bool requireResponse,
                       int endpointId)
{
  auto t = m_pSockets.find(endpointId);
  if (t != m_pSockets.end())
    return t->second->write(QByteArray(msg.c_str(), static_cast<int>(msg.size())));
  else
  {
    std::cout << "Endpoint Does not exist" << std::endl;
    return 0;
  }
}

quint16 TCPSender::getServerPort()
{
  return m_pServer->serverPort();
}

void TCPSender::connection()
{
  if (m_connectingSocket == nullptr)
  {
    m_connectingSocket =
      std::shared_ptr<QTcpSocket>(m_pServer->nextPendingConnection());
  }
  connect(m_connectingSocket.get(), &QIODevice::readyRead, this, [this]() {
    auto s = m_connectingSocket->readAll();
    std::cout << "This is one step closer." << std::endl;
    std::cout << s.toStdString() << std::endl;
    emit msgReceived(s.toStdString(),
                     m_connectingSocket->peerAddress(),
                     m_connectingSocket->peerPort());
  });
  connect(m_connectingSocket.get(),
          &QTcpSocket::disconnected,
          this,
          &TCPSender::disconnected);
  std::cout << "Connection Successful" << std::endl;
  // send id
  int next;
  if (m_myId != 0)
  {
    next = m_nextId++;
    int i = m_connectingSocket->write("Id:" + next);
    std::cout << "wrote " << std::to_string(i) << std::endl;
    std::cout << "Adding to TCPSender" << std::endl;
    m_pSockets[i] = m_connectingSocket;
    emit newConnection(m_myId);
  }
  else
    std::cout << "Nope..." << std::endl;

}

void TCPSender::readStream()
{
  auto readSocket =
    std::shared_ptr<QTcpSocket>(qobject_cast<QTcpSocket*>(sender()));
  auto s = readSocket->readAll();
  if (s.size() == 4)
  {
    m_myId = s[3];
    m_pSockets[m_myId] = readSocket;
    emit newConnection(m_myId);
  }
  std::cout << "This is one step closer." << std::endl;
  std::cout << s.toStdString() << std::endl;
  emit msgReceived(
    s.toStdString(), readSocket->peerAddress(), readSocket->peerPort());
}

void TCPSender::disconnected()
{
  std::cout << "lost connection" << std::endl;
  emit lostConnection();
}
