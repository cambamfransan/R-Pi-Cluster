#ifndef TCPSENDER_H
#define TCPSENDER_H

#include "ProtoFiles/MsgToSend.pb.h"
#include <chrono>
#include <mutex>
#include <qhostaddress.h>
#include <qtcpsocket.h>
#include <qtcpserver.h>
#include <string>
#include <thread>

class TCPSender : public QObject {
  Q_OBJECT

public:
  TCPSender(QHostAddress ip, qint16 port = 0);
  ~TCPSender();

  qint64 send(msg::MsgToSend msg, int convId, std::chrono::seconds timeout,
              bool requireResponse, QHostAddress receiver, quint16 port);

  qint64 send(std::string msg, int convId, std::chrono::seconds timeout,
    bool requireResponse, QHostAddress receiver, quint16 port);

  void connection();

  private slots:

private:
  QTcpServer *m_pServer;
  QTcpSocket* m_pSocket;
};

#endif