#include "JSONHelpers.hpp"

std::string json::jsonToString(const rapidjson::Document& doc)
{
  rapidjson::StringBuffer buffer;

  buffer.Clear();

  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  doc.Accept(writer);

  return std::string(buffer.GetString());
}


void json::addNullToDoc(rapidjson::Document& doc, const std::string& member)
{
  rapidjson::Value rapidValue;
  rapidjson::Value index(member.c_str(), static_cast<int>(member.size()), doc.GetAllocator());
  doc.AddMember(index, rapidValue, doc.GetAllocator());
}

void json::addBoolToDoc(rapidjson::Document& doc,
                        const std::string& member,
                        bool value)
{
  rapidjson::Value rapidValue(value);
  rapidjson::Value index(member.c_str(), static_cast<int>(member.size()), doc.GetAllocator());
  doc.AddMember(index, rapidValue, doc.GetAllocator());
}

void json::addObjectToDoc(rapidjson::Document& doc,
                          const std::string& member,
                          rapidjson::Value& value)
{
  rapidjson::Value rapidValue(rapidjson::kObjectType);
  rapidValue = value;
  rapidjson::Value index(member.c_str(), static_cast<int>(member.size()), doc.GetAllocator());
  doc.AddMember(index, rapidValue, doc.GetAllocator());
}

void json::addArrayToDoc(rapidjson::Document& doc,
                         const std::string& member,
                         rapidjson::Value& value)
{
  rapidjson::Value rapidValue(rapidjson::kArrayType);
  rapidValue = value;
  rapidjson::Value index(member.c_str(), static_cast<int>(member.size()), doc.GetAllocator());
  doc.AddMember(index, rapidValue, doc.GetAllocator());
}

void json::addStringToDoc(rapidjson::Document& doc,
                          const std::string& member,
                          const std::string& value)
{
  rapidjson::Value rapidValue(value.c_str(), static_cast<int>(value.size()), doc.GetAllocator());
  rapidjson::Value index(member.c_str(), static_cast<int>(member.size()), doc.GetAllocator());
  doc.AddMember(index, rapidValue, doc.GetAllocator());
}

void json::addIntToDoc(rapidjson::Document& doc, std::string& member, int value)
{
  rapidjson::Value rapidValue;
  rapidValue.SetInt(value);
  rapidjson::Value index(member.c_str(), static_cast<int>(member.size()), doc.GetAllocator());
  doc.AddMember(index, rapidValue, doc.GetAllocator());
}

void json::addNullToObject(rapidjson::Document& doc,
                           rapidjson::Value& obj,
                           const std::string& member)
{
  rapidjson::Value rapidValue;
  rapidjson::Value index(member.c_str(), static_cast<int>(member.size()), doc.GetAllocator());
  obj.AddMember(index, rapidValue, doc.GetAllocator());
}

void json::addBoolToObject(rapidjson::Document& doc,
                           rapidjson::Value& obj,
                           const std::string& member,
                           bool value)
{
  rapidjson::Value rapidValue(value);
  rapidjson::Value index(member.c_str(), static_cast<int>(member.size()), doc.GetAllocator());
  obj.AddMember(index, rapidValue, doc.GetAllocator());
}

void json::addObjectToObject(rapidjson::Document& doc,
                             rapidjson::Value& obj,
                             const std::string& member,
                             rapidjson::Value& value)
{
  rapidjson::Value rapidValue(rapidjson::kObjectType);
  rapidValue = value;
  rapidjson::Value index(member.c_str(), static_cast<int>(member.size()), doc.GetAllocator());
  obj.AddMember(index, rapidValue, doc.GetAllocator());
}

void json::addArrayToObject(rapidjson::Document& doc,
                            rapidjson::Value& obj,
                            const std::string& member,
                            rapidjson::Value& value)
{
  rapidjson::Value rapidValue(rapidjson::kArrayType);
  rapidValue = value;
  rapidjson::Value index(member.c_str(), static_cast<int>(member.size()), doc.GetAllocator());
  obj.AddMember(index, rapidValue, doc.GetAllocator());
}

void json::addStringToObject(rapidjson::Document& doc,
                             rapidjson::Value& obj,
                             const std::string& member,
                             const std::string& value)
{
  rapidjson::Value rapidValue(value.c_str(), static_cast<int>(value.size()), doc.GetAllocator());
  rapidjson::Value index(member.c_str(), static_cast<int>(member.size()), doc.GetAllocator());
  obj.AddMember(index, rapidValue, doc.GetAllocator());
}

void json::addIntToObject(rapidjson::Document& doc,
                          rapidjson::Value& obj,
                          std::string& member,
                          int value)
{
  rapidjson::Value rapidValue;
  rapidValue.SetInt(value);
  rapidjson::Value index(member.c_str(), member.size(), doc.GetAllocator());
  obj.AddMember(index, rapidValue, doc.GetAllocator());
}

void json::addNullToArray(rapidjson::Document& doc, rapidjson::Value& arr)
{
  rapidjson::Value rapidValue;
  arr.PushBack(rapidValue, doc.GetAllocator());
}

void json::addBoolToArray(rapidjson::Document& doc,
                          rapidjson::Value& arr,
                          bool value)
{
  rapidjson::Value rapidValue(value);
  arr.PushBack(rapidValue, doc.GetAllocator());
}

void json::addObjectToArray(rapidjson::Document& doc,
                            rapidjson::Value& arr,
                            rapidjson::Value& value)
{
  rapidjson::Value rapidValue(rapidjson::kObjectType);
  rapidValue = value;
  arr.PushBack(rapidValue, doc.GetAllocator());
}

void json::addArrayToArray(rapidjson::Document& doc,
                           rapidjson::Value& arr,
                           rapidjson::Value& value)
{
  rapidjson::Value rapidValue(rapidjson::kArrayType);
  rapidValue = value;
  arr.PushBack(rapidValue, doc.GetAllocator());
}

void json::addStringToArray(rapidjson::Document& doc,
                            rapidjson::Value& arr,
                            const std::string& value)
{
  rapidjson::Value rapidValue(value.c_str(), static_cast<int>(value.size()), doc.GetAllocator());
  arr.PushBack(rapidValue, doc.GetAllocator());
}

void json::addIntToArray(rapidjson::Document& doc,
                         rapidjson::Value& arr,
                         int value)
{
  rapidjson::Value rapidValue(value);
  arr.PushBack(rapidValue, doc.GetAllocator());
}
