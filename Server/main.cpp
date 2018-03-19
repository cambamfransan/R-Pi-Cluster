#include <iostream>
#include <thread>
#include "ServerLib/Server.hpp"
#include <QApplication>
#include <qtimer.h>
#include "Logger/Logger.hpp"

int main(int argc, char* argv[])
{
  QApplication a(argc, argv);
  Logger::init("Server");
  std::string arg;
  if (argc == 1)
    arg = "";
  else arg = argv[1];
    
  Server server(arg);

  return a.exec();
}