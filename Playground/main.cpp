#include "TCPSender/TCPSender.hpp"
#include <QApplication>
#include <iostream>
#include <thread>

int main(int argc, char* argv[])
{
  QApplication a(argc, argv);

  std::cout << "What port would you like to run on?" << std::endl;
  std::string s;
  std::cin >> s;

  TCPSender tSocket1(QHostAddress("127.0.0.1"), std::stoi(s));

  return a.exec();
}