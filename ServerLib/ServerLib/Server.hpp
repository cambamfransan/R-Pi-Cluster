#ifndef SERVER_CLASS_H
#define SERVER_CLASS_H

#include "TCPSender/TCPSenderServer.hpp"
#include <qobject.h>
#include <Gui/mainwindow.hpp>

class Server : public QObject
{
  Q_OBJECT

public:
  Server();
  ~Server();

  void send(std::string msg);

private slots:
  void newConnection(int);
  void clicked(std::string s);
  void recieveMessage(msg::MsgToSend*, QHostAddress, qint16);

private:
  std::shared_ptr<TCPSenderServer> m_pSender;
  std::vector<int> m_clientIds;
  MainWindow* m_window;
};

#endif
