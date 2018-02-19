#ifndef MAKE_MSGS_H
#define MAKE_MSGS_H

#include "ClientInfo.hpp"
#include "ProtoFiles/MsgToSend.pb.h"
#include <vector>

namespace make_msgs
{
  msg::MsgToSend* makeTestMsg(int fromId,
                              int toId,
                              int convId,
                              std::string msg);

  msg::MsgToSend* makeTaskMsg(int fromId,
                              int toId,
                              int convId,
                              std::string msg);

  msg::BasicMsg* makeBasicMsg(int fromId, int toId, int msgType, int convId);

  msg::MsgToSend* makeBasicMsgToSend(int fromId,
                                     int toId,
                                     int msgType,
                                     int convId);

  msg::MsgToSend* makeUpdateMsg(int fromId,
                                int toId,
                                int msgType,
                                int convId,
                                std::vector<ClientInfo> clients);

  msg::MsgToSend* makeIdMsg(int fromId,
                            int toId,
                            int convId,
                            std::string ipAddress,
                            int port);

} // namespace make_msgs

#endif