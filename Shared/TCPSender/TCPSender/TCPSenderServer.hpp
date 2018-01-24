#ifndef TCPSENDER_SERVER_H
#define TCPSENDER_SERVER_H

#include "ProtoFiles/MsgToSend.pb.h"
#include <chrono>
#include <mutex>
#include <qhostaddress.h>
#include <qtcpsocket.h>
#include <qtcpserver.h>
#include <string>
#include <thread>

class TCPSenderServer : public QObject {
  Q_OBJECT

public:
  TCPSenderServer();
  ~TCPSenderServer();

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
  std::shared_ptr<QTcpSocket> getSocket(int id);

private slots: 
  void connection();
  void disconnected();
  void readStream();
  void emitMessage();
private:
  std::shared_ptr<QTcpServer> m_pServer;
  std::map<int, std::shared_ptr<QTcpSocket>> m_pSockets;
  int m_nextId;
  int m_myId;
};

#endif