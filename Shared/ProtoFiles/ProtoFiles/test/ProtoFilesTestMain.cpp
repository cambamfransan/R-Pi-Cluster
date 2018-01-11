#include <QtTest>
#include "ProtoFiles/Backup.pb.h"
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
  void backupMsg();
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

void ProtoFilesTest::backupMsg()
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
  pMsg->set_subid(6);
  std::string encrypted = pMsg->SerializeAsString();
  std::shared_ptr<msg::BasicMsg> pUnencrypted = std::make_shared<msg::BasicMsg>();
  pUnencrypted->ParseFromString(encrypted);
  QVERIFY(pUnencrypted->fromid() == 1);
  QVERIFY(pUnencrypted->toid() == 2);
  QVERIFY(pUnencrypted->msgtype() == 3);
  QVERIFY(pUnencrypted->attempt() == 4);
  QVERIFY(pUnencrypted->convid() == 5);
  QVERIFY(pUnencrypted->subid() == 6);
}

void ProtoFilesTest::taskMsg()
{
  std::shared_ptr<msg::TaskMsg> pMsg = std::make_shared<msg::TaskMsg>();
  pMsg->add_toexecute("1 2 3");
  pMsg->add_toexecute("4 5 6");
  pMsg->add_toexecute("7 8 9");
  std::string encrypted = pMsg->SerializeAsString();
  std::shared_ptr<msg::TaskMsg> pUnencrypted = std::make_shared<msg::TaskMsg>();
  pUnencrypted->ParseFromString(encrypted);
  QVERIFY(pUnencrypted->toexecute(0) == "1 2 3");
  QVERIFY(pUnencrypted->toexecute(1) == "4 5 6");
  QVERIFY(pUnencrypted->toexecute(2) == "7 8 9");
}

QTEST_APPLESS_MAIN(ProtoFilesTest)

#include "ProtoFilesTestMain.moc"
