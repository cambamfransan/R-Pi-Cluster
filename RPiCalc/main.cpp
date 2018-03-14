#include "ComputePi.hpp"
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    return 0;
  }
  int number;
  try
  {
    number = std::stoi(argv[1]);
  }
  catch (...)
  {
    return 0;
  }
  std::cout << pi::computeDigit(number) << std::endl;

  return 0;
}
