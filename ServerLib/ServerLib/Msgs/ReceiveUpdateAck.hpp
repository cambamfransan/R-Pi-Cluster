#ifndef SERVER_RECEIVE_HEART_BEAT_H
#define SERVER_RECEIVE_HEART_BEAT_H

#include "ProtoFiles/MsgToSend.pb.h"
#include "qrunnable.h"
#include <mutex>

namespace receive_msgs
{
  class UpdateAckTask : public QRunnable
  {
  public:
    UpdateAckTask(
      msg::MsgToSend,
      std::shared_ptr<std::mutex>,
      std::shared_ptr<std::map<int, std::chrono::steady_clock::time_point>>);

  private:
    void run();

    msg::MsgToSend m_msg;
    std::shared_ptr<std::mutex> m_pMutex;
    std::shared_ptr<std::map<int, std::chrono::steady_clock::time_point>>
      m_pClientMap;
  };
} // namespace receive_msgs

#endif
