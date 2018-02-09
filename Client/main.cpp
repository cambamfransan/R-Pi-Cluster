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
  if(argc==2) addr = "127.0.0.1";
  else addr = argv[1];
  int port;
  if (argc == 3)port = std::stoi(argv[2]);
  else
  {
    std::cout << "What Port is would you like to run on?" << std::endl;
    std::cin >> port;
  }
  Client client(QHostAddress(addr.c_str()), port);

  return a.exec();
}
