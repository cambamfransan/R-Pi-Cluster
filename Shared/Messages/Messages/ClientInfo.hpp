#ifndef CLIENT_INFO_H
#define CLIENT_INFO_H

#include <string>

struct ClientInfo
{
  /** Ip Address of the client */
  std::string ipAddress;

  /** port of the clinet */
  int port;

  /** username on the pi */
  std::string username;

  /** password for the username */
  std::string password;

  /** What priority in line */
  int priority;
  
  /** The clients Id*/
  int clientId;

  /** Port the TCP Server is running on */
  int serverPort;
};

#endif