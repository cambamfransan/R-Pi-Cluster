#include "TCPSender.hpp"
#include "Logger/Logger.hpp"
#include "ProtoFiles/MsgToSend.pb.h"

#include <qbytearray.h>
#include <qtimer.h>

TCPSender::TCPSender(QHostAddress ip, qint16 port)
  : QObject(nullptr), m_pServer(new QTcpServer(this)), m_pSocket(nullptr), m_pTimer(std::make_shared<QTimer>(this))
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
  connect(m_pServer.get(), &QTcpServer::newConnection, this, &TCPSender::connection);
  }
  else
  {
    std::cout << "Waiting to connect" << std::endl;
    m_pSocket = std::make_shared<QTcpSocket>(this);
    connect(m_pSocket.get(), &QTcpSocket::connected, this, &TCPSender::connection);
    m_pSocket->connectToHost(ip, port);
    m_pTimer->start(1000);
  }
}

TCPSender::~TCPSender()
{
}

qint64 TCPSender::send(msg::MsgToSend msg,
                       int convId,
                       std::chrono::seconds timeout,
                       bool requireResponse,
                       QHostAddress receiver,
                       quint16 port)
{
  return m_pSocket->write(QByteArray(
    msg.SerializeAsString().c_str(), msg.SerializeAsString().size()));
}

qint64 TCPSender::send(std::string msg,
                       int convId,
                       std::chrono::seconds timeout,
                       bool requireResponse,
                       QHostAddress receiver,
                       quint16 port)
{
  return m_pSocket->write(QByteArray(msg.c_str(), msg.size()));
}

void TCPSender::connection()
{
  if (m_pSocket == nullptr) m_pSocket = std::shared_ptr<QTcpSocket>(m_pServer->nextPendingConnection());
  connect(m_pSocket.get(), &QIODevice::readyRead, this, &TCPSender::readStream);
  connect(m_pSocket.get(), &QTcpSocket::disconnected, this, &TCPSender::disconnected);
  std::cout << "Connection Successful" << std::endl;
  int i = m_pSocket->write("YAY?");
  std::cout << "wrote " << std::to_string(i)
            << std::endl;
}

void TCPSender::readStream()
{
  m_pTimer->start(1000);
  auto s = m_pSocket->readAll();
  std::cout << "This is one step closer." << std::endl;
  std::cout << s.toStdString() << std::endl;
  emit msgReceived(s.toStdString(), m_pSocket->peerAddress(), m_pSocket->peerPort());
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
