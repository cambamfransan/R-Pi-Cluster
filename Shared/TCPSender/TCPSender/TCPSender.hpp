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

public:
  qint64 send(msg::MsgToSend msg, int convId, std::chrono::seconds timeout,
              bool requireResponse, QHostAddress receiver, quint16 port);

  qint64 send(std::string msg, int convId, std::chrono::seconds timeout,
    bool requireResponse, QHostAddress receiver, quint16 port);


private slots:
  void connection();
  void disconnected();
  void readStream();
  void update();

private:
  QTcpServer *m_pServer;
  QTcpSocket* m_pSocket;
  QTimer* m_timer;
};

#endif