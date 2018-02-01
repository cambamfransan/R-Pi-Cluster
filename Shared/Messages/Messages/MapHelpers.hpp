#ifndef TCPHELPERS_H
#define TCPHELPERS_H

#include "ProtoFiles/MsgToSend.pb.h"
#include <math.h>

namespace make_msgs
{
  inline int getMapId(int pId, int cId)
  {
    return pId *(pow(10, (trunc(log10(cId))+1))) + cId;
  }
}

#endif