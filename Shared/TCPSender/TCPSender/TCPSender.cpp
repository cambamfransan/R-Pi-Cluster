#include "TCPSender.hpp"
#include "Logger/Logger.hpp"
#include "ProtoFiles/MsgToSend.pb.h"

#include <qbytearray.h>
#include <qtimer.h>

namespace
{
  inline std::string qHostportToString(QHostAddress ip, qint16 port)
  {
    auto a = ip.toString().toStdString();
    auto b = std::to_string(port);
    std::cout << a << std::endl;
    std::cout << b << std::endl;

    return ip.toString().toStdString() + std::to_string(port);
  }
} // namespace

TCPSender::TCPSender(QHostAddress ip, qint16 port)
  : QObject(nullptr),
    m_pServer(new QTcpServer(this)),
    m_pSockets(),
    m_pTimer(std::make_shared<QTimer>(this))
{
  connect(m_pTimer.get(), SIGNAL(timeout()), this, SLOT(update()));
  if (ip == QHostAddress("0.0.0.0"))
  {
    std::cout << "waiting for connection" << std::endl;
    if (!m_pServer->listen())
    {
      std::cout << "not listening" << std::endl;
      return;
    }
    std::cout << "now running on port: " << m_pServer->serverPort()
              << std::endl;
    connect(m_pServer.get(),
            &QTcpServer::newConnection,
            this,
            &TCPSender::connection);
  }
  else
  {
    m_connectingSocket = std::make_shared<QTcpSocket>(this);
    connect(m_connectingSocket.get(),
            &QTcpSocket::connected,
            this,
            &TCPSender::connection);
    m_connectingSocket->connectToHost(ip, port);
    m_pTimer->start(1000);
  }
}

TCPSender::~TCPSender() {}

qint64 TCPSender::send(msg::MsgToSend msg,
                       int convId,
                       std::chrono::seconds timeout,
                       bool requireResponse,
                       QHostAddress receiver,
                       quint16 port)
{
  auto t = m_pSockets.find(qHostportToString(receiver, port));
  if (t != m_pSockets.end())
    return t->second->write(QByteArray(
      msg.SerializeAsString().c_str(), msg.SerializeAsString().size()));
}

qint64 TCPSender::send(std::string msg,
                       int convId,
                       std::chrono::seconds timeout,
                       bool requireResponse,
                       QHostAddress receiver,
                       quint16 port)
{
  auto t = m_pSockets.find(qHostportToString(receiver, port));
  if (t != m_pSockets.end())
    return t->second->write(QByteArray(msg.c_str(), msg.size()));
}

void TCPSender::connection()
{
  if (m_connectingSocket == nullptr)
    m_connectingSocket =
      std::shared_ptr<QTcpSocket>(m_pServer->nextPendingConnection());
  connect(m_connectingSocket.get(), &QIODevice::readyRead, this, [this]() {
    if (m_pTimer->isActive()) m_pTimer->start(1000);
    auto s = m_connectingSocket->readAll();
    std::cout << "This is one step closer." << std::endl;
    std::cout << s.toStdString() << std::endl;
    emit msgReceived(s.toStdString(),
                     m_connectingSocket->peerAddress(),
                     m_connectingSocket->peerPort());
  });
  //          &TCPSender::readStream);
  connect(m_connectingSocket.get(),
          &QTcpSocket::disconnected,
          this,
          &TCPSender::disconnected);
  std::cout << "Connection Successful" << std::endl;
  int i = m_connectingSocket->write("YAY?");
  std::cout << "wrote " << std::to_string(i) << std::endl;
  m_pSockets[qHostportToString(
    m_connectingSocket->peerAddress(), m_connectingSocket->peerPort())] =
    m_connectingSocket;
}

void TCPSender::readStream()
{
  if (m_pTimer->isActive()) m_pTimer->start(1000);
  // auto s = m_pSocket->readAll();
  // std::cout << "This is one step closer." << std::endl;
  // std::cout << s.toStdString() << std::endl;
  // emit msgReceived(
  //  s.toStdString(), m_pSocket->peerAddress(), m_pSocket->peerPort());
}

void TCPSender::disconnected()
{
  std::cout << "lost connection" << std::endl;
  emit lostConnection();
}

void TCPSender::update()
{
  send("Another one bytes the dust",
       1,
       std::chrono::seconds(1),
       false,
       QHostAddress("127.0.0.1"),
       0);
}
