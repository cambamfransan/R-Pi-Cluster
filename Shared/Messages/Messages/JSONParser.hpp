#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <map>
#include <rapidjson/document.h>
#include <rapidjson/rapidjson.h>

namespace json
{
  class JSONParser
  {
  public:
    JSONParser() {}
    JSONParser(const rapidjson::Document& doc);

    void convertJSONArray(const rapidjson::Value& arr,
      const std::string& path);

    void convertJSONObject(const rapidjson::Value& obj,
                           const std::string& path);

    std::map<std::string, std::string> getMap();

  private:
    std::map<std::string, std::string> m_jsonMap;
  };
} // namespace json

#endif