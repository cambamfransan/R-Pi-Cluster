#ifndef MAKE_JSON_MSGS_H
#define MAKE_JSON_MSGS_H

#include <string>

namespace json
{
  std::string makeJsonHeartbeat(int convId);

  std::string makeJsonAddJobAck(int convId, int jobId, std::string remote, std::string name, int pri, int tpb);

} // namespace json

#endif