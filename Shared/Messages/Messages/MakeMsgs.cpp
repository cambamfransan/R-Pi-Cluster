#include "MakeMsgs.hpp"

msg::MsgToSend* make_msgs::makeTestMsg(int fromId, int toId, int convId, std::string msg)
{
  msg::MsgToSend* pToReturn = new msg::MsgToSend();
  pToReturn->mutable_test()->add_teststring(msg);
  pToReturn->set_allocated_basicmsg(makeBasicMsg(fromId, toId, msg::ProtoType::TEST_MSG, convId));
  return pToReturn;
}

msg::MsgToSend* make_msgs::makeTaskMsg(int fromId, int toId, int convId, std::string msg)
{
  msg::MsgToSend* pToReturn = new msg::MsgToSend();
  pToReturn->mutable_task()->add_toexecute(msg);
  pToReturn->set_allocated_basicmsg(makeBasicMsg(fromId, toId, msg::ProtoType::TASK_MSG, convId));
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

msg::MsgToSend* make_msgs::makeBasicMsgToSend(int fromId, int toId, int msgType, int convId)
{
  msg::MsgToSend* pToReturn = new msg::MsgToSend();
  pToReturn->set_allocated_basicmsg(makeBasicMsg(fromId, toId, msgType, convId));
  return pToReturn;
}
