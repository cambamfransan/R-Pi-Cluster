#include "TCPEndpoint.hpp"
#include "Logger/Logger.hpp"
#include "ProtoFiles/MsgToSend.pb.h"

#include <qbytearray.h>

TCPEndpoint::TCPEndpoint(QHostAddress ip, qint16 port)
    : QObject(nullptr)
{
  m_pSender = new TCPSender(ip, port);
  m_pListener = new TCPListener(ip);
  connect(m_pSender, &TCPSender::connectionReceived, this, &TCPEndpoint::connection);
  connect(m_pListener, &TCPListener::msgReceived, this, &TCPEndpoint::msgReceived);
}

TCPEndpoint::~TCPEndpoint() {}

qint64 TCPEndpoint::send(msg::MsgToSend msg, int convId,
                       std::chrono::seconds timeout, bool requireResponse,
                       QHostAddress receiver, quint16 port)
{
  return m_pSender->send(msg, convId, timeout, requireResponse, receiver, port);
}

qint64 TCPEndpoint::send(std::string msg, int convId,
                       std::chrono::seconds timeout, bool requireResponse,
                       QHostAddress receiver, quint16 port)
{
  return m_pSender->send(msg, convId, timeout, requireResponse, receiver, port);
}

void TCPEndpoint::connection()
{
  m_pSender->send("YAY?!", 1, std::chrono::seconds(1), false, QHostAddress("127.0.0.1"), 0);
}
