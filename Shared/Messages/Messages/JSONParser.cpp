#include "JSONParser.hpp"
#include <iostream>
#include <string>

namespace
{
  const int RAPIDJSON_NULL = 0;
  const int RAPIDJSON_FALSE = 1;
  const int RAPIDJSON_TRUE = 2;
  const int RAPIDJSON_OBJECT = 3;
  const int RAPIDJSON_ARRAY = 4;
  const int RAPIDJSON_STRING = 5;
  const int RAPIDJSON_NUMBER = 6;
} // namespace

json::JSONParser::JSONParser(const rapidjson::Document& doc)
{
  switch (doc.GetType())
  {
  case RAPIDJSON_NULL:
    m_jsonMap["/"] = "null";
    break;
  case RAPIDJSON_FALSE:
    m_jsonMap["/"] = "false";
    break;
  case RAPIDJSON_TRUE:
    m_jsonMap["/"] = "true";
    break;
  case RAPIDJSON_OBJECT:
    convertJSONObject(doc, "");
    break;
  case RAPIDJSON_ARRAY:
    convertJSONArray(doc, "");
    break;
  case RAPIDJSON_STRING:
    m_jsonMap["/"] = doc.GetString();
    break;
  case RAPIDJSON_NUMBER:
    m_jsonMap["/"] = std::to_string(doc.GetInt64());
    break;
  default:
    std::cout << "Bad things..." << std::endl;
    break;
  }
}

void json::JSONParser::convertJSONArray(const rapidjson::Value& arr,
                                        const std::string& path)
{
  int i(0);
  for (auto& v : arr.GetArray())
  {
    switch (v.GetType())
    {
    case RAPIDJSON_NULL:
      m_jsonMap[path + "/" + std::to_string(i)] = "null";
      break;
    case RAPIDJSON_FALSE:
      m_jsonMap[path + "/" + std::to_string(i)] = "false";
      break;
    case RAPIDJSON_TRUE:
      m_jsonMap[path + "/" + std::to_string(i)] = "true";
      break;
    case RAPIDJSON_OBJECT:
      convertJSONObject(v, path + "/" + std::to_string(i));
      break;
    case RAPIDJSON_ARRAY:
      convertJSONArray(v, path + "/" + std::to_string(i));
      break;
    case RAPIDJSON_STRING:
      m_jsonMap[path + "/" + std::to_string(i)] = v.GetString();;
      break;
    case RAPIDJSON_NUMBER:
      m_jsonMap[path + "/" + std::to_string(i)] = std::to_string(v.GetInt64());
      break;
    default:
      std::cout << "Error in Array" << std::endl;
      break;
    }
    i++;
  }
}

void json::JSONParser::convertJSONObject(const rapidjson::Value& obj,
                                         const std::string& path)
{
  for (auto& m : obj.GetObject())
  {
    switch (m.value.GetType())
    {
    case RAPIDJSON_NULL:
      m_jsonMap[path + "/" + m.name.GetString()] = "null";
      break;
    case RAPIDJSON_FALSE:
      m_jsonMap[path + "/" + m.name.GetString()] = "false";
      break;
    case RAPIDJSON_TRUE:
      m_jsonMap[path + "/" + m.name.GetString()] = "true";
      break;
    case RAPIDJSON_OBJECT:
      convertJSONObject(m.value, path + "/" + m.name.GetString());
      break;
    case RAPIDJSON_ARRAY:
      convertJSONArray(m.value, path + "/" + m.name.GetString());
      break;
    case RAPIDJSON_STRING:
      m_jsonMap[path + "/" + m.name.GetString()] = m.value.GetString();
      break;
    case RAPIDJSON_NUMBER:
      m_jsonMap[path + "/" + m.name.GetString()] = std::to_string(m.value.GetInt64());
      break;
    default:
      std::cout << "Error in Object" << std::endl;
      break;
    }
  }
}

std::map<std::string, std::string> json::JSONParser::getMap()
{
  return m_jsonMap;
}
