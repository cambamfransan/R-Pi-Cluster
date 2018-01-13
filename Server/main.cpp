#include <iostream>
#include <thread>
#include "TCPSender/TCPSender.hpp"
#include "ServerLib/Server.hpp"
#include <QApplication>
#include <qtimer.h>

int main(int argc, char* argv[])
{
  QApplication a(argc, argv);
  
  Server server;

  //for (int i = 0; i < 5; i++)
  //{
  //  std::cout << "What would you like to send?" << std::endl;
  //  std::string s;
  //  std::cin >> s;
  //  server.send(s);
  //}

  return a.exec();
}