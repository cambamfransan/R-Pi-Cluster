#ifndef CLIENT_CLASS_H
#define CLIENT_CLASS_H

#include "Messages/ClientInfo.hpp"
#include "TCPSender/TCPSenderClient.hpp"
#include <qobject.h>

#if (TESTING_GUIS == 1)
#include <Gui/mainwindow.hpp>
#endif

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
  void send(msg::MsgToSend* pMsg,
            int convId,
            std::chrono::seconds timeout,
            bool requireResponse);
  void lostConnection();
  void recieveUpdate(msg::MsgToSend* pMsg, int convId);

private:
  std::shared_ptr<TCPSenderClient> m_pSender;
  int m_serverId;
  int m_myId;
  int m_myPriority;
#if (TESTING_GUIS == 1)
  MainWindow* m_window;
#endif
  std::map<int, Conversation> m_outMessages;
  std::map<int, std::chrono::steady_clock::time_point> m_inputMessages;
  std::map<int, ClientInfo> m_allClientsInfo;
};

#endif
