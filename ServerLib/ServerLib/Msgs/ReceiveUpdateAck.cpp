#include "ReceiveUpdateAck.hpp"
#include "Logger/Logger.hpp"

receive_msgs::UpdateAckTask::UpdateAckTask(
  msg::MsgToSend msg,
  std::shared_ptr<std::mutex> pMutex,
  std::shared_ptr<std::map<int, std::chrono::steady_clock::time_point>> pMap)
  : QRunnable(), m_msg(msg), m_pMutex(pMutex), m_pClientMap(pMap)
{
}

void receive_msgs::UpdateAckTask::run()
{
  Logger::info("received heartbeat");
  {
    if (m_pMutex)
    {
      std::lock_guard<std::mutex> gaurd(*m_pMutex);
      auto itr = m_pClientMap->find(m_msg.basicmsg().fromid());
      if (itr != m_pClientMap->end())
      {
        itr->second = std::chrono::steady_clock::now();
      }
    }
  }
  Logger::info("finished heartbeat");
}
