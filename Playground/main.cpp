#include <array>
#include <cstdio>
#include <iostream>
#include <memory>
#include <rapidjson/document.h>
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

void printJson(rapidjson::Document& pDoc)
{
  for (rapidjson::Value::ConstMemberIterator itr = pDoc.MemberBegin();
       itr != pDoc.MemberEnd();
       ++itr)
  {
    switch (itr->value.GetType())
    {
    case 0:
      std::cout << itr->name.GetString() << " is: null" << std::endl;
      break;
    case 1:
      std::cout << itr->name.GetString() << " is: False" << std::endl;
      break;
    case 2:
      std::cout << itr->name.GetString() << " is: True" << std::endl;
      break;
    case 3:
      std::cout << itr->name.GetString() << " is: Object" << std::endl;
      break;
    case 4:
      std::cout << itr->name.GetString() << " is: Array" << std::endl;
      break;
    case 5:
      std::cout << itr->name.GetString() << " is: String" << std::endl;
      break;
    case 6:
      std::cout << itr->name.GetString() << " is: Number" << std::endl;
      break;
    default:
      std::cout << "not anything...? What is going ON!!!!!????" << std::endl;
    }
  }
}

void makeString(rapidjson::Document& pDoc,
                std::string member,
                std::string value)
{
  rapidjson::Value author;
  author.SetString(member.c_str(), member.size(), pDoc.GetAllocator());
  printJson(pDoc);
}

int main()
{
  //  std::cout << exec("dir") << std::endl;


//  printJson(d);

  return 0;
}