#include "TCPSenderClient.hpp"
#include "Logger/Logger.hpp"
#include "Messages/MakeMsgs.hpp"
#include "ProtoFiles/MsgToSend.pb.h"
#include "Messages/MapHelpers.hpp"

#include <qbytearray.h>

TCPSenderClient::TCPSenderClient(QHostAddress ip, qint16 port)
  : QObject(nullptr),
    m_connectingSocket(),
    m_myId(0),
    m_nextConvId(1),
    m_serverId(1)
{
  m_connectingSocket = std::make_shared<QTcpSocket>(this);
  connect(m_connectingSocket.get(),
          &QTcpSocket::connected,
          this,
          &TCPSenderClient::connection);
  m_connectingSocket->connectToHost(ip, port);
}

TCPSenderClient::~TCPSenderClient() {}

qint64 TCPSenderClient::send(msg::MsgToSend* pMsg)
{
  Logger::info("Sending: " + pMsg->DebugString());
  auto str = pMsg->SerializeAsString();
  return m_connectingSocket->write(
    QByteArray(str.c_str(),
               static_cast<int>(str.size())));
}

quint16 TCPSenderClient::getLocalPort()
{
  return m_connectingSocket->localPort();
}

quint16 TCPSenderClient::getPeerPort()
{
  return m_connectingSocket->peerPort();
}

QHostAddress TCPSenderClient::getPeerAddress()
{
  return m_connectingSocket->peerAddress();
}

int TCPSenderClient::getNextConvId()
{
  return m_nextConvId++;
}

void TCPSenderClient::connection()
{
  connect(m_connectingSocket.get(),
          &QIODevice::readyRead,
          this,
          &TCPSenderClient::emitMessage);
  connect(m_connectingSocket.get(),
          &QTcpSocket::disconnected,
          this,
          &TCPSenderClient::disconnected);
  Logger::info("Connection Successful");
  emit newConnection();
}

void TCPSenderClient::emitMessage()
{
  QTcpSocket* m_connectingSocket = qobject_cast<QTcpSocket*>(sender());
  msg::MsgToSend* pMsg = new msg::MsgToSend();
  pMsg->ParseFromString(m_connectingSocket->readAll().toStdString());
  Logger::info("Message Received: " + pMsg->DebugString());
  emit msgReceived(
    pMsg, m_connectingSocket->peerAddress(), m_connectingSocket->peerPort());
}

void TCPSenderClient::readStream()
{
  auto readSocket =
    std::shared_ptr<QTcpSocket>(qobject_cast<QTcpSocket*>(sender()));
  msg::MsgToSend* pReceived = new msg::MsgToSend();
  pReceived->ParseFromString(readSocket->readAll().toStdString());
  Logger::info("Message Received: " + pReceived->DebugString());
  emit msgReceived(pReceived, readSocket->peerAddress(), readSocket->peerPort());
}

void TCPSenderClient::disconnected()
{
  Logger::error("lost connection");
  emit lostConnection();
}
