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

  void send(msg::MsgToSend* pMsg,
    int convId,
    std::chrono::seconds timeout,
    bool requireResponse, int endpointId);

private slots:
  void newConnection(int);
  void clicked(std::string s);
  void recieveMessage(msg::MsgToSend* pMsg, QHostAddress, qint16);
  void lostConnection(int);

private:
  std::shared_ptr<TCPSenderServer> m_pSender;
  std::vector<int> m_clientIds;
  int m_myId;
  MainWindow* m_window;
  std::map<int, Conversation> m_outMessages;
  std::map<int, std::chrono::steady_clock::time_point> m_inputMessages;
};

#endif
