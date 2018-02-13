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

void MessagesTest::cleanupTestCase()
{

}

QTEST_APPLESS_MAIN(MessagesTest)

#include "MessagesTestMain.moc"