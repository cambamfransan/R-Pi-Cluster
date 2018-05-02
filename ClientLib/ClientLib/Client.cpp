#include "Client.hpp"
#include "Logger/Logger.hpp"
#include "Messages/MakeMsgs.hpp"
#include "Messages/MapHelpers.hpp"
#include <iostream>
#include <thread>

Client::Client(QHostAddress addr, qint16 port, std::string arg)
  : m_pSender(std::make_shared<TCPSenderClient>(addr, port)),
    m_serverId(1),
    m_myId(0),
    m_myPriority(99),
#if (TESTING_GUIS == 1)
    m_window(new MainWindow()),
#endif
//    m_outMessages(),
//    m_inputMessages(),
//    m_allClientsInfo(),
    m_database(arg),
  m_pClientManager(std::make_shared<manager::ClientManager>(m_pSender, m_database))
{
#if (TESTING_GUIS == 1)
  m_window->show();
  connect(m_window, &MainWindow::signalClicked, this, &Client::clicked);
#endif
  connect(m_pSender.get(),
          &TCPSenderClient::newConnection,
          this,
          &Client::newConnection);
  connect(m_pSender.get(),
          &TCPSenderClient::msgReceived,
          this,
          &Client::recieveMessage);
  connect(m_pSender.get(),
          &TCPSenderClient::lostConnection,
          this,
          &Client::lostConnection);

  //connect clientManager
  connect(m_pClientManager.get(), 
          &manager::ClientManager::sendResultsToClientMain, 
          this, 
          &Client::sendResults);
}

Client::~Client() {}

void Client::newConnection()
{
#if (TESTING_GUIS == 1)
  m_window->setPort(m_pSender->getLocalPort());
  m_window->addConnection(
    m_pSender->getPeerAddress(), m_pSender->getPeerPort());
#endif
}

void Client::recieveMessage(msg::MsgToSend* pMsg, QHostAddress ip, qint16 port)
{
//  int id =
//    make_msgs::getMapId(pMsg->basicmsg().fromid(), pMsg->basicmsg().convid());
//  if (m_inputMessages.find(id) == m_inputMessages.end())
//    m_inputMessages[id] = std::chrono::steady_clock::now();
//  else
//    return;
//  if (auto itr = m_outMessages.find(make_msgs::getMapId(
//                   pMsg->basicmsg().toid(), pMsg->basicmsg().convid())) ==
 //                m_outMessages.end())
 //   m_outMessages.erase(itr);
#if (TESTING_GUIS == 1)
  m_window->receivedMsg(pMsg->DebugString(), ip, port);
#endif

  int convId = pMsg->basicmsg().convid();
  switch (pMsg->basicmsg().msgtype())
  {
  case msg::ProtoType::ID_MSG:
    m_myId = pMsg->basicmsg().toid();
    m_serverId = pMsg->basicmsg().fromid();
    send(make_msgs::makeIdMsgAck(m_myId, m_serverId, convId),
      convId);/*,
         std::chrono::seconds(1),
         false);*/
    break;
  case msg::ProtoType::UPDATE:
    recieveUpdate(pMsg, convId);
    break;
  case msg::ProtoType::TASK_MSG:
    {
      std::vector<manager::Task> tasks;
      auto pTasks = pMsg->task().task();
      for(const auto& pT : pTasks)
        tasks.emplace_back(pT.jobid(), pT.pagenumber(), pT.id(), pT.toexecute());
      m_pClientManager->execute(tasks);
    }
    break;
  case msg::ProtoType::CURRENT_STATE:
    receiveCurState(pMsg);
    break;
  }
}

void Client::send(msg::MsgToSend* pMsg,
                  int convId/*,
                  std::chrono::seconds timeout,
                  bool requireResponse*/)
{
//  if (requireResponse)
//  {
//    m_outMessages[make_msgs::getMapId(m_myId, pMsg->basicmsg().convid())] =
//      Conversation{
//        pMsg, convId, timeout, std::chrono::steady_clock::now(), m_serverId};
//  }
  m_pSender->send(pMsg);
}

void Client::clicked(std::string msg)
{
  msg::MsgToSend* pMsg =
    make_msgs::makeTestMsg(m_myId, 1, m_pSender->getNextConvId(), msg);
  send(pMsg, 1)/*, std::chrono::seconds(1), false)*/;
}

void Client::lostConnection()
{
  std::cout << "Client lost connection" << std::endl;
}

void Client::recieveUpdate(msg::MsgToSend* pMsg, int convId)
{
  std::map<int, manager::Pi> infos; 
  send(make_msgs::makeBasicMsgToSend(
    m_myId, pMsg->basicmsg().fromid(), msg::ProtoType::UPDATE_ACK, convId),
    convId);/* ,
       std::chrono::seconds(1),
       false);*/
  std::thread thr([this, pMsg](){
    m_pClientManager->update(pMsg->update());
      });
  thr.detach();
}

void Client::receiveCurState(msg::MsgToSend* pMsg)
{
//  std::thread thr([this, pMsg](){
      m_pClientManager->receiveCurState(pMsg->state());
//      });
//  thr.detach();
}

void Client::sendResults(std::vector<manager::Result> results)
{
  int convId(m_pSender->getNextConvId());
  m_pSender->send(
      make_msgs::makeResultsMsg(m_myId, m_serverId, convId, results));
}

