#include "TCPSenderClient.hpp"
#include "Logger/Logger.hpp"
#include "ProtoFiles/MsgToSend.pb.h"

#include <qbytearray.h>

TCPSenderClient::TCPSenderClient(QHostAddress ip, qint16 port)
  : QObject(nullptr), m_connectingSocket(), m_myId(0)
{
  m_connectingSocket = std::make_shared<QTcpSocket>(this);
  connect(m_connectingSocket.get(),
          &QTcpSocket::connected,
          this,
          &TCPSenderClient::connection);
  m_connectingSocket->connectToHost(ip, port);
}

TCPSenderClient::~TCPSenderClient() {}

qint64 TCPSenderClient::send(msg::MsgToSend msg,
                             int convId,
                             std::chrono::seconds timeout,
                             bool requireResponse)
{
  return m_connectingSocket->write(QByteArray(
    msg.SerializeAsString().c_str(), static_cast<int>(msg.SerializeAsString().size())));
}

qint64 TCPSenderClient::send(std::string msg,
                             int convId,
                             std::chrono::seconds timeout,
                             bool requireResponse)
{
  return m_connectingSocket->write(
    QByteArray(msg.c_str(), static_cast<int>(msg.size())));
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

void TCPSenderClient::connection()
{
  connect(m_connectingSocket.get(), &QIODevice::readyRead, this, &TCPSenderClient::emitMessage);
  connect(m_connectingSocket.get(),
          &QTcpSocket::disconnected,
          this,
          &TCPSenderClient::disconnected);
  std::cout << "Connection Successful" << std::endl;
  emit newConnection();
}

void TCPSenderClient::emitMessage()
{
  QTcpSocket* m_connectingSocket = qobject_cast<QTcpSocket*>(sender());
  auto s = m_connectingSocket->readAll();
  std::cout << "Message Received." << std::endl;
  std::cout << s.toStdString() << std::endl;
  emit msgReceived(s.toStdString(),
    m_connectingSocket->peerAddress(),
    m_connectingSocket->peerPort());
}

void TCPSenderClient::readStream()
{
  auto readSocket =
    std::shared_ptr<QTcpSocket>(qobject_cast<QTcpSocket*>(sender()));
  auto s = readSocket->readAll();
  if (s.size() == 4)
  {
    m_myId = s[3];
  }
  emit msgReceived(
    s.toStdString(), readSocket->peerAddress(), readSocket->peerPort());
}

void TCPSenderClient::disconnected()
{
  std::cout << "lost connection" << std::endl;
  emit lostConnection();
}
