#ifndef TCPHELPERS_H
#define TCPHELPERS_H

#include "ProtoFiles/MsgToSend.pb.h"
#include <math.h>

namespace tcp
{
  inline int getMapId(int pId, int cId)
  {
    return pId*log10(cId) + cId;
  }
}

#endif