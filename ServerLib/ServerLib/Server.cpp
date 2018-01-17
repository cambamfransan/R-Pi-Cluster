#include "Server.hpp"

Server::Server()
  : m_sender(QHostAddress("0.0.0.0"))
{

}

Server::~Server()
{

}

void Server::send(std::string msg)
{
  m_sender.send(msg, 1, std::chrono::seconds(1), false, QHostAddress("127.0.0.1"), 0);
}
