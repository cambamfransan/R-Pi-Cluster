#include "TCPSenderClient.hpp"
#include "Logger/Logger.hpp"
#include "Messages/MakeMsgs.hpp"
#include "Messages/MapHelpers.hpp"
#include "ProtoFiles/MsgToSend.pb.h"
#include <qnetworkinterface.h>

#include <qbytearray.h>

TCPSenderClient::TCPSenderClient(QHostAddress ip, qint16 port)
  : QObject(nullptr),
    m_pServerSocket(std::make_shared<QTcpSocket>(this)),
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
  m_pServerSocket->write(QByteArray(std::to_string(str.size()).c_str(), 20));
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
//  Logger::info("incrementing from: " + std::to_string(m_nextConvId));
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
  while (true)
  {
    auto size = m_pServerSocket->read(20).toInt();
    if (size == 0)
    {
      Logger::error("Size was 0");
      break;
    }
    auto msg = m_pServerSocket->read(size).toStdString();
    msg::MsgToSend* pMsg = new msg::MsgToSend();
    pMsg->ParseFromString(msg);
    Logger::info("Message Received: " + pMsg->DebugString());
    emit msgReceived(
      pMsg, m_pServerSocket->peerAddress(), m_pServerSocket->peerPort());
    if (!m_pServerSocket->bytesAvailable()) break;
  }
}

void TCPSenderClient::disconnected()
{
  Logger::error("lost connection");
  emit lostConnection();
}
