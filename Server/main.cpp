#include <iostream>
#include <thread>
#include "ServerLib/Server.hpp"
#include <QApplication>
#include <qtimer.h>

int main(int argc, char* argv[])
{
  QApplication a(argc, argv);
  
  Server server;

  return a.exec();
}