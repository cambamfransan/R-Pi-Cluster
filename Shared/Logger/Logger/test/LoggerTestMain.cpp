#include <QtTest>
#include "Logger/Logger.hpp"
#include <fstream>
#include <cstdio>
#include <filesystem>

class LoggerTest : public QObject
{
  Q_OBJECT

public:
  LoggerTest();
  ~LoggerTest();

  private slots:
  void initTestCase();
  void cleanupTestCase();
  void debug();
  void info();
  void error();
};

LoggerTest::LoggerTest()
{
}

LoggerTest::~LoggerTest()
{
}

void LoggerTest::initTestCase()
{
}

void LoggerTest::cleanupTestCase()
{
}

void LoggerTest::debug()
{
  auto file = Logger::init("first");
  Logger::debug("This is a debug Message");
  Logger::debug("This is a debug Message1");
  Logger::debug("This is a debug Message2");
  Logger::debug("This is a debug Message3");
  std::ifstream fin(file);
  QVERIFY(!fin.fail());
}

void LoggerTest::info()
{
  auto file = Logger::init("second");
  Logger::info("This is a info Message");
  Logger::info("This is a info Message1");
  Logger::info("This is a info Message2");
  Logger::info("This is a info Message3");
  std::ifstream fin(file);
  QVERIFY(!fin.fail());
}

void LoggerTest::error()
{
  auto file = Logger::init("third");
  Logger::error("This is a error Message");
  Logger::error("This is a error Message1");
  Logger::error("This is a error Message2");
  Logger::error("This is a error Message3");
  std::ifstream fin(file);
  QVERIFY(!fin.fail());
}

QTEST_APPLESS_MAIN(LoggerTest)

#include "LoggerTestMain.moc"
