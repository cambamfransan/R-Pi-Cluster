#ifndef SERVER_CLASS_H
#define SERVER_CLASS_H

#include "TCPSender/TCPSenderServer.hpp"
#include "TCPSender/TCPSenderWeb.hpp"
#include <qobject.h>
#include <chrono>
#include <rapidjson/document.h>
#include <mutex>
#include <qtimer.h>
#include "Messages/Pi/Pi.hpp"
#include "Messages/Manager/ServerManager.hpp"

#if (TESTING_GUIS == 1)
#include "Gui/mainwindow.hpp"
#endif

class Server : public QObject
{
  Q_OBJECT

public:
  Server(std::string arg);
  ~Server();

  void send(msg::MsgToSend* pMsg,
    int convId,
    std::chrono::seconds timeout,
    bool requireResponse, int endpointId);

  void sendToWeb(const std::string& msg,
    int convId,
    std::chrono::seconds timeout,
    bool requireResponse);

private slots:
  void newConnection(int);
  void newWebConnection();
  void clicked(std::string s);
  void recieveMessage(msg::MsgToSend* pMsg, QHostAddress, qint16);
  void receiveMessageWeb(std::string msg);
  void lostConnection(int);
  void lostConnectionWeb();
  void sendTimedMsgs();

  void sendTasksToSend(std::vector<manager::Task>, int);

private:
  std::shared_ptr<TCPSenderServer> m_pSender;
  std::shared_ptr<TCPSenderWeb> m_pWebSender;
  //std::shared_ptr<std::mutex> m_clientsMutex;
  //std::shared_ptr<std::map<int, std::chrono::steady_clock::time_point>> m_clientIds;
//  std::mutex m_clientInfosMutex;
  //std::map<int,manager::Pi> m_clientInfos;
  int m_myId;
  std::shared_ptr<manager::ServerManager> m_pServerManager;
  bool connectedToWeb;
  
#if (TESTING_GUIS == 1)
  MainWindow* m_window;
#endif
//  std::mutex m_outMessagesMutex;
//  std::map<int/*clientId*/, std::map<int/*convID*/,Conversation>> m_outMessages;
//  std::mutex m_webOutMessagesMutex;
//  std::map<int/*convID*/, JSONConversation> m_webOutMessages;
 // std::map<int, std::chrono::steady_clock::time_point> m_inputMessages;
 // std::map<int, std::chrono::steady_clock::time_point> m_webInputMessages;

//  int m_nextPriority;

  //Timers
  QTimer* m_pTimer;
  std::string m_database;
};

#endif
