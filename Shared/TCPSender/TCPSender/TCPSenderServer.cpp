#include "TCPSenderServer.hpp"
#include "Logger/Logger.hpp"
#include "MakeMsgs.hpp"
#include "ProtoFiles/MsgToSend.pb.h"
#include "TCPHelpers.hpp"

#include <qbytearray.h>

TCPSenderServer::TCPSenderServer()
  : QObject(nullptr),
    m_pServer(new QTcpServer(this)),
    m_pSockets(),
    m_nextId(2),
    m_myId(1),
    m_outMessages(),
    m_nextConvId(1),
    m_inputMessages()
{
  Logger::info("waiting for connection");
  if (!m_pServer->listen())
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

qint64 TCPSenderServer::send(msg::MsgToSend* pMsg,
                             int convId,
                             std::chrono::seconds timeout,
                             bool requireResponse,
                             int endpointId)
{
  Logger::info("Sending: " + pMsg->DebugString());
  if (requireResponse)
  {
    m_outMessages[tcp::getMapId(m_myId, pMsg->basicmsg().convid())] =
      Conversation{
        pMsg, convId, timeout, std::chrono::steady_clock::now(), endpointId};
  }

  return m_pSockets[endpointId]->write(
    QByteArray(pMsg->SerializeAsString().c_str(),
               static_cast<int>(pMsg->SerializeAsString().size())));
}

#ifdef TESTING
qint64 TCPSenderServer::send(std::string msg,
                             int convId,
                             std::chrono::seconds timeout,
                             bool requireResponse,
                             int endpointId)
{
  auto pMsg = make_msgs::makeTestMsg(m_myId, endpointId, convId, msg);
  Logger::info("Sending: " + pMsg->DebugString());
  return m_pSockets[endpointId]->write(
    QByteArray(pMsg->SerializeAsString().c_str(), static_cast<int>(pMsg->SerializeAsString().size())));
}
#endif

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
  msg::MsgToSend* pMsg = make_msgs::makeBasicMsgToSend(
    m_myId, m_nextId, msg::ProtoType::ID_MSG, m_nextConvId++);
  Logger::info("Send: " + pMsg->DebugString());
  connectingSocket->write(QByteArray::fromStdString(pMsg->SerializeAsString()));
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
  emit msgReceived(
    pReceived, readSocket->peerAddress(), readSocket->peerPort());
}

void TCPSenderServer::disconnected()
{
  auto readSocket =
    std::shared_ptr<QTcpSocket>(qobject_cast<QTcpSocket*>(sender()));
  //Todo
//  m_pSockets.erase(m_pSockets.begin(), m_pSockets.end(), readSocket));
  Logger::error("lost connection");
  emit lostConnection();
}
