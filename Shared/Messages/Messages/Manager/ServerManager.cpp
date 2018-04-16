#include "ServerManager.hpp"
#include "Logger/Logger.hpp"
#include "MakeMsgs.hpp"
#include <iostream>

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
  m_piManager.changePiTasks(clientId, tasksCompleted, {});
  int tasksToGet(m_piManager.getAmountToSend(clientId));
  Logger::info("getting tasks: " + std::to_string(tasksToGet));

  auto toSend = m_jobManager.getTasks(tasksToGet);
  std::cout << "Get Tasks: " << toSend.size() << std::endl;
  if (toSend.empty()) return;
  std::cout << "Get Tasks done" << std::endl;
  m_piManager.changePiTasks(clientId, {}, toSend);
  int nextConvId(m_pServerSender->getNextConvId());
  m_pServerSender->send(
    make_msgs::makeTaskMsg(m_myId, clientId, nextConvId, toSend), clientId);
}

void manager::ServerManager::addPi(int id, std::string ip, int port)
{
  // Add pi and send tasks to it
  m_piManager.addPi(id, ip, port);
  Logger::info("Pi added: " + std::to_string(id));

  int nextConvId(m_pServerSender->getNextConvId());
  m_pServerSender->send(make_msgs::makeIdMsg(m_myId, id, nextConvId), id);

  auto tasks = m_jobManager.getTasks(m_piManager.getAmountToSend(id));

  if (tasks.empty()) return;
  Logger::info("sending Tasks");
  m_piManager.changePiTasks(id, {}, tasks);

  nextConvId = m_pServerSender->getNextConvId();
  m_pServerSender->send(
    make_msgs::makeTaskMsg(m_myId, id, nextConvId, tasks), id);
  // send all info
}

int manager::ServerManager::getNextJobId()
{
  return m_jobManager.getNextId();
}

int manager::ServerManager::addJob(int size,
                                    int pri,
                                    int taskpb,
                                    std::string gitUrl,
                                    int jobId)
{
  // If active pis, send tasks to it
  m_jobManager.addJob(size, pri, taskpb, gitUrl, jobId);

  int id;
  while ((id = m_piManager.waitingPis()) != -1)
  {
    Logger::info("Id == " + std::to_string(id));
    auto toSend = m_jobManager.getTasks(m_piManager.getAmountToSend(id));
    Logger::info("Size: " + std::to_string(toSend.size()));
    m_piManager.changePiTasks(id, {}, toSend);
    //int convId = m_pServerSender->getNextConvId();
    emit tasksToSend(toSend, id);
    //m_pServerSender->send(
    //  make_msgs::makeTaskMsg(m_myId, id, convId, toSend), id);
  }
  Logger::info("Adding Job in Server Manager as: " + std::to_string(jobId));
  return jobId;
}

void manager::ServerManager::sendUpdates()
{
  std::vector<int> pis = m_piManager.getClientIds();
  auto piUpdate = m_piManager.getUpdates();
  auto jobUpdate = m_jobManager.getUpdates();

  auto pMsg = make_msgs::makeUpdateMsg(
    m_myId, 0, 0, piUpdate, jobUpdate);
  for (const auto& pi : pis)
  {
    pMsg->mutable_basicmsg()->set_convid(m_pServerSender->getNextConvId());
    pMsg->mutable_basicmsg()->set_toid(pi);
    //std::cout << "Sending to: " << pi << std::endl;
    m_pServerSender->send(pMsg, pi);
  }
}

void manager::ServerManager::updateAck(int id)
{
  m_piManager.updateAck(id);
}

void manager::ServerManager::removePi(int id)
{
  m_piManager.removePi(id);
}

void manager::ServerManager::removeUnresponsive()
{
  m_piManager.removeUnresponsive();
}

