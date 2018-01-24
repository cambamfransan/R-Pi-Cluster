#define TESTING
#ifndef TCPSENDER_SERVER_H
#define TCPSENDER_SERVER_H

#include "ProtoFiles/MsgToSend.pb.h"
#include "Conversation.hpp"
#include <chrono>
#include <qhostaddress.h>
#include <qtcpsocket.h>
#include <qtcpserver.h>
#include <string>

class TCPSenderServer : public QObject {
  Q_OBJECT

public:
  TCPSenderServer();
  ~TCPSenderServer();

  signals:
    void msgReceived(msg::MsgToSend*, QHostAddress, qint16);
    void lostConnection();
    void newConnection(int);

public:
  qint64 send(msg::MsgToSend* pMsg, int msgId, std::chrono::seconds timeout,
              bool requireResponse, int endpointId);

#ifdef TESTING
  qint64 send(std::string msg, int msgId, std::chrono::seconds timeout,
    bool requireResponse, int endpointId);
#endif

  quint16 getServerPort();
  std::shared_ptr<QTcpSocket> getSocket(int id);
  int getNextConvId();

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
  int m_nextConvId;
  std::map<int, Conversation> m_outMessages;
  std::map<int, std::chrono::steady_clock::time_point> m_inputMessages;
};

#endif