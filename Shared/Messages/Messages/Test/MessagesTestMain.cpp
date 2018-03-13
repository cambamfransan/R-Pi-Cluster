#include "Messages/MakeMsgs.hpp"
#include "Messages/MapHelpers.hpp"
#include <Messages/JSONHelpers.hpp>
#include <Messages/JSONParser.hpp>
#include <QtTest>
#include <iostream>
#include <regex>

class MessagesTest : public QObject
{
  Q_OBJECT

public:
  MessagesTest();
  ~MessagesTest();

private slots:
  void initTestCase();
  void cleanupTestCase();
  void TestMakeMsgs();
  void testMapIndex();
  void testUpdateMsgs();
  void testAddToObject();
  void testAddToArray();
  void testAddToDoc();
  void testJSONToString();
};

MessagesTest::MessagesTest() {}

MessagesTest::~MessagesTest() {}

void MessagesTest::initTestCase() {}

void MessagesTest::TestMakeMsgs()
{
  msg::MsgToSend* first = make_msgs::makeTestMsg(1, 2, 3, "This Message");
  msg::MsgToSend* second = make_msgs::makeTaskMsg(1, 2, 3, "This Message");
  msg::BasicMsg* third = make_msgs::makeBasicMsg(1, 2, 3, 4);
  msg::MsgToSend* fourth = make_msgs::makeBasicMsgToSend(1, 2, 3, 4);
  QVERIFY(first->basicmsg().convid() == 3);
  QVERIFY(first->basicmsg().toid() == 2);
  QVERIFY(first->basicmsg().fromid() == 1);
  QVERIFY(first->test().teststring(0) == "This Message");

  QVERIFY(second->basicmsg().convid() == 3);
  QVERIFY(second->basicmsg().toid() == 2);
  QVERIFY(second->basicmsg().fromid() == 1);
  QVERIFY(second->task().toexecute(0) == "This Message");

  QVERIFY(third->convid() == 4);
  QVERIFY(third->toid() == 2);
  QVERIFY(third->fromid() == 1);
  QVERIFY(third->msgtype() == 3);

  QVERIFY(fourth->basicmsg().convid() == 4);
  QVERIFY(fourth->basicmsg().toid() == 2);
  QVERIFY(fourth->basicmsg().fromid() == 1);
  QVERIFY(fourth->basicmsg().msgtype() == 3);
}

void MessagesTest::testMapIndex()
{
  QVERIFY(make_msgs::getMapId(2, 3) == 23);
  QVERIFY(make_msgs::getMapId(12, 3) == 123);
  QVERIFY(make_msgs::getMapId(2, 13) == 213);
  QVERIFY(make_msgs::getMapId(12, 13) == 1213);
}

void MessagesTest::testUpdateMsgs()
{
  auto first = make_msgs::makeUpdateMsg(1, 2, 3, 4, {});
  ClientInfo info{"127.0.0.1", 51212, "pi", "PiCluster!", 1};
  std::map<int, ClientInfo> firstMap;
  firstMap[1] = info;
  auto second = make_msgs::makeUpdateMsg(1, 2, 3, 4, firstMap);
  ClientInfo info2{"127.0.0.1", 51212, "pi", "PiCluster!", 2};
  ClientInfo info3{"127.0.0.1", 51212, "pi", "PiCluster!", 3};
  ClientInfo info4{"127.0.0.1", 51212, "pi", "PiCluster!", 4};
  firstMap[2] = info2;
  firstMap[3] = info3;
  firstMap[4] = info4;
  auto third = make_msgs::makeUpdateMsg(1, 2, 3, 4, firstMap);

  third->update().clients(0).ipaddress();

  QVERIFY(first->update().clients_size() == 0);
  QVERIFY(second->update().clients_size() == 1);
  QVERIFY(third->update().clients(0).ipaddress() == "127.0.0.1");
  QVERIFY(third->update().clients(0).port() == 51212);
  QVERIFY(third->update().clients(0).username() == "pi");
  QVERIFY(third->update().clients(0).password() == "PiCluster!");
  QVERIFY(third->update().clients(0).priority() == 1);

  QVERIFY(third->update().clients(1).ipaddress() == "127.0.0.1");
  QVERIFY(third->update().clients(1).port() == 51212);
  QVERIFY(third->update().clients(1).username() == "pi");
  QVERIFY(third->update().clients(1).password() == "PiCluster!");
  QVERIFY(third->update().clients(1).priority() == 2);

  QVERIFY(third->update().clients(2).ipaddress() == "127.0.0.1");
  QVERIFY(third->update().clients(2).port() == 51212);
  QVERIFY(third->update().clients(2).username() == "pi");
  QVERIFY(third->update().clients(2).password() == "PiCluster!");
  QVERIFY(third->update().clients(2).priority() == 3);

  QVERIFY(third->update().clients(3).ipaddress() == "127.0.0.1");
  QVERIFY(third->update().clients(3).port() == 51212);
  QVERIFY(third->update().clients(3).username() == "pi");
  QVERIFY(third->update().clients(3).password() == "PiCluster!");
  QVERIFY(third->update().clients(3).priority() == 4);
}

void MessagesTest::cleanupTestCase() {}

