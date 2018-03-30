#include "PiClientManager.hpp"
#include <fstream>

namespace
{
  const std::string PI_PRIORITY("Priority");
  const std::string PI_THREADS("Threads");
} // namespace

manager::PiClientManager::PiClientManager(std::string database)
  : m_piFile(database + "PiFile.txt")
{
  // Do i need to check for ending slash
}

manager::PiClientManager::~PiClientManager()
{
  // Delete file
}

void manager::PiClientManager::addPi(Pi pi)
{
  std::ofstream fout(m_piFile, std::fstream::app);
  fout << pi.toString() << std::endl;
  fout.close();
}

void manager::PiClientManager::removePi(int id)
{
  std::ifstream fin(m_piFile);
  std::string nextLine;
  std::string toWrite;
  std::string idString(std::to_string(id));
  while (!fin.eof())
  {
    std::getline(fin, nextLine);
    size_t spot = nextLine.find_last_of("~");
    if (nextLine.substr(spot + 1, nextLine.size()) != idString)
    {
      toWrite.append(nextLine);
    }
  }
  fin.close();
  std::ofstream fout(m_piFile);
  fout << toWrite << std::endl;
  fout.close();
}

void manager::PiClientManager::modifyPi(int id,
                                        std::string field,
                                        std::string value)
{
  std::ifstream fin(m_piFile);
  std::string nextLine;
  std::string toWrite;
  std::string idString(std::to_string(id));
  while (!fin.eof())
  {
    std::getline(fin, nextLine);
    size_t spot = nextLine.find_last_of("~");
    if (nextLine.substr(spot + 1, nextLine.size()) == idString)
    {
      manager::Pi pi = manager::Pi::fromString(nextLine);
      if (field == PI_PRIORITY) pi.changePriority(std::stoi(value));
      else if (field == PI_THREADS) pi.changeThreads(std::stoi(value));
        
      toWrite.append(pi.toString());
      continue;
    }
    toWrite.append(nextLine);
  }
  fin.close();
  std::ofstream fout(m_piFile);
  fout << toWrite << std::endl;
  fout.close();
}
