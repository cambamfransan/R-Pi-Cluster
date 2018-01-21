#ifndef SERVER_CLASS_H
#define SERVER_CLASS_H

#include "TCPSender/TCPSender.hpp"
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

private:
  std::shared_ptr<TCPSender> m_pSender;
  std::vector<int> m_clientIds;
  MainWindow* m_window;
};

#endif
