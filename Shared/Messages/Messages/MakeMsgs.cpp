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

msg::MsgToSend* make_msgs::makeUpdateMsg(int fromId,
                                         int toId,
                                         int msgType,
                                         int convId,
                                         std::map<int, manager::Pi> clients)
{
  msg::MsgToSend* pToReturn = new msg::MsgToSend();
  pToReturn->set_allocated_basicmsg(
    makeBasicMsg(fromId, toId, msgType, convId));

  for (auto&& c : clients)
  {
    auto pClient = pToReturn->mutable_update()->add_clients();
    pClient->set_ipaddress(c.second.getIpAddress());
    pClient->set_port(c.second.getPort());
    pClient->set_username(c.second.getUsername());
    pClient->set_password(c.second.getPassword());
    pClient->set_priority(c.second.getPriority());
    pClient->set_clientid(c.second.getClientId());
  }

  return pToReturn;
}

msg::MsgToSend* make_msgs::makeIdMsg(
  int fromId, int toId, int convId)
{
  msg::MsgToSend* pToReturn = new msg::MsgToSend();
  pToReturn->set_allocated_basicmsg(
    makeBasicMsg(fromId, toId, msg::ProtoType::ID_MSG, convId));
  return pToReturn;
}

msg::MsgToSend* make_msgs::makeIdMsgAck(int fromId,
                                        int toId,
                                        int convId)
{
  msg::MsgToSend* pToReturn = new msg::MsgToSend();
  pToReturn->set_allocated_basicmsg(
    makeBasicMsg(fromId, toId, msg::ProtoType::ID_MSG_ACK, convId));
  return pToReturn;
}
