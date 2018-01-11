#ifndef TCPENDPOINT_H
#define TCPENDPOINT_H

#include "Listener.hpp"
#include "ProtoFiles/MsgToSend.pb.h"
#include "TCPSender.hpp"
#include <chrono>
#include <mutex>
#include <qhostaddress.h>
#include <qtcpserver.h>
#include <qtcpsocket.h>
#include <string>
#include <thread>

class TCPEndpoint : public QObject {
  Q_OBJECT

public:
  TCPEndpoint(QHostAddress ip, qint16 port = 0);
  ~TCPEndpoint();

  qint64 send(msg::MsgToSend msg, int convId, std::chrono::seconds timeout,
              bool requireResponse, QHostAddress receiver, quint16 port);

  qint64 send(std::string msg, int convId, std::chrono::seconds timeout,
              bool requireResponse, QHostAddress receiver, quint16 port);

private slots:
  void connection();
  void msgReceived(std::string, QHostAddress, qint16);

private:
  TCPListener *m_pListener;
  TCPSender *m_pSender;
};

#endif