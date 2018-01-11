#include <iostream>
#include <thread>
#include "TCPSender/TCPSender.hpp"
#include "TCPSender/Listener.hpp"
#include <QApplication>

int main(int argc, char* argv[])
{
  QApplication a(argc, argv);

  TCPListener tSocket1(QHostAddress("127.0.0.1"), 5000);

  std::cout << "Hello World" << std::endl;
  return a.exec();
}