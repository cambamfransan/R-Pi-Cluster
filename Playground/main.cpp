//#include "ProtoFiles/Results.pb.h"
#include "ProtoFiles/TaskMsg.pb.h"
#include <array>
#include <chrono>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>
#include <rapidjson/document.h>
#include <stdexcept>
#include <string>

namespace
{
  const std::string utask("Calculate pi now");
}

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

/*
int main()
{
  std::ofstream fout("ProtoTimeResults.txt");
  for (int i = 0; i < 10000; i++)
  {
    msg::TaskMsg* taskFile = new msg::TaskMsg();;

    auto start = std::chrono::steady_clock::now();
    // Read the existing address book.
    std::fstream input("tasks.txt", std::ios::in | std::ios::binary);
    if (!input)
    {
      std::cout << "test"
                << ": File not found.  Creating a new file." << std::endl;
    }
    else if (!taskFile->ParseFromIstream(&input))
    {
      std::cerr << "Failed to parse address book." << std::endl;
      return -1;
    }

    //for (int i = 0; i < 10000; i++)
    //{
    //  taskFile->add_toexecute(utask);
    //}

   taskFile->mutable_toexecute()->RemoveLast();

    std::fstream output(
      "tasks.txt", std::ios::out | std::ios::trunc | std::ios::binary);
    if (!taskFile->SerializeToOstream(&output))
    {
      std::cerr << "Failed to write address book." << std::endl;
      return -1;
    }

    auto stop = std::chrono::steady_clock::now();

    fout << "It took: "
         << std::chrono::duration_cast<std::chrono::microseconds>(stop - start)
              .count()
         << std::endl;
  

  }
  return 0;
}
*/
int main()
{
  std::ofstream fout("FStreamResults.txt");
  for (int i = 0; i < 10000; i++)
  {
    auto start = std::chrono::steady_clock::now();
    // Read the existing address book.
    std::ifstream input("tasks.txt");
    if (!input)
    {
      std::cout << "test"
                << ": File not found.  Creating a new file." << std::endl;
    }
    std::string str((std::istreambuf_iterator<char>(input)),
                    std::istreambuf_iterator<char>());
    input.close();

    str.erase(0, str.find(",")+1);

    std::ofstream output("tasks.txt");
    output << str;

    auto stop = std::chrono::steady_clock::now();

    fout << "It took: "
         << std::chrono::duration_cast<std::chrono::microseconds>(stop - start)
              .count()
         << std::endl;
  }
  return 0;
}
