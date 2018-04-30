#ifndef CLIENT_MANAGER_H
#define CLIENT_MANAGER_H

#include "ExecuteManager.hpp"
#include "Messages/Pi/PiClientManager.hpp"
#include "TCPSender/TCPSenderClient.hpp"
#include "TCPSender/TCPSenderWeb.hpp"
#include <string>
#include <QObject>

namespace manager
{
  class ClientManager : public QObject
  {
    Q_OBJECT
    private slots:
    void sendResults(std::vector<Result>);

signals:
    void sendResultsToClientMain(std::vector<Result>);

  public:
    ClientManager(
        std::shared_ptr<TCPSenderClient> pSenderClient,
                  std::string database);
    ~ClientManager();

    void update(const msg::Update pMsg);

    void execute(std::vector<manager::Task> tasks);

  private:
    std::shared_ptr<TCPSenderClient> m_pSenderClient;
    std::string m_database;
    std::shared_ptr<manager::ExecuteManager> m_pExecuteManager;
    manager::PiClientManager m_piManager;
  };
} // namespace manager

#endif
