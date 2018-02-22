#include "TCPSenderServer.hpp"
#include "Logger/Logger.hpp"
#include "Messages/MakeMsgs.hpp"
#include "Messages/MapHelpers.hpp"
#include "ProtoFiles/MsgToSend.pb.h"

#include <qbytearray.h>
#include <qnetworkinterface.h>

TCPSenderServer::TCPSenderServer()
  : QObject(nullptr),
    m_pServer(new QTcpServer(this)),
    m_pSockets(),
    m_nextId(2),
    m_myId(1),
    m_nextConvId(1)
{
  Logger::info("waiting for connection");
  QList<QHostAddress> allAddresses = QNetworkInterface::allAddresses();
  QHostAddress ipAddress;

  for (auto&& ip : allAddresses)  
  {
    if (ip.protocol() == QAbstractSocket::IPv4Protocol &&
      ip != QHostAddress(QHostAddress::LocalHost))
    {
      if (ip.toString().toStdString().find("169.254.") == std::string::npos)
      {
        std::cout << ip.toString().toStdString() << std::endl;
        ipAddress = ip;
        break;
      }
    }
  }

  if (!m_pServer->listen(ipAddress))
  {
    Logger::info("not listening");
    return;
  }
  connect(m_pServer.get(),
          &QTcpServer::newConnection,
          this,
          &TCPSenderServer::connection);
}

TCPSenderServer::~TCPSenderServer() {}

qint64 TCPSenderServer::send(msg::MsgToSend* pMsg, int endpointId)
{
  Logger::info("Sending: " + pMsg->DebugString());

  auto itr = m_pSockets.find(endpointId);
  if (itr != m_pSockets.end() && itr->second != nullptr)
  {
    return m_pSockets[endpointId]->write(
      QByteArray(pMsg->SerializeAsString().c_str()+'~',
                 static_cast<int>(pMsg->SerializeAsString().size()+1)));
  }
  Logger::error("Not in endpoints");
  return 0;
}

std::string TCPSenderServer::getServerIpAddress()
{
  return m_pServer->serverAddress().toString().toStdString();
}

quint16 TCPSenderServer::getServerPort()
{
  return m_pServer->serverPort();
}

std::shared_ptr<QTcpSocket> TCPSenderServer::getSocket(int id)
{
  return (*m_pSockets.find(id)).second;
}

int TCPSenderServer::getNextConvId()
{
  Logger::info("incrementing from: " + std::to_string(m_nextConvId));
  return m_nextConvId++;
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
  Logger::info("Connection Successful, Id: " + std::to_string(m_nextId));
  // send id
  m_pSockets[m_nextId] = connectingSocket;
  emit newConnection(m_nextId);

  m_nextId++;
}

void TCPSenderServer::emitMessage()
{
  QTcpSocket* readSocket = qobject_cast<QTcpSocket*>(sender());
  msg::MsgToSend* pMsg = new msg::MsgToSend();
  pMsg->ParseFromString(readSocket->readAll().toStdString());
  Logger::info("Received: " + pMsg->DebugString());
  emit msgReceived(pMsg, readSocket->peerAddress(), readSocket->peerPort());
}

void TCPSenderServer::readStream()
{
  auto readSocket =
    std::shared_ptr<QTcpSocket>(qobject_cast<QTcpSocket*>(sender()));
  msg::MsgToSend* pReceived = new msg::MsgToSend();
  pReceived->ParseFromString(readSocket->readAll().toStdString());

  Logger::info("Message Received: " + pReceived->DebugString());
  emit msgReceived(
    pReceived, readSocket->peerAddress(), readSocket->peerPort());
}

void TCPSenderServer::disconnected()
{
  auto readSocket = qobject_cast<QTcpSocket*>(sender());
  if (readSocket == nullptr) return;
  auto itr =
    std::find_if(m_pSockets.begin(),
                 m_pSockets.end(),
                 [readSocket](std::pair<int, std::shared_ptr<QTcpSocket>> rhs) {
                   if (rhs.second == nullptr) return false;
                   return readSocket->localPort() == rhs.second->localPort();
                 });
  if (itr != m_pSockets.end())
  {
    emit lostConnection(itr->first);
    itr->second == nullptr;
  }
}
