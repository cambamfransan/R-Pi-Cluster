#include "Listener.hpp"
#include <iostream>

TCPListener::TCPListener(QHostAddress ip, qint16 port)
  :QObject(nullptr)
{
  m_pSocket = new QTcpSocket(this);
  connect(m_pSocket, &QIODevice::readyRead, this, &TCPListener::readStream);

  std::cout << "What port?" << std::endl;
  std::string t;
  std::cin >> t;
  m_pSocket->connectToHost("127.0.0.1", std::stoi(t));

}

TCPListener::~TCPListener()
{
}

void TCPListener::connection()
{
  std::cout << "Connection Successful" << std::endl;
}

void TCPListener::readStream()
{
  std::cout << "This is one step closer." << std::endl;
  std::cout << m_pSocket->readAll().toStdString() << std::endl;
}


//
//void TCPListener::newConnection()
//{
//  while (m_pServer->hasPendingConnections())
//  {
//    QTcpSocket *socket = m_pServer->nextPendingConnection();
//    connect(socket, SIGNAL(readyRead()), SLOT(readBuffer()));
//    QByteArray* buffer = new QByteArray();
//    m_buffers.insert(socket, buffer);
//  }
//}

//void TCPListener::readBuffer()
//{
//  QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
//  QByteArray *buffer = m_buffers.value(socket);
//
//  buffer->append(socket->readAll());
//  if (buffer->size() > 100) // Need to place size of actual message here
//  {
//    std::cout << buffer->data() << std::endl;
//    emit msgReceived(std::string(buffer->data()), socket->peerAddress(), socket->peerPort());
//  }
//}