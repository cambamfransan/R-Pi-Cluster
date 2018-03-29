#ifndef PI_CLIENT_MANAGER_H
#define PI_CLIENT_MANAGER_H

#include <string>
#include "Pi.hpp"
#include "ProtoFiles/Update.pb.h"

namespace manager
{
  class PiClientManager
  {
  public:
    PiClientManager(std::string database);
    ~PiClientManager();

    void addPi(Pi pi);
    void removePi(int id);
    void modifyPi(int id, std::string field, std::string value);

  private:
    std::string m_piFile;
  };

} // namespace manager

#endif
