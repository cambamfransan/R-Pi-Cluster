#include <iostream>
#include <boost/filesystem/path.hpp>

int main()
{
  boost::filesystem::path path("myFile.txt");
  std::cout << path.string() << std::endl;
  return 0;
}