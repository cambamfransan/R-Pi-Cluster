#include "UDPSender.hpp"
#include "Logger/Logger.hpp"
#include "ProtoFiles/MsgToSend.pb.h"
#include <iostream>

namespace
{
  const int MAX_TRIES = 5;
}

UDPSender::UDPSender(qint16 port)
  : QObject(nullptr),
  m_pSocket(new QUdpSocket)
{
  m_pSocket->bind(QHostAddress::Any,
    port,
    QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);

  connect(m_pSocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
}

UDPSender::~UDPSender()
{
    delete m_pSocket;
}

qint64 UDPSender::send(std::string msg,
  QHostAddress receiver,
  quint16 port)
{
  qint64 sent(m_pSocket->writeDatagram(
    QByteArray(msg.c_str(), static_cast<int>(msg.size())), receiver, port));
  Logger::info("Sent " + std::to_string(sent) + " bytes");
  return sent;
}

void UDPSender::readPendingDatagrams()
{
  while (m_pSocket->hasPendingDatagrams())
  {
   // std::cout << "received bytes" << std::endl;
    QByteArray datagram;
    QHostAddress host;
    quint16 port;
    datagram.resize(m_pSocket->pendingDatagramSize());
    m_pSocket->readDatagram(datagram.data(), datagram.size(), &host, &port);
    msg::MsgToSend* toSend = new msg::MsgToSend();
    toSend->ParseFromString(std::string(datagram.data()));
    emit msgReceived(std::string(datagram.data()), host, port);
  }
}