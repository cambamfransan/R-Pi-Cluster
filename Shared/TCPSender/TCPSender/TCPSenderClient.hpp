#ifndef TCPSENDER_CLIENT_H
#define TCPSENDER_CLIENT_H

#include "Messages/Conversation.hpp"
#include "ProtoFiles/MsgToSend.pb.h"
#include <chrono>
#include <qhostaddress.h>
#include <qtcpserver.h>
#include <qtcpsocket.h>
#include <string>

class TCPSenderClient : public QObject
{
  Q_OBJECT

public:
  TCPSenderClient(QHostAddress ip, qint16 port = 0);
  ~TCPSenderClient();

signals:
  void msgReceived(msg::MsgToSend*, QHostAddress, qint16);
  void lostConnection();
  void newConnection();

public:
  qint64 send(msg::MsgToSend* pMsg);

  quint16 getLocalPort();
  quint16 getPeerPort();
  QHostAddress getPeerAddress();
  int getNextConvId();

private slots:
  void connection();
  void disconnected();
  void emitMessage();

private:
  std::shared_ptr<QTcpSocket> m_pServerSocket;
  int m_myId;
  int m_nextConvId;
  int m_serverId;
};

#endif