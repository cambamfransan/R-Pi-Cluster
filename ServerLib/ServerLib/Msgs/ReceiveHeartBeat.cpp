#include "ReceiveHeartBeat.hpp"
#include "Logger/Logger.hpp"

receive_msgs::HeartBeatTask::HeartBeatTask(
  msg::MsgToSend msg,
  std::shared_ptr<std::mutex> pMutex,
  std::shared_ptr<std::map<int, std::chrono::steady_clock::time_point>> pMap)
  : QRunnable(), m_msg(msg), m_pMutex(pMutex), m_pClientMap(pMap)
{
}

void receive_msgs::HeartBeatTask::run()
{
  Logger::info("received heartbeat");
  {
    if (m_pMutex)
    {
      std::lock_guard<std::mutex> gaurd(*m_pMutex);
      std::cout << "good mutex" << std::endl;
      std::cout << m_msg.DebugString() << std::endl;
      m_pClientMap->at(m_msg.basicmsg().fromid()) =
        std::chrono::steady_clock::now();
      std::cout << "Finished" << std::endl;
    }
  }
  Logger::info("finished heartbeat");
}
