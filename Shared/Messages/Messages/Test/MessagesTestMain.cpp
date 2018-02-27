#include <QtTest>
#include "Messages/MakeMsgs.hpp"
#include "Messages/MapHelpers.hpp"
#include <iostream>

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
};

MessagesTest::MessagesTest()
{

}

MessagesTest::~MessagesTest()
{

}

void MessagesTest::initTestCase()
{

}

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
  QVERIFY(make_msgs::getMapId(2, 3)==23);
  QVERIFY(make_msgs::getMapId(12, 3) == 123);
  QVERIFY(make_msgs::getMapId(2, 13) == 213);
  QVERIFY(make_msgs::getMapId(12, 13) == 1213);
}

void MessagesTest::testUpdateMsgs()
{
  auto first = make_msgs::makeUpdateMsg(1, 2, 3, 4, {});
  ClientInfo info{ "127.0.0.1", 51212, "pi", "PiCluster!", 1 };
  std::map<int, ClientInfo> firstMap;
  firstMap[1] = info;
  auto second = make_msgs::makeUpdateMsg(1, 2, 3, 4, firstMap);
  ClientInfo info2{ "127.0.0.1", 51212, "pi", "PiCluster!", 2 };
  ClientInfo info3{ "127.0.0.1", 51212, "pi", "PiCluster!", 3 };
  ClientInfo info4{ "127.0.0.1", 51212, "pi", "PiCluster!", 4 };
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

void MessagesTest::cleanupTestCase()
{

}

QTEST_APPLESS_MAIN(MessagesTest)

#include "MessagesTestMain.moc"