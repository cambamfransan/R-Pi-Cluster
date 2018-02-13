#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>

class Logger
{
public:
  Logger();
  static std::string init(const std::string& s);
  static void debug(std::string msg);
  static void info(std::string msg);
  static void error(std::string msg);

  static std::string filename;
};

#endif // LOGGER_HPP