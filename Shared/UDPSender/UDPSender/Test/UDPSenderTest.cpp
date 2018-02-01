#include <QtTest>
#include "UDPSender/UDPSender.hpp"
#include <iostream>

class UDPSenderTest : public QObject
{
  Q_OBJECT

public:
  UDPSenderTest();
  ~UDPSenderTest();

  private slots:
  void initTestCase();
  void cleanupTestCase();
  void testDatagramConversion();
  void testBytesSent();
};

UDPSenderTest::UDPSenderTest()
{

}

UDPSenderTest::~UDPSenderTest()
{

}

void UDPSenderTest::initTestCase()
{

}

void UDPSenderTest::testDatagramConversion()
{
  std::string start = "Hello";
  QByteArray buf = QByteArray::fromStdString(start);
  std::string end = buf.toStdString();
  QVERIFY(start == end);
}
void UDPSenderTest::testBytesSent()
{
  UDPSender* sender = new UDPSender();
  std::string msg = "This is a test";
  qint64 msgLen = (qint64)msg.size();
  qint64 bytesSent = sender->send(msg, 1, std::chrono::seconds(5), false, QHostAddress("127.0.0.1"), 8080);
  QVERIFY(msgLen == bytesSent);
  delete sender;
}

void UDPSenderTest::cleanupTestCase()
{

}

QTEST_APPLESS_MAIN(UDPSenderTest)

#include "UDPSenderTest.moc"