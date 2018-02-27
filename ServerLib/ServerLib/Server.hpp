#ifndef SERVER_CLASS_H
#define SERVER_CLASS_H

#include "TCPSender/TCPSenderServer.hpp"
#include "TCPSender/TCPSenderWeb.hpp"
#include <qobject.h>
#include <chrono>
#include <Gui/mainwindow.hpp>
#include <rapidjson/document.h>
#include <mutex>
#include "Messages/ClientInfo.hpp"

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

  void sendToWeb(const rapidjson::Document& msg,
    int convId,
    std::chrono::seconds timeout,
    bool requireResponse, int endpointId);

private slots:
  void newConnection(int);
  void newWebConnection();
  void clicked(std::string s);
  void recieveMessage(msg::MsgToSend* pMsg, QHostAddress, qint16);
  void receiveMessageWeb(std::string msg);
  void lostConnection(int);
  void lostConnectionWeb();
  void sendTimedMsgs();

private:
  std::shared_ptr<TCPSenderServer> m_pSender;
  std::shared_ptr<TCPSenderWeb> m_pWebSender;
  std::shared_ptr<std::mutex> m_clientsMutex;
  std::shared_ptr<std::map<int, std::chrono::steady_clock::time_point>> m_clientIds;
  std::mutex m_clientInfosMutex;
  std::map<int,ClientInfo> m_clientInfos;
  int m_myId;
  MainWindow* m_window;
  std::mutex m_outMessagesMutex;
  std::map<int/*clientId*/, std::map<int/*convID*/,Conversation>> m_outMessages;
  std::map<int, std::chrono::steady_clock::time_point> m_inputMessages;
  int m_nextPriority;

  //Timers
  QTimer* m_pTimer;
};

#endif
