#ifndef CLIENT_CLASS_H
#define CLIENT_CLASS_H

#include "TCPSender/TCPSender.hpp"
#include <qobject.h>

class Client : public QObject
{
  Q_OBJECT

public:
  Client(QHostAddress addr, qint16 port);
  ~Client();

private slots:
  void newConnection(int);

private:
  std::shared_ptr<TCPSender> m_pSender;
  int m_serverId;
};

#endif
