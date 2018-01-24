#ifndef TCPSENDER_CLIENT_H
#define TCPSENDER_CLIENT_H

#include "ProtoFiles/MsgToSend.pb.h"
#include <chrono>
#include <mutex>
#include <qhostaddress.h>
#include <qtcpsocket.h>
#include <qtcpserver.h>
#include <string>
#include <thread>

class TCPSenderClient : public QObject {
  Q_OBJECT

public:
  TCPSenderClient(QHostAddress ip, qint16 port = 0);
  ~TCPSenderClient();

  signals:
    void msgReceived(std::string, QHostAddress, qint16);
    void lostConnection();
    void newConnection();

public:
  qint64 send(msg::MsgToSend msg, int msgId, std::chrono::seconds timeout,
              bool requireResponse);

  qint64 send(std::string msg, int msgId, std::chrono::seconds timeout,
    bool requireResponse);

  quint16 getLocalPort();
  quint16 getPeerPort();
  QHostAddress getPeerAddress();

private slots: 
  void connection();
  void disconnected();
  void readStream();
  void emitMessage();

private:
  std::shared_ptr<QTcpSocket> m_connectingSocket;
  int m_myId;

};

#endif