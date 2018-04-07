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
  json::addIntToDoc(d, std::string("convId"), convId);
  std::string msg(json::jsonToString(d));
  //std::cout << "HeartbeatToDeliver: " << msg << " Size: " << msg.size() << std::endl;
  return msg;
}

std::string json::makeJsonAddJobAck(int convId, int jobId, std::string remote, std::string name, int pri, int tpb)
{
  rapidjson::Document d;
  d.SetObject();
  json::addStringToDoc(d, std::string("MsgType"), "AddJobAck");
  json::addIntToDoc(d, std::string("JobId"), jobId);
  json::addIntToDoc(d, std::string("convId"), convId);
  json::addStringToDoc(d, std::string("remote"), remote);
  json::addStringToDoc(d, std::string("name"), name);
  json::addIntToDoc(d, std::string("priority"), pri);
  json::addIntToDoc(d, std::string("taskPerBundle"), tpb);
  std::string msg(json::jsonToString(d));
  return msg;
}
