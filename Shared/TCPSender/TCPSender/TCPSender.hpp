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

  signals:
    void msgReceived(std::string, QHostAddress, qint16);
    void lostConnection();
    void newConnection(int);

public:
  qint64 send(msg::MsgToSend msg, int msgId, std::chrono::seconds timeout,
              bool requireResponse, int endpointId);

  qint64 send(std::string msg, int msgId, std::chrono::seconds timeout,
    bool requireResponse, int endpointId);

  quint16 getServerPort();

private slots:
  void connection();
  void disconnected();
  void readStream();

private:
  std::shared_ptr<QTcpServer> m_pServer;
  std::map<int, std::shared_ptr<QTcpSocket>> m_pSockets;
  std::shared_ptr<QTcpSocket> m_connectingSocket;
  int m_nextId;
  int m_myId;

};

#endif