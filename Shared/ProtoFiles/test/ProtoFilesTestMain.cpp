#include <QtTest>
#include "ProtoFiles/addressbook.pb.h"

class ProtoFilesTest : public QObject
{
  Q_OBJECT

public:
  ProtoFilesTest();
  ~ProtoFilesTest();

  private slots:
  void initTestCase();
  void cleanupTestCase();
  void addressbook();
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

void ProtoFilesTest::addressbook()
{
  tutorial::Person* person = new tutorial::Person();
  std::cout << "yea!!!" << std::endl;
  person->set_allocated_email(new std::string("cameron.frandsen3@gmail.com"));
  auto sString = person->SerializeAsString();
  tutorial::Person returnedMsg;
  returnedMsg.ParseFromString(sString);
  QVERIFY(returnedMsg.email() == "cameron.frandsen3@gmail.com");
}

QTEST_APPLESS_MAIN(ProtoFilesTest)

#include "ProtoFilesTestMain.moc"
