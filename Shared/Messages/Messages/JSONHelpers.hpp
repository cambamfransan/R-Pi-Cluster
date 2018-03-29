#ifndef JSON_HELPERS_H
#define JSON_HELPERS_H

#include <map>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

namespace json
{
  std::string jsonToString(const rapidjson::Document& doc);

  void addNullToDoc(rapidjson::Document& doc, const std::string& member);

  void addBoolToDoc(rapidjson::Document& doc,
                    const std::string& member,
                    bool value);

  void addObjectToDoc(rapidjson::Document& doc,
                      const std::string& member,
                      rapidjson::Value& value);

  void addArrayToDoc(rapidjson::Document& doc,
                     const std::string& member,
                     rapidjson::Value& value);

  void addStringToDoc(rapidjson::Document& doc,
                      const std::string& member,
                      const std::string& value);

  void addIntToDoc(rapidjson::Document& doc, std::string& member, int value);

  // Add to Objects
  void addNullToObject(rapidjson::Document& doc,
                       rapidjson::Value& obj,
                       const std::string& member);

  void addBoolToObject(rapidjson::Document& doc,
                       rapidjson::Value& obj,
                       const std::string& member,
                       bool value);

  void addObjectToObject(rapidjson::Document& doc,
                         rapidjson::Value& obj,
                         const std::string& member,
                         rapidjson::Value& value);

  void addArrayToObject(rapidjson::Document& doc,
                        rapidjson::Value& obj,
                        const std::string& member,
                        rapidjson::Value& value);

  void addStringToObject(rapidjson::Document& doc,
                         rapidjson::Value& obj,
                         const std::string& member,
                         const std::string& value);

  void addIntToObject(rapidjson::Document& doc,
                      rapidjson::Value& obj,
                      std::string& member,
                      int value);

  void addNullToArray(rapidjson::Document& doc, rapidjson::Value& arr);

  void addBoolToArray(rapidjson::Document& doc,
                      rapidjson::Value& arr,
                      bool value);

  void addObjectToArray(rapidjson::Document& doc,
                        rapidjson::Value& arr,
                        rapidjson::Value& value);

  void addArrayToArray(rapidjson::Document& doc,
                       rapidjson::Value& arr,
                       rapidjson::Value& value);

  void addStringToArray(rapidjson::Document& doc,
                        rapidjson::Value& arr,
                        const std::string& value);

  void addIntToArray(rapidjson::Document& doc,
                     rapidjson::Value& arr,
                     int value);

} // namespace json

#endif