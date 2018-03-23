#include <QtTest>
#include "ProtoFiles/Update.pb.h"
#include "ProtoFiles/BasicMsg.pb.h"
#include "ProtoFiles/TaskMsg.pb.h"

class ProtoFilesTest : public QObject
{
  Q_OBJECT

public:
  ProtoFilesTest();
  ~ProtoFilesTest();

  private slots:
  void initTestCase();
  void cleanupTestCase();
  void updateMsg();
  void basicMsg();
  void taskMsg();
};

ProtoFilesTest::ProtoFilesTest()
{
}

ProtoFilesTest::~ProtoFilesTest()
{
}

void ProtoFilesTest::initTestCase()
{
}

void ProtoFilesTest::cleanupTestCase()
{
  google::protobuf::ShutdownProtobufLibrary();
}

void ProtoFilesTest::updateMsg()
{

}

void ProtoFilesTest::basicMsg()
{
  std::shared_ptr<msg::BasicMsg> pMsg = std::make_shared<msg::BasicMsg>();
  pMsg->set_fromid(1);
  pMsg->set_toid(2);
  pMsg->set_msgtype(3);
  pMsg->set_attempt(4);
  pMsg->set_convid(5);
  std::string encrypted = pMsg->SerializeAsString();
  std::shared_ptr<msg::BasicMsg> pUnencrypted = std::make_shared<msg::BasicMsg>();
  pUnencrypted->ParseFromString(encrypted);
  QVERIFY(pUnencrypted->fromid() == 1);
  QVERIFY(pUnencrypted->toid() == 2);
  QVERIFY(pUnencrypted->msgtype() == 3);
  QVERIFY(pUnencrypted->attempt() == 4);
  QVERIFY(pUnencrypted->convid() == 5);
}

void ProtoFilesTest::taskMsg()
{
  std::shared_ptr<msg::TaskMsg> pMsg = std::make_shared<msg::TaskMsg>();
  msg::Task* pTask = new msg::Task();
  pTask->set_id(0);
  pTask->set_jobid(1);
  pTask->set_pagenumber(2);
  pTask->set_toexecute("string");
  pTask = pMsg->add_task();
//  pMsg->add_toexecute("4 5 6");
//  pMsg->add_toexecute("7 8 9");
  std::string encrypted = pMsg->SerializeAsString();
  std::shared_ptr<msg::TaskMsg> pUnencrypted = std::make_shared<msg::TaskMsg>();
  pUnencrypted->ParseFromString(encrypted);
  QVERIFY(pUnencrypted->task(0).id() == 0);
  QVERIFY(pUnencrypted->task(0).jobid() == 1);
  QVERIFY(pUnencrypted->task(0).pagenumber() == 2);
  QVERIFY(pUnencrypted->task(0).toexecute() == "string");
//  QVERIFY(pUnencrypted->toexecute(1) == "4 5 6");
//  QVERIFY(pUnencrypted->toexecute(2) == "7 8 9");
}

QTEST_APPLESS_MAIN(ProtoFilesTest)

#include "ProtoFilesTestMain.moc"