void MessagesTest::testAddToObject()
{
  rapidjson::Document d;
  rapidjson::Value v(rapidjson::kObjectType);
  json::addIntToObject(d, v, std::string("first"), 1);
  json::addIntToObject(d, v, std::string("second"), 2);
  json::addBoolToObject(d, v, std::string("third"), true);
  json::addBoolToObject(d, v, std::string("fourth"), false);
  json::addNullToObject(d, v, std::string("fifth"));
  json::addStringToObject(
    d, v, std::string("sixth"), std::string("Hello World"));

  rapidjson::Value v2(rapidjson::kObjectType);
  json::addIntToObject(d, v2, std::string("first"), 1);
  json::addIntToObject(d, v2, std::string("second"), 2);
  json::addBoolToObject(d, v2, std::string("third"), true);
  json::addBoolToObject(d, v2, std::string("fourth"), false);
  json::addNullToObject(d, v2, std::string("fifth"));
  json::addStringToObject(
    d, v2, std::string("sixth"), std::string("Hello World"));
  json::addObjectToObject(d, v, std::string("seventh"), v2);

  json::JSONParser parser;
  parser.convertJSONObject(v, "");
  auto map = parser.getMap();

  int int1(0), int2(0), bool1(0), bool2(0), null1(0), string1(0), int3(0),
    int4(0), bool3(0), bool4(0), null2(0), string2(0);
  for (auto&& value : map)
  {
    if (value.first == "/first" && value.second == "1") int1 += 1;
    if (value.first == "/second" && value.second == "2") int2 += 1;
    if (value.first == "/third" && value.second == "true") bool1 += 1;
    if (value.first == "/fourth" && value.second == "false") bool2 += 1;
    if (value.first == "/fifth" && value.second == "null") null1 += 1;
    if (value.first == "/sixth" && value.second == "Hello World") string1 += 1;

    if (value.first == "/seventh/first" && value.second == "1") int3 += 1;
    if (value.first == "/seventh/second" && value.second == "2") int4 += 1;
    if (value.first == "/seventh/third" && value.second == "true") bool3 += 1;
    if (value.first == "/seventh/fourth" && value.second == "false") bool4 += 1;
    if (value.first == "/seventh/fifth" && value.second == "null") null2 += 1;
    if (value.first == "/seventh/sixth" && value.second == "Hello World")
      string2 += 1;
  }

  QVERIFY(int1 == 1);
  QVERIFY(int2 == 1);
  QVERIFY(bool1 == 1);
  QVERIFY(bool2 == 1);
  QVERIFY(null1 == 1);
  QVERIFY(string1 == 1);

  QVERIFY(int3 == 1);
  QVERIFY(int4 == 1);
  QVERIFY(bool3 == 1);
  QVERIFY(bool4 == 1);
  QVERIFY(null2 == 1);
  QVERIFY(string2 == 1);
}
void MessagesTest::testAddToArray()
{
  rapidjson::Document d;
  rapidjson::Value v(rapidjson::kArrayType);
  json::addIntToArray(d, v, 1);
  json::addIntToArray(d, v, 2);
  json::addBoolToArray(d, v, true);
  json::addBoolToArray(d, v, false);
  json::addNullToArray(d, v);
  json::addStringToArray(d, v, std::string("Hello World"));

  json::JSONParser parser;
  parser.convertJSONArray(v, "");
  auto map = parser.getMap();

  int int1(0), int2(0), bool1(0), bool2(0), null1(0), string1(0);

  for (auto&& value : map)
  {
    if (value.second == "1") int1 += 1;
    if (value.second == "2") int2 += 1;
    if (value.second == "true") bool1 += 1;
    if (value.second == "false") bool2 += 1;
    if (value.second == "null") null1 += 1;
    if (value.second == "Hello World") string1 += 1;
  }

  QVERIFY(int1 == 1);
  QVERIFY(int2 == 1);
  QVERIFY(bool1 == 1);
  QVERIFY(bool2 == 1);
  QVERIFY(null1 == 1);
  QVERIFY(string1 == 1);
}

void MessagesTest::testAddToDoc()
{
  rapidjson::Document d;
  d.SetObject();
  json::addIntToDoc(d, std::string("first"), 1);
  json::addIntToDoc(d, std::string("second"), 2);
  json::addBoolToDoc(d, std::string("third"), true);
  json::addBoolToDoc(d, std::string("fourth"), false);
  json::addNullToDoc(d, std::string("fifth"));
  json::addStringToDoc(d, std::string("sixth"), std::string("Hello World"));

  json::JSONParser parser(d);
  auto map = parser.getMap();

  int int1(0), int2(0), bool1(0), bool2(0), null1(0), string1(0);

  for (auto&& value : map)
  {
    if (value.first == "/first" && value.second == "1") int1 += 1;
    if (value.first == "/second" && value.second == "2") int2 += 1;
    if (value.first == "/third" && value.second == "true") bool1 += 1;
    if (value.first == "/fourth" && value.second == "false") bool2 += 1;
    if (value.first == "/fifth" && value.second == "null") null1 += 1;
    if (value.first == "/sixth" && value.second == "Hello World") string1 += 1;
  }

  QVERIFY(int1 == 1);
  QVERIFY(int2 == 1);
  QVERIFY(bool1 == 1);
  QVERIFY(bool2 == 1);
  QVERIFY(null1 == 1);
  QVERIFY(string1 == 1);
}

void MessagesTest::testJSONToString()
{
  rapidjson::Document d;
  d.SetObject();
  json::addIntToDoc(d, std::string("first"), 1);
  json::addIntToDoc(d, std::string("second"), 2);
  json::addBoolToDoc(d, std::string("third"), true);
  json::addBoolToDoc(d, std::string("fourth"), false);
  json::addNullToDoc(d, std::string("fifth"));
  json::addStringToDoc(d, std::string("sixth"), std::string("Hello World"));

  std::string expected("{\"first\":1,\"second\":2,\"third\":true,\"fourth\":false,\"fifth\":null,\"sixth\":\"Hello World\"}");

  auto jsonString = json::jsonToString(d);

  QVERIFY(expected == jsonString);
}

QTEST_APPLESS_MAIN(MessagesTest)

#include "MessagesTestMain.moc"