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
#include <qtimer.h>

class TCPSender : public QObject {
  Q_OBJECT

public:
  TCPSender(QHostAddress ip, qint16 port = 0);
  ~TCPSender();

  signals:
    void msgReceived(std::string, QHostAddress, qint16);
    void lostConnection();

public:
  qint64 send(msg::MsgToSend msg, int msgId, std::chrono::seconds timeout,
              bool requireResponse, QHostAddress receiver, quint16 port);

  qint64 send(std::string msg, int msgId, std::chrono::seconds timeout,
    bool requireResponse, QHostAddress receiver, quint16 port);

private slots:
  void connection();
  void disconnected();
  void readStream();
  void update();

private:
  std::shared_ptr<QTcpServer> m_pServer;
  std::shared_ptr<QTcpSocket> m_pSocket;
  std::shared_ptr<QTimer> m_pTimer;

};

#endif