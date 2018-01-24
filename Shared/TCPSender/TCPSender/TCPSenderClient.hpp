#ifndef TCPSENDER_CLIENT_H
#define TCPSENDER_CLIENT_H

#include "ProtoFiles/MsgToSend.pb.h"
#include <chrono>
#include <qhostaddress.h>
#include <qtcpsocket.h>
#include <qtcpserver.h>
#include <string>
#include "Conversation.hpp"

class TCPSenderClient : public QObject {
  Q_OBJECT

public:
  TCPSenderClient(QHostAddress ip, qint16 port = 0);
  ~TCPSenderClient();

  signals:
    void msgReceived(msg::MsgToSend*, QHostAddress, qint16);
    void lostConnection();
    void newConnection();

public:
  qint64 send(msg::MsgToSend* pMsg, int msgId, std::chrono::seconds timeout,
              bool requireResponse);

#ifdef TESTING
  qint64 send(std::string msg, int msgId, std::chrono::seconds timeout,
    bool requireResponse);
#endif

  quint16 getLocalPort();
  quint16 getPeerPort();
  QHostAddress getPeerAddress();
  int getNextConvId();

private slots: 
  void connection();
  void disconnected();
  void readStream();
  void emitMessage();

private:
  std::shared_ptr<QTcpSocket> m_connectingSocket;
  int m_myId;
  int m_nextConvId;
  int m_serverId;
  std::map<int, Conversation> m_outMessages;
  std::map<int, std::chrono::steady_clock::time_point> m_inputMessages;
};

#endif