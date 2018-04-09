#include "MakeJSONMsgs.hpp"

#include <rapidjson/document.h>
#include "JSONHelpers.hpp"
#include <map>
#include <iostream>
#include <string>

namespace
{
  const std::string MSG_TYPE = "MsgType";
  const std::string JOB_ID = "JobId";
  const std::string CONV_ID = "convId";
  const std::string REMOTE = "remote";
  const std::string NAME = "name";
  const std::string PRIORITY = "priority";
  const std::string TASK_PER_BUNDLE = "taskPerBundle";

}

std::string json::makeJsonHeartbeat(int convId)
{
  rapidjson::Document d;
  d.SetObject();
  json::addStringToDoc(d, MSG_TYPE, "Heartbeat");
  json::addIntToDoc(d, CONV_ID, convId);
  std::string msg(json::jsonToString(d));
  //std::cout << "HeartbeatToDeliver: " << msg << " Size: " << msg.size() << std::endl;
  return msg;
}

std::string json::makeJsonAddJobAck(int convId, int jobId, std::string remote, std::string name, int pri, int tpb)
{
  rapidjson::Document d;
  d.SetObject();
  json::addStringToDoc(d, MSG_TYPE, "AddJobAck");
  json::addIntToDoc(d, JOB_ID, jobId);
  json::addIntToDoc(d, CONV_ID, convId);
  json::addStringToDoc(d, REMOTE, remote);
  json::addStringToDoc(d, NAME, name);
  json::addIntToDoc(d, PRIORITY, pri);
  json::addIntToDoc(d, TASK_PER_BUNDLE, tpb);
  std::string msg(json::jsonToString(d));
  return msg;
}
