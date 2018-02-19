#include "TCPSenderClient.hpp"
#include "Logger/Logger.hpp"
#include "Messages/MakeMsgs.hpp"
#include "Messages/MapHelpers.hpp"
#include "ProtoFiles/MsgToSend.pb.h"

#include <qbytearray.h>

TCPSenderClient::TCPSenderClient(QHostAddress ip, qint16 port)
  : QObject(nullptr),
    m_pServerSocket(std::make_shared<QTcpSocket>(this)),
    m_pTcpServer(std::make_shared<QTcpServer>(this)),
    m_pPreviousPriority(std::make_shared<QTcpSocket>(this)),
    m_pNextPriority(std::make_shared<QTcpSocket>(this)),
    m_myId(0),
    m_nextConvId(1),
    m_serverId(1)
{
  connect(m_pServerSocket.get(),
          &QTcpSocket::connected,
          this,
          &TCPSenderClient::connection);
  m_pServerSocket->connectToHost(ip, port);
}

TCPSenderClient::~TCPSenderClient() {}

qint64 TCPSenderClient::send(msg::MsgToSend* pMsg)
{
  Logger::info("Sending: " + pMsg->DebugString());
  auto str = pMsg->SerializeAsString();
  return m_pServerSocket->write(
    QByteArray(str.c_str(), static_cast<int>(str.size())));
}

quint16 TCPSenderClient::getLocalPort()
{
  return m_pServerSocket->localPort();
}

quint16 TCPSenderClient::getPeerPort()
{
  return m_pServerSocket->peerPort();
}

QHostAddress TCPSenderClient::getPeerAddress()
{
  return m_pServerSocket->peerAddress();
}

int TCPSenderClient::getNextConvId()
{
  return m_nextConvId++;
}

void TCPSenderClient::connection()
{
  connect(m_pServerSocket.get(),
          &QIODevice::readyRead,
          this,
          &TCPSenderClient::emitMessage);
  connect(m_pServerSocket.get(),
          &QTcpSocket::disconnected,
          this,
          &TCPSenderClient::disconnected);
  Logger::info("Connection Successful");
  emit newConnection();
}

void TCPSenderClient::emitMessage()
{
  QTcpSocket* m_pServerSocket = qobject_cast<QTcpSocket*>(sender());
  msg::MsgToSend* pMsg = new msg::MsgToSend();
  pMsg->ParseFromString(m_pServerSocket->readAll().toStdString());
  Logger::info("Message Received: " + pMsg->DebugString());
  emit msgReceived(
    pMsg, m_pServerSocket->peerAddress(), m_pServerSocket->peerPort());
}

void TCPSenderClient::readStream()
{
  auto readSocket =
    std::shared_ptr<QTcpSocket>(qobject_cast<QTcpSocket*>(sender()));
  msg::MsgToSend* pReceived = new msg::MsgToSend();
  pReceived->ParseFromString(readSocket->readAll().toStdString());
  Logger::info("Message Received: " + pReceived->DebugString());
  emit msgReceived(
    pReceived, readSocket->peerAddress(), readSocket->peerPort());
}

void TCPSenderClient::disconnected()
{
  Logger::error("lost connection");
  emit lostConnection();
}

bool TCPSenderClient::connectPrevious(QHostAddress ip, qint16 port)
{
  m_pPreviousPriority->connectToHost(ip, port);
}
