#include <iostream>
#include <thread>
#include "TCPSender/TCPSender.hpp"
#include <QApplication>
#include <qtimer.h>

int main(int argc, char* argv[])
{
  QApplication a(argc, argv);
  
  TCPSender tSocket(QHostAddress("0.0.0.0"));

  std::cout << "Hello World" << std::endl;
  return a.exec();
}