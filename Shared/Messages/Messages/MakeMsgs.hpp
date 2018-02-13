#ifndef MAKE_MSGS_H
#define MAKE_MSGS_H

#include "ProtoFiles/MsgToSend.pb.h"

namespace make_msgs
{
  msg::MsgToSend* makeTestMsg(
    int fromId, int toId, int convId, std::string msg);

  msg::MsgToSend* makeTaskMsg(int fromId, int toId, int convId, std::string msg);

  msg::BasicMsg* makeBasicMsg(int fromId, int toId, int msgType, int convId);

  msg::MsgToSend* makeBasicMsgToSend(int fromId, int toId, int msgType, int convId);
} // namespace make_msgs

#endif