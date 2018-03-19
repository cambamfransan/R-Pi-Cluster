#include "ClientLib/Client.hpp"
#include <iostream>
#include <qapplication.h>
#include <qhostaddress.h>
#include "Logger/Logger.hpp"

int main(int argc, char* argv[])
{
  QApplication a(argc, argv);

  Logger::init("Client");
  std::string addr;
  if(argc==1) addr = "127.0.0.1";
  else addr = argv[1];
  int port;
  if (argc > 2)port = std::stoi(argv[2]);
  else
  {
    std::cout << "What Port is would you like to run on?" << std::endl;
    std::cin >> port;
  }
  std::string arg;
  if (argc == 4) arg = argv[3];
  Client client(QHostAddress(addr.c_str()), port, arg);

  return a.exec();
}
