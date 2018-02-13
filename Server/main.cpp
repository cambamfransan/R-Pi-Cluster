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
  Server server;

  return a.exec();
}