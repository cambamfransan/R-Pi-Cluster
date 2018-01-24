#include "TCPSenderClient.hpp"
#include "Logger/Logger.hpp"
#include "MakeMsgs.hpp"
#include "ProtoFiles/MsgToSend.pb.h"
#include "TCPHelpers.hpp"

#include <qbytearray.h>

TCPSenderClient::TCPSenderClient(QHostAddress ip, qint16 port)
  : QObject(nullptr),
    m_connectingSocket(),
    m_myId(0),
    m_nextConvId(1),
    m_serverId(1),
    m_outMessages(),
    m_inputMessages()
{
  m_connectingSocket = std::make_shared<QTcpSocket>(this);
  connect(m_connectingSocket.get(),
          &QTcpSocket::connected,
          this,
          &TCPSenderClient::connection);
  m_connectingSocket->connectToHost(ip, port);
}

TCPSenderClient::~TCPSenderClient() {}

qint64 TCPSenderClient::send(msg::MsgToSend* pMsg,
                             int convId,
                             std::chrono::seconds timeout,
                             bool requireResponse)
{
  Logger::info("Sending: " + pMsg->DebugString());
  if (requireResponse)
  {
    m_outMessages[tcp::getMapId(m_myId, pMsg->basicmsg().convid())] =
      Conversation{
        pMsg, convId, timeout, std::chrono::steady_clock::now(), m_serverId};
  }

  return m_connectingSocket->write(
    QByteArray(pMsg->SerializeAsString().c_str(),
               static_cast<int>(pMsg->SerializeAsString().size())));
}

#ifdef TESTING
qint64 TCPSenderClient::send(std::string msg,
                             int convId,
                             std::chrono::seconds timeout,
                             bool requireResponse)
{
  Logger::info("Sending: " + msg);
  make_msgs::makeTestMsg(m_myId, endpointId, convId, msg);
  m_pSocket->write(
    QByteArray(msg.c_str(), static_cast<int>(msg.size())));
}
#endif

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
  int id = tcp::getMapId(
    pReceived->basicmsg().fromid(), pReceived->basicmsg().convid());
  if (m_inputMessages.find(id) == m_inputMessages.end())
    m_inputMessages[id] = std::chrono::steady_clock::now();
  else
    return;
  if (auto itr = m_outMessages.find(tcp::getMapId(
    pReceived->basicmsg().toid(),
    pReceived->basicmsg().convid())) == m_outMessages.end())
    m_outMessages.erase(itr);
  Logger::info("Message Received: " + pReceived->DebugString());
  emit msgReceived(pReceived, readSocket->peerAddress(), readSocket->peerPort());
}

void TCPSenderClient::disconnected()
{
  // Do something with this...
  Logger::error("lost connection");
  emit lostConnection();
}
