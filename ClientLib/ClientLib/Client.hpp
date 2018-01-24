#ifndef CLIENT_CLASS_H
#define CLIENT_CLASS_H

#include "TCPSender/TCPSenderClient.hpp"
#include <qobject.h>
#include <Gui/mainwindow.hpp>

class Client : public QObject
{
  Q_OBJECT

public:
  Client(QHostAddress addr, qint16 port);
  ~Client();

private slots:
  void newConnection();
  void clicked(std::string s);
  void recieveMessage(msg::MsgToSend* msg, QHostAddress, qint16);

private:
  std::shared_ptr<TCPSenderClient> m_pSender;
  int m_serverId;
  MainWindow* m_window;

};

#endif
