#ifndef UDPSENDER_H
#define UDPSENDER_H

#include "ProtoFiles/MsgToSend.pb.h"
#include <chrono>
#include <mutex>
#include <qhostaddress.h>
#include <qudpsocket.h>
#include <string>
#include <thread>

class UDPSender : public QObject
{
  Q_OBJECT

    signals :
  void msgReceived(std::string, QHostAddress, qint16);

public:
  UDPSender(qint16 port = 0);
  ~UDPSender();

  qint64 send(std::string msg,
    QHostAddress receiver,
    quint16 port);

private:
  QUdpSocket* m_pSocket;

  private slots:
  void readPendingDatagrams();
};

#endif