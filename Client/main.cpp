#include "ClientLib/Client.hpp"
#include <iostream>
#include <qapplication.h>
#include <qhostaddress.h>

int main(int argc, char* argv[])
{
  QApplication a(argc, argv);

  std::string addr("127.0.0.1");
  std::cout << "What Port is would you like to run on?" << std::endl;
  std::string s;
  std::cin >> s;
  //Client client(QHostAddress(addr.c_str()), std::stoi(s));

  return a.exec();
}