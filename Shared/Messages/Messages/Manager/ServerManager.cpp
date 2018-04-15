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
    m_pJobManager(std::make_shared<manager::JobManager>(database)),
    m_pPiManager(std::make_shared<manager::PiServerManager>()),
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
    std::cout << "job" << job << std::endl;
    int page(result.task().pagenumber());
    std::cout << "page" << page << std::endl;
    int id(result.task().id());
    std::cout << "id" << id << std::endl;
    std::string execute(result.task().toexecute());
    manager::Task task(job, page, id, execute);
    toSave[job].emplace_back(task, result.result());

    tasksCompleted.push_back(task);
  }

  for (auto&& job : toSave)
  {
    std::cout << "jobasdf" << std::endl;
    m_pJobManager->addJobResults(job.first, job.second);
  }

  // find how many jobs to get
    std::cout << "jobasdfasdf" << std::endl;
  int clientId(pMsg->basicmsg().fromid());
    std::cout << "jobasd" << clientId << std::endl;
  m_pPiManager->changePiTasks(clientId, tasksCompleted, {}); // this causes a seg fault
  int tasksToGet(m_pPiManager->getAmountToSend(clientId));

  auto toSend = m_pJobManager->getTasks(tasksToGet);
  std::cout << "Get Tasks" << std::endl;
  if (toSend.empty()) return;
  std::cout << "Get Tasks done" << std::endl;
  m_pPiManager->changePiTasks(clientId, {}, toSend);
  int nextConvId(m_pServerSender->getNextConvId());
  m_pServerSender->send(
    make_msgs::makeTaskMsg(m_myId, clientId, nextConvId, toSend), clientId);
}

void manager::ServerManager::addPi(int id, std::string ip, int port)
{
  // Add pi and send tasks to it
  m_pPiManager->addPi(id, ip, port);
  Logger::info("Pi added: " + std::to_string(id));

  int nextConvId(m_pServerSender->getNextConvId());
  m_pServerSender->send(make_msgs::makeIdMsg(m_myId, id, nextConvId), id);

  auto tasks = m_pJobManager->getTasks(m_pPiManager->getAmountToSend(id));

  if (tasks.empty()) return;
  Logger::info("sending Tasks");
  m_pPiManager->changePiTasks(id, {}, tasks);

  nextConvId = m_pServerSender->getNextConvId();
  m_pServerSender->send(
    make_msgs::makeTaskMsg(m_myId, id, nextConvId, tasks), id);
  // send all info
}

int manager::ServerManager::getNextJobId()
{
  return m_pJobManager->getNextId();
}

int manager::ServerManager::addJob(int size,
                                    int pri,
                                    int taskpb,
                                    std::string gitUrl,
                                    int jobId)
{
  // If active pis, send tasks to it
  m_pJobManager->addJob(size, pri, taskpb, gitUrl, jobId);

  int id;
  while ((id = m_pPiManager->waitingPis()) != -1)
  {
    Logger::info("Id == " + std::to_string(id));
    auto toSend = m_pJobManager->getTasks(m_pPiManager->getAmountToSend(id));
    Logger::info("Size: " + std::to_string(toSend.size()));
    m_pPiManager->changePiTasks(id, {}, toSend);
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
  std::vector<int> pis = m_pPiManager->getClientIds();
  auto piUpdate = m_pPiManager->getUpdates();
  auto jobUpdate = m_pJobManager->getUpdates();

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
  m_pPiManager->updateAck(id);
}

void manager::ServerManager::removePi(int id)
{
  m_pPiManager->removePi(id);
}

void manager::ServerManager::removeUnresponsive()
{
  m_pPiManager->removeUnresponsive();
}
