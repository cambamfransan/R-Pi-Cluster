#include "ServerManager.hpp"
#include "MakeMsgs.hpp"

manager::ServerManager::ServerManager(
  int id,
  std::shared_ptr<TCPSenderServer> pServerSender,
  std::shared_ptr<TCPSenderWeb> pWebSender,
  std::string database)
  : m_myId(id),
    m_jobManager(database), // Verify Constructors
    m_piManager(), // Verify Constructor
    m_pServerSender(pServerSender),
    m_pWebSender(pWebSender)
{
}

manager::ServerManager::~ServerManager() {}

void manager::ServerManager::receiveMsg(msg::MsgToSend* pMsg)
{
  //Implement this function
}

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
  m_piManager.changePiTasks(clientId, tasksCompleted, toSend);
  int nextConvId(m_pServerSender->getNextConvId());
  m_pServerSender->send(make_msgs::makeTaskMsg(m_myId, clientId, nextConvId, toSend), clientId);
}

void manager::ServerManager::addPi(std::string ip, int port)
{
  //Add pi and send tasks to it
}

void manager::ServerManager::addJob(
  int id, int size, int pri, int taskpb, std::string gitUrl)
{
  //If active pis, send tasks to it
}
