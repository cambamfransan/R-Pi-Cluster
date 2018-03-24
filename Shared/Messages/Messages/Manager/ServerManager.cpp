#include "ServerManager.hpp"
#include "MakeMsgs.hpp"
#include "Logger/Logger.hpp"

manager::ServerManager::ServerManager(
  int id,
  std::shared_ptr<TCPSenderServer> pServerSender,
  std::shared_ptr<TCPSenderWeb> pWebSender,
  std::string database)
  : m_myId(id),
    m_jobManager(database), 
    m_piManager(),          
    m_pServerSender(pServerSender),
    m_pWebSender(pWebSender)
{
}

manager::ServerManager::~ServerManager() {}

void manager::ServerManager::addResults(msg::MsgToSend* pMsg)
{
  auto results = pMsg->results().results();
  std::map<int, std::vector<manager::Result>> toSave;
  std::vector<manager::Task> tasksCompleted;

  for (auto&& result : results)
  {
    int job(result.task().jobid());
    int page(result.task().pagenumber());
    int id(result.task().id());
    std::string execute(result.task().toexecute());
    manager::Task task(job, page, id, execute);
    toSave[job].emplace_back(task, result.result());

    tasksCompleted.push_back(task);
  }

  for (auto&& job : toSave)
  {
    m_jobManager.addJobResults(job.first, job.second);
  }

  // find how many jobs to get
  int clientId(pMsg->basicmsg().fromid());
  int tasksToGet(m_piManager.getAmountToSend(clientId));

  auto toSend = m_jobManager.getTasks(tasksToGet);
  if (toSend.empty()) return;
  m_piManager.changePiTasks(clientId, tasksCompleted, toSend);
  int nextConvId(m_pServerSender->getNextConvId());
  m_pServerSender->send(
    make_msgs::makeTaskMsg(m_myId, clientId, nextConvId, toSend), clientId);
}

void manager::ServerManager::addPi(std::string ip, int port)
{
  // Add pi and send tasks to it
  int id(m_piManager.addPi(ip, port));
  Logger::info("Pi added");

  int nextConvId(m_pServerSender->getNextConvId());
  m_pServerSender->send(make_msgs::makeIdMsg(m_myId, id, nextConvId), id);

  auto tasks = m_jobManager.getTasks(m_piManager.getAmountToSend(id));

  if (tasks.empty()) return;
  Logger::info("sending Tasks");
  m_piManager.changePiTasks(id, {}, tasks);

  nextConvId = m_pServerSender->getNextConvId();
  m_pServerSender->send(
    make_msgs::makeTaskMsg(m_myId, id, nextConvId, tasks), id);
}

void manager::ServerManager::addJob(int size,
                                    int pri,
                                    int taskpb,
                                    std::string gitUrl)
{
  // If active pis, send tasks to it
  m_jobManager.addJob(size, pri, taskpb, gitUrl);

  int id;
  while ((id = m_piManager.waitingPis()) != -1)
  {
    auto toSend = m_jobManager.getTasks(m_piManager.getAmountToSend(id));
    m_piManager.changePiTasks(id, {}, toSend);
    int convId = m_pServerSender->getNextConvId();
    m_pServerSender->send(
      make_msgs::makeTaskMsg(m_myId, id, convId, toSend), id);
  }
}

void manager::ServerManager::sendUpdates()
{
  //TODO Send updates
  /*
      std::lock_guard<std::mutex> gaurd(m_clientInfosMutex);
    for (auto&& info : m_clientInfos)
    {
      int nextConvId(m_pSender->getNextConvId());
      auto pMsg = make_msgs::makeUpdateMsg(m_myId,
                                           info.second.getClientId(),
                                           msg::ProtoType::UPDATE,
                                           nextConvId,
                                           m_clientInfos);

      send(pMsg,
           nextConvId,
           std::chrono::seconds(1),
           true,
           info.second.getClientId());
    }
*/
}

void manager::ServerManager::updateAck(int id)
{
  m_piManager.updateAck(id);
}

void manager::ServerManager::removePi(int id)
{
  m_piManager.removePi(id);
}
