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
  connect(m_pServerSocket.get(),
          &QTcpSocket::connected,
          this,
          &TCPSenderClient::previousConnection);
  m_pServerSocket->connectToHost(ip, port);
  connect(m_pTcpServer.get(),
          &QTcpServer::newConnection,
          this,
          &TCPSenderClient::serverConnection);
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

void TCPSenderClient::previousConnection()
{
  connect(m_pPreviousPriority.get(),
          &QIODevice::readyRead,
          this,
          &TCPSenderClient::emitMessage);
  connect(m_pPreviousPriority.get(),
          &QTcpSocket::disconnected,
          this,
          &TCPSenderClient::disconnected);
  Logger::info("Connection Successful to another client");
}

void TCPSenderClient::serverConnection()
{
  m_pNextPriority =
    std::shared_ptr<QTcpSocket>(m_pTcpServer->nextPendingConnection());

  connect(m_pNextPriority.get(),
          &QIODevice::readyRead,
          this,
          &TCPSenderClient::emitMessage);
  connect(m_pNextPriority.get(),
          &QTcpSocket::disconnected,
          this,
          &TCPSenderClient::disconnected);
  Logger::info("Connection Successful, connected to next client");
}

void TCPSenderClient::newPreviousConnection()
{
  m_pNextPriority =
    std::shared_ptr<QTcpSocket>(m_pTcpServer->nextPendingConnection());

  connect(m_pNextPriority.get(),
          &QIODevice::readyRead,
          this,
          &TCPSenderClient::emitMessage);
  connect(m_pNextPriority.get(),
          &QTcpSocket::disconnected,
          this,
          &TCPSenderClient::disconnected);
  Logger::info("Connection Successful, connected to next client");
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

void TCPSenderClient::connectPrevious(std::string ip, qint16 port)
{
  m_pPreviousPriority->connectToHost(
    QHostAddress(QString::fromStdString(ip)), port);
  if (m_pTcpServer->listen())
  {
    Logger::info("Listening for next client");
    connect(m_pNextPriority.get(),
            &QTcpSocket::connected,
            this,
            &TCPSenderClient::connectNext);
    return;
  }
}

void TCPSenderClient::topPriority()
{
  m_pPreviousPriority = m_pServerSocket;
}

void TCPSenderClient::connectNext()
{
  m_pNextPriority =
    std::shared_ptr<QTcpSocket>(m_pTcpServer->nextPendingConnection());

  connect(m_pNextPriority.get(),
          &QIODevice::readyRead,
          this,
          &TCPSenderClient::emitMessage);
  connect(m_pNextPriority.get(),
          &QTcpSocket::disconnected,
          this,
          &TCPSenderClient::disconnected);
  Logger::info("Connection Successful, connected to next client");
}
