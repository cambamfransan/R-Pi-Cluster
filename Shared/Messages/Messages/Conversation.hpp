#ifndef CONVERSATION_H
#define CONVERSATION_H

#include "ProtoFiles/MsgToSend.pb.h"
#include <chrono>

struct Conversation
{
  /** Request message */
  msg::MsgToSend* msg;

  /** Conversation Id */
  int convId;

  /** How long it should take to receive a response */
  std::chrono::seconds timeout;

  /** When it was sent last*/
  std::chrono::steady_clock::time_point timeSend;

  /** Where it is to be sent to */
  int endpointId;
};

#endif