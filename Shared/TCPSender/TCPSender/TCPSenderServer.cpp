#include "TCPSenderServer.hpp"
#include "Logger/Logger.hpp"
#include "ProtoFiles/MsgToSend.pb.h"

#include <qbytearray.h>

TCPSenderServer::TCPSenderServer()
  : QObject(nullptr),
    m_pServer(new QTcpServer(this)),
    m_pSockets(),
    m_nextId(2),
    m_myId(1)
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
          &TCPSenderServer::connection);
}

TCPSenderServer::~TCPSenderServer() {}

qint64 TCPSenderServer::send(msg::MsgToSend msg,
                             int convId,
                             std::chrono::seconds timeout,
                             bool requireResponse,
                             int endpointId)
{
  // In the future can remove this
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

qint64 TCPSenderServer::send(std::string msg,
                             int convId,
                             std::chrono::seconds timeout,
                             bool requireResponse,
                             int endpointId)
{
  // In the future can remove this
  auto t = m_pSockets.find(endpointId);
  if (t != m_pSockets.end())
    return t->second->write(
      QByteArray(msg.c_str(), static_cast<int>(msg.size())));
  else
  {
    std::cout << "Endpoint Does not exist" << std::endl;
    return 0;
  }
}

quint16 TCPSenderServer::getServerPort()
{
  return m_pServer->serverPort();
}

std::shared_ptr<QTcpSocket> TCPSenderServer::getSocket(int id)
{ 
  return (*m_pSockets.find(id)).second; 
}

void TCPSenderServer::connection()
{
  auto connectingSocket =
    std::shared_ptr<QTcpSocket>(m_pServer->nextPendingConnection());

  connect(connectingSocket.get(),
          &QIODevice::readyRead,
          this,
          &TCPSenderServer::emitMessage);
  connect(connectingSocket.get(),
          &QTcpSocket::disconnected,
          this,
          &TCPSenderServer::disconnected);
  std::cout << "Connection Successful" << std::endl;
  // send id
  std::cout << "Id: " << m_nextId << std::endl;
  int i = connectingSocket->write("Id:" + QByteArray::number(m_nextId));
  std::cout << "wrote " << std::to_string(i) << std::endl;
  std::cout << "Adding to TCPSenderServer" << std::endl;
  m_pSockets[m_nextId] = connectingSocket;
  emit newConnection(m_nextId);

  m_nextId++;
}

void TCPSenderServer::emitMessage()
{
  QTcpSocket* readSocket = qobject_cast<QTcpSocket*>(sender());
  auto s = readSocket->readAll();
  std::cout << "This is one step closer." << std::endl;
  std::cout << s.toStdString() << std::endl;
  emit msgReceived(
    s.toStdString(), readSocket->peerAddress(), readSocket->peerPort());
}

void TCPSenderServer::readStream()
{
  auto readSocket =
    std::shared_ptr<QTcpSocket>(qobject_cast<QTcpSocket*>(sender()));
  auto s = readSocket->readAll();
  std::cout << "Message Received." << std::endl;
  std::cout << s.toStdString() << std::endl;
  emit msgReceived(
    s.toStdString(), readSocket->peerAddress(), readSocket->peerPort());
}

void TCPSenderServer::disconnected()
{
  // Remove from list
  std::cout << "lost connection" << std::endl;
  emit lostConnection();
}
