#include "MakeJSONMsgs.hpp"

#include <rapidjson/document.h>
#include "JSONHelpers.hpp"
#include <map>
#include <iostream>

std::string json::makeJsonHeartbeat(int convId)
{
  rapidjson::Document d;
  d.SetObject();
  json::addStringToDoc(d, std::string("MsgType"), "Heartbeat");
  json::addStringToDoc(d, std::string("convId"), std::to_string(convId));
  std::string msg(json::jsonToString(d));
  //std::cout << "HeartbeatToDeliver: " << msg << " Size: " << msg.size() << std::endl;
  return msg;
}
