#ifndef MAKE_MSGS_H
#define MAKE_MSGS_H

#include "Manager/JobManager.hpp"
//#include "Manager/Results.hpp"
#include "Pi/Pi.hpp"
#include "Pi/PiServerManager.hpp"
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
                              std::vector<manager::Task> msg);

  msg::BasicMsg* makeBasicMsg(int fromId, int toId, int msgType, int convId);

  msg::MsgToSend* makeBasicMsgToSend(int fromId,
                                     int toId,
                                     int msgType,
                                     int convId);

  msg::MsgToSend* makeUpdateMsg(
    int fromId,
    int toId,
    int convId,
    manager::PiUpdateStruct pis,
    manager::JobUpdateStruct jobs);

  msg::MsgToSend* makeIdMsg(int fromId, int toId, int convId);

  msg::MsgToSend* makeIdMsgAck(int fromId, int toId, int convId);

  msg::MsgToSend* makeResultsMsg(int fromId, 
                                 int toId,
                                 int convId,
                                 std::vector<manager::Result> results);

  msg::MsgToSend* makeCurrentStateMsg(int fromId,
      int toId,
      int convId,
      std::vector<manager::JobInfo> jobInfo,
      std::vector<manager::Pi> pis,
      std::vector<std::string> results,
      int resultId);

} // namespace make_msgs

#endif
