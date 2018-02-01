#ifndef UDPSENDER_H
#define UDPSENDER_H

#include "ProtoFiles/MsgToSend.pb.h"
#include <chrono>
#include <mutex>
#include <qhostaddress.h>
#include <qudpsocket.h>
#include <string>
#include <thread>

struct Conversation
{
  /* Vector of all of the messages in the conversation */
  std::string msg;
  /* The last time their was activity in the conversation */
  std::chrono::steady_clock::time_point lastUsed;
  /* How long till it needs to resend */
  std::chrono::seconds timeout;
  /* Address of who it was sent to*/
  QHostAddress receive;
  /* Port of who it was sent to*/
  quint16 port;

  Conversation()
    : msg(),
    lastUsed(std::chrono::steady_clock::now()),
    timeout(std::chrono::seconds(0)),
    receive("0.0.0.0"),
    port(0)
  {
  }

  Conversation(std::string msg,
    std::chrono::seconds newTimeout,
    bool requireAnswer,
    QHostAddress ip,
    quint16 p)
    : msg(msg),
    lastUsed(std::chrono::steady_clock::now()),
    timeout(newTimeout),
    receive(ip),
    port(p)
  {
  }
};


class UDPSender : public QObject
{
  Q_OBJECT

    signals :
  void msgReceived(std::string, QHostAddress, qint16);

public:
  UDPSender(qint16 port = 0);
  ~UDPSender();

  qint64 send(std::string msg,
    int convId,
    std::chrono::seconds timeout,
    bool requireResponse,
    QHostAddress receiver,
    quint16 port);

private:
  QUdpSocket* m_pSocket;

  private slots:
  void readPendingDatagrams();
};

#endif