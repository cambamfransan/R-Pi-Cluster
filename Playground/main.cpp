#include <array>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

#ifdef __unix || __APPLE__
std::string exec(const char* cmd)
{
  std::array<char, 128> buffer;
  std::string result;
  std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
  if (!pipe) throw std::runtime_error("popen() failed!");
  while (!feof(pipe.get()))
  {
    if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
      result += buffer.data();
  }
  return result;
}
#else
std::string exec(const char* cmd)
{
  std::array<char, 128> buffer;
  std::string result;
  std::shared_ptr<FILE> pipe(_popen(cmd, "r"), _pclose);
  if (!pipe) throw std::runtime_error("_popen() failed!");
  while (!feof(pipe.get()))
  {
    if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
      result += buffer.data();
  }
  return result;
}

#endif

int main()
{
//  std::cout << exec("dir") << std::endl;
#if (TESTING_GUIS == 0)
  std::cout << "Build without Guis" << std::endl;
#else
  std::cout << "Build with Guis" << std::endl;
#endif

  return 0;
}