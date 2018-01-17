#ifndef SERVER_CLASS_H
#define SERVER_CLASS_H

#include "TCPSender/TCPSender.hpp"
#include <qobject.h>

class Server : public QObject
{
  Q_OBJECT

public:
  Server();
  ~Server();

  void send(std::string msg);

private:
  TCPSender m_sender;
};

#endif
