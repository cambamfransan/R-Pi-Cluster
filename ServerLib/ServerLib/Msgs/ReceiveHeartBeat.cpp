#include "ReceiveHeartBeat.hpp"

  receive_msgs::HeartBeatTask::HeartBeatTask(
    msg::MsgToSend msg,
    std::shared_ptr<std::mutex> pMutex,
    std::shared_ptr<std::map<int, std::chrono::steady_clock::time_point>> pMap)
    : QRunnable(), m_msg(msg), m_pMutex(pMutex), m_pClientMap(pMap)
  {
  }

  void receive_msgs::HeartBeatTask::run()
  {
    std::lock_guard<std::mutex> gaurd(*m_pMutex);
    m_pClientMap->at(m_msg.basicmsg().fromid()) = std::chrono::steady_clock::now();
  }
