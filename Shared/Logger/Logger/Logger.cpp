#include "Logger.hpp"
#include <QDateTime>
#include <fstream>
#include <ctime>
#include <chrono>
#include <iostream>
#include <QDebug>
#include <sstream>
#include <iomanip> // put_time

namespace
{
  inline std::string getTime()
  {
    auto in_time_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %I-%M-%S");
    return ss.str();
  }
  const std::string DEBUG = " [DEBUG]: ";
    const std::string ERROR = " [ERROR]: ";
    const std::string INFO = " [INFO]: ";
}

Logger::Logger()
{

}

std::string Logger::filename = "";

std::string Logger::init(const std::string& s)
{
  filename = s + "_" + getTime() + ".log";
  std::ofstream fout(filename);
  fout << "" << std::endl;
  system("cd");
  fout.close();
  return filename;
}

void Logger::debug(std::string msg)
{
  if (filename == "") return;
  std::ofstream fout;
  fout.open(filename, std::ofstream::out | std::ofstream::app);
  std::string message = getTime() + DEBUG + msg + '\n';
  fout << message;
  fout.close();
}

void Logger::info(std::string msg)
{
  if (filename == "") return;
  std::ofstream fout;
  fout.open(filename, std::ofstream::out | std::ofstream::app);
  std::string message = getTime() + INFO + msg + '\n';
  fout << message;
  fout.close();
}

void Logger::error(std::string msg)
{
  if (filename == "") return;
  std::ofstream fout;
  fout.open(filename, std::ofstream::out | std::ofstream::app);
  std::string message = getTime() + ERROR + msg + '\n';
  fout << message;
  fout.close();
}