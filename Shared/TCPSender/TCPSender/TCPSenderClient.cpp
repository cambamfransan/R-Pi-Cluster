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
    m_pTcpServer(std::make_shared<QTcpServer>(this)),
    m_pPreviousPriority(std::make_shared<QTcpSocket>(this)),
    m_pNextPriority(std::make_shared<QTcpSocket>(this)),
    m_myId(0),
    m_nextConvId(1),
    m_serverId(1),
    m_myAddress()
{
  connect(m_pServerSocket.get(),
          &QTcpSocket::connected,
          this,
          &TCPSenderClient::connection);
  m_pServerSocket->connectToHost(ip, port);

  QList<QHostAddress> allAddresses = QNetworkInterface::allAddresses();

  for (auto&& ip : allAddresses)
  {
    if (ip.protocol() == QAbstractSocket::IPv4Protocol &&
        ip != QHostAddress(QHostAddress::LocalHost))
    {
      if (ip.toString().toStdString().find("169.254.") == std::string::npos)
      {
        std::cout << ip.toString().toStdString() << std::endl;
        m_myAddress = ip;
        break;
      }
    }
  }
}

TCPSenderClient::~TCPSenderClient() {}

qint64 TCPSenderClient::send(msg::MsgToSend* pMsg)
{
  Logger::info("Sending: " + pMsg->DebugString());
  if (m_pTcpServer->isListening())
    Logger::info("Still Listening on: " +
                 m_pTcpServer->serverAddress().toString().toStdString() + ":" +
                 QString::number(m_pTcpServer->serverPort()).toStdString());
  else
    Logger::info("stopped listening");
  if (m_pNextPriority->isValid())
    Logger::info("Still open");
  else
    Logger::info("not open");

  auto str = pMsg->SerializeAsString();
  return m_pServerSocket->write(
    QByteArray(str.c_str(), static_cast<int>(str.size())));
}

qint64 TCPSenderClient::sendToNext(msg::MsgToSend* pMsg)
{
  Logger::info("Sending: " + pMsg->DebugString());
  auto str = pMsg->SerializeAsString();
  return m_pNextPriority->write(
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
  Logger::info("incrementing from: " + std::to_string(m_nextConvId));
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

void TCPSenderClient::newNextConnection()
{
  std::cout << "YAY!!!!!" << std::endl;
  m_pNextPriority =
    std::shared_ptr<QTcpSocket>(m_pTcpServer->nextPendingConnection());
  //connect(m_pNextPriority.get(),
  //        &QTcpSocket::disconnected,
  //        this,
  //        &TCPSenderClient::disconnected);
  connect(m_pNextPriority.get(),
          &QIODevice::readyRead,
          this,
          &TCPSenderClient::emitMessage);
  Logger::info("Connection Successful, connected to next client");
}

void TCPSenderClient::newPreviousConnection()
{
  m_pPreviousPriority =
    std::shared_ptr<QTcpSocket>(qobject_cast<QTcpSocket*>(sender()));
  connect(m_pPreviousPriority.get(),
          &QIODevice::readyRead,
          this,
          &TCPSenderClient::emitMessage);
  connect(m_pPreviousPriority.get(),
          &QTcpSocket::disconnected,
          this,
          &TCPSenderClient::disconnected);
  Logger::info("Connection Successful, connected to prev client");
}

void TCPSenderClient::emitMessage()
{
  QTcpSocket* m_pServerSocket = qobject_cast<QTcpSocket*>(sender());
  auto msg = m_pServerSocket->readAll().toStdString();
  while (auto pos = msg.find('~') != std::string::npos)
  {
  msg::MsgToSend* pMsg = new msg::MsgToSend();
  pMsg->ParseFromString(msg.substr(0, pos));
  msg.erase(0, pos);
  Logger::info("Message Received: " + pMsg->DebugString());
  emit msgReceived(
    pMsg, m_pServerSocket->peerAddress(), m_pServerSocket->peerPort());
  }
  msg::MsgToSend* pMsg = new msg::MsgToSend();
  pMsg->ParseFromString(msg);
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

void TCPSenderClient::connectPrevious(std::string ip, quint16 port)
{
  auto i = QHostAddress(ip.c_str());
  qInfo() << i << port;
  m_pPreviousPriority->connectToHost(QHostAddress(ip.c_str()), port);
  connect(m_pPreviousPriority.get(),
          &QTcpSocket::connected,
          this,
          &TCPSenderClient::newPreviousConnection);
  Logger::info("ConnectPrevious: Ip: " + ip +
               " Port: " + QString::number(port).toStdString());
  connect(m_pTcpServer.get(),
          &QTcpServer::newConnection,
          this,
          &TCPSenderClient::newNextConnection);

  if (m_pTcpServer->listen(m_myAddress))
  {
    Logger::info("Listening for next client");
    return;
  }
}

void TCPSenderClient::topPriority()
{
  m_pPreviousPriority = m_pServerSocket;
  if (m_pTcpServer->listen(m_myAddress))
  {
    connect(m_pTcpServer.get(),
            &QTcpServer::newConnection,
            this,
            &TCPSenderClient::newNextConnection);
    Logger::info("Listening for next client");
    return;
  }
}

int TCPSenderClient::getLocalServerPort()
{
  return static_cast<int>(m_pTcpServer->serverPort());
}
