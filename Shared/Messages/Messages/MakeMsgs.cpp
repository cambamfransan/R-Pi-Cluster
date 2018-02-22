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
                                       std::string msg)
{
  msg::MsgToSend* pToReturn = new msg::MsgToSend();
  pToReturn->mutable_task()->add_toexecute(msg);
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
                                         std::map<int, ClientInfo> clients)
{
  msg::MsgToSend* pToReturn = new msg::MsgToSend();
  pToReturn->set_allocated_basicmsg(
    makeBasicMsg(fromId, toId, msgType, convId));

  for (auto&& c : clients)
  {
    auto pClient = pToReturn->mutable_update()->add_clients();
    pClient->set_ipaddress(c.second.ipAddress);
    pClient->set_port(c.second.port);
    pClient->set_username(c.second.username);
    pClient->set_password(c.second.password);
    pClient->set_priority(c.second.priority);
    pClient->set_clientid(c.second.clientId);
    pClient->set_serverport(c.second.serverPort);
  }

  return pToReturn;
}

msg::MsgToSend* make_msgs::makeIdMsg(
  int fromId, int toId, int convId, std::string ipAddress, int port)
{
  msg::MsgToSend* pToReturn = new msg::MsgToSend();
  msg::IDMsg* pId = new msg::IDMsg;
  pId->set_ipaddress(ipAddress);
  pId->set_port(port);
  pToReturn->set_allocated_basicmsg(
    makeBasicMsg(fromId, toId, msg::ProtoType::ID_MSG, convId));
  pToReturn->set_allocated_newid(pId);
  return pToReturn;
}

msg::MsgToSend* make_msgs::makeIdMsgAck(int fromId,
                                        int toId,
                                        int convId,
                                        int port)
{
  msg::MsgToSend* pToReturn = new msg::MsgToSend();
  msg::IDMsg* pId = new msg::IDMsg;
  pId->set_port(port);
  pToReturn->set_allocated_basicmsg(
    makeBasicMsg(fromId, toId, msg::ProtoType::ID_MSG_ACK, convId));
  pToReturn->set_allocated_newid(pId);
  return pToReturn;
}
