#include "MakeMsgs.hpp"

msg::MsgToSend* make_msgs::makeTestMsg(int fromId,
                                       int toId,
                                       int convId,
                                       std::string msg)
{
  msg::MsgToSend* pToReturn = new msg::MsgToSend();
  pToReturn->mutable_test()->add_teststring(msg);
  pToReturn->set_allocated_basicmsg(
    makeBasicMsg(fromId, toId, msg::ProtoType::TEST_MSG, convId));
  return pToReturn;
}

msg::MsgToSend* make_msgs::makeTaskMsg(int fromId,
                                       int toId,
                                       int convId,
                                       std::vector<manager::Task> task)
{
  msg::MsgToSend* pToReturn = new msg::MsgToSend();
  for (auto&& t : task)
  {
    msg::Task* pTask = new msg::Task();
    pTask->set_jobid(t.jobId);
    pTask->set_pagenumber(t.pageNumber);
    pTask->set_jobid(t.taskId);
    pTask->set_toexecute(t.toExecute);
    pTask = pToReturn->mutable_task()->add_task();
  }
  pToReturn->set_allocated_basicmsg(
    makeBasicMsg(fromId, toId, msg::ProtoType::TASK_MSG, convId));
  return pToReturn;
}

msg::BasicMsg* make_msgs::makeBasicMsg(int fromId,
                                       int toId,
                                       int msgType,
                                       int convId)
{
  msg::BasicMsg* pToReturn = new msg::BasicMsg();
  pToReturn->set_attempt(1);
  pToReturn->set_convid(convId);
  pToReturn->set_fromid(fromId);
  pToReturn->set_msgtype(msgType);
  pToReturn->set_toid(toId);
  return pToReturn;
}

msg::MsgToSend* make_msgs::makeBasicMsgToSend(int fromId,
                                              int toId,
                                              int msgType,
                                              int convId)
{
  msg::MsgToSend* pToReturn = new msg::MsgToSend();
  pToReturn->set_allocated_basicmsg(
    makeBasicMsg(fromId, toId, msgType, convId));
  return pToReturn;
}

msg::MsgToSend* make_msgs::makeUpdateMsg(
  int fromId,
  int toId,
  int convId,
  manager::PiUpdateStruct pis,
  manager::JobUpdateStruct jobs)
{
  msg::MsgToSend* pToReturn = new msg::MsgToSend();
  pToReturn->set_allocated_basicmsg(
    makeBasicMsg(fromId, toId, msg::ProtoType::UPDATE, convId));

  for (auto&& c : pis.newPis)
  {
    auto pClient = pToReturn->mutable_update()->add_newclients();
    pClient->set_ipaddress(c.getIpAddress());
    pClient->set_port(c.getPort());
    pClient->set_username(c.getUsername());
    pClient->set_password(c.getPassword());
    pClient->set_priority(c.getPriority());
    pClient->set_clientid(c.getClientId());
  }

  for (auto&& c : pis.lostPis)
  {
    pToReturn->mutable_update()->add_lostclients(c);
  }

  for (auto&& c : pis.modifiedPis)
  {
    auto modifiedPi = pToReturn->mutable_update()->add_modifiedpis();
    modifiedPi->set_id(c.id);
    modifiedPi->set_field(c.field);
    modifiedPi->set_value(c.value);
  }

  for (auto&& j : jobs.newJobs)
  {
    auto pJob = pToReturn->mutable_update()->add_newjobs();
    pJob->set_id(j.getJobId());
    pJob->set_size(j.getSize());
    pJob->set_priority(j.getPriority());
    pJob->set_taskperbundle(j.getTasksPerBundle());
    pJob->set_giturl(j.getUrl());
    pJob->set_name(j.getName());
    pJob->set_toexec(j.getExec());
    pJob->set_status(static_cast<int>(j.getStatus()));
  }

  for (auto&& j : jobs.lostJobs)
  {
    pToReturn->mutable_update()->add_lostjobs(j);
  }

  for (auto&& j : jobs.modifiedJobs)
  {
    auto pMod = pToReturn->mutable_update()->add_modifiedjobs();
    pMod->set_id(j.id);
    pMod->set_field(j.field);
    pMod->set_value(j.value);
  }

  for (auto&& i : jobs.newResults)
  {
    auto pResults = pToReturn->mutable_update()->add_results();
    for (auto&& j : i)
    {
      auto pResult = pResults->add_results();
      msg::Task* pTask = new msg::Task();
      pTask->set_id(j.first.taskId);
      pTask->set_jobid(j.first.jobId);
      pTask->set_pagenumber(j.first.pageNumber);
      pTask->set_toexecute(j.first.toExecute);
      pResult->set_allocated_task(pTask);
      pResult->set_result(j.second);
    }
  }

  return pToReturn;
}

msg::MsgToSend* make_msgs::makeIdMsg(int fromId, int toId, int convId)
{
  msg::MsgToSend* pToReturn = new msg::MsgToSend();
  pToReturn->set_allocated_basicmsg(
    makeBasicMsg(fromId, toId, msg::ProtoType::ID_MSG, convId));
  return pToReturn;
}

msg::MsgToSend* make_msgs::makeIdMsgAck(int fromId, int toId, int convId)
{
  msg::MsgToSend* pToReturn = new msg::MsgToSend();
  pToReturn->set_allocated_basicmsg(
    makeBasicMsg(fromId, toId, msg::ProtoType::ID_MSG_ACK, convId));
  return pToReturn;
}
