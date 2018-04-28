#ifndef MAKE_JSON_MSGS_H
#define MAKE_JSON_MSGS_H

#include <string>
#include <map>

namespace json
{
  std::string makeJsonHeartbeat(int convId);

  std::string makeJsonAddJobAck(int convId, int jobId, std::string remote, std::string name, int pri, int tpb);

  std::string makeJsonProgress(int convId, std::map<int, int> progress);

  std::string makeJsonNewClient(int convId, int clientId, std::string ip);

  std::string makeJsonLostClient(int convId, int clientId);

} // namespace json

#endif
