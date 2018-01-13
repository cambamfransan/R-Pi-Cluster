#include "TCPSender.hpp"
#include "Logger/Logger.hpp"
#include "ProtoFiles/MsgToSend.pb.h"

#include <qbytearray.h>
#include <qtimer.h>

TCPSender::TCPSender(QHostAddress ip, qint16 port)
  : QObject(nullptr), m_pServer(new QTcpServer(this)), m_pSocket(nullptr), connected(false)
{
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
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
  connect(m_pServer, &QTcpServer::newConnection, this, &TCPSender::connection);
  }
  else
  {
    std::cout << "Waiting to connect" << std::endl;
    m_pSocket = new QTcpSocket(this);
    connect(m_pSocket, &QTcpSocket::connected, this, &TCPSender::connection);
    m_pSocket->connectToHost(ip, port);
    m_timer->start(1000);
  }
}

TCPSender::~TCPSender()
{
  delete m_pServer;
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
  if (m_pSocket == nullptr) m_pSocket = m_pServer->nextPendingConnection();
  connected = true;
  connect(m_pSocket, &QIODevice::readyRead, this, &TCPSender::readStream);
  connect(m_pSocket, &QTcpSocket::disconnected, this, &TCPSender::disconnected);
  std::cout << "Connection Successful" << std::endl;
  int i = m_pSocket->write("YAY?");
  std::cout << "wrote " << std::to_string(i)
            << std::endl;
}

void TCPSender::readStream()
{
  m_timer->start(1000);
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

bool TCPSender::isConnected()
{
  return connected;
}
