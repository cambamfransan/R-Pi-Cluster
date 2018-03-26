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
