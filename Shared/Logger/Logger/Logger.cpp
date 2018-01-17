#include "Logger.hpp"
#include <QDateTime>
#include <fstream>
#include <ctime>
#include <chrono>

#include <QDebug>

Logger::Logger()
{

}

std::string Logger::filename = "";

void Logger::init(std::string i)
{
  //std::string date = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss").toStdString();
  //filename = "GameClient_" + date + ".log";
  filename = i + "Thisrun.log";
  std::ofstream fout;
  fout.open(filename);
  fout << "" << std::endl;
  fout.close();

  qDebug() << QString::fromStdString(filename);
}

void Logger::debug(std::string msg)
{
  if (filename == "") return;
  std::ofstream fout;
  fout.open(filename, std::ofstream::out | std::ofstream::app);
  std::string time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss,zzz").toStdString();
  std::string type = " [DEBUG]: ";
  std::string message = time + type + msg + '\n';
  fout << message;
  fout.close();
}

void Logger::info(std::string msg)
{
  if (filename == "") return;
  std::ofstream fout;
  fout.open(filename, std::ofstream::out | std::ofstream::app);
  std::string time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss,zzz").toStdString();
  std::string type = " [INFO]: ";
  std::string message = time + type + msg + '\n';
  fout << message;
  fout.close();
}

void Logger::error(std::string msg)
{
  if (filename == "") return;
  std::ofstream fout;
  fout.open(filename, std::ofstream::out | std::ofstream::app);
  std::string time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss,zzz").toStdString();
  std::string type = " [ERROR]: ";
  std::string message = time + type + msg + '\n';
  fout << message;
  fout.close();
}