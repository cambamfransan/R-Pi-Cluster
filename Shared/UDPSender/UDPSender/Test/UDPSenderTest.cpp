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
  void testCheckConvs();
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
  //UDPSender* sender = new UDPSender(UdpEndpoint::CLIENT, 0);
  //std::string msg = "This is a test";
  //qint64 msgLen = (qint64)msg.size();
  //qint64 bytesSent = sender->send(msg, 1, std::chrono::seconds(5), false, UdpEndpoint::CLIENT, 1, QHostAddress("127.0.0.1"), 8080);
  //QVERIFY(msgLen == bytesSent);
  //delete sender;
}

void UDPSenderTest::testCheckConvs()
{
  //UDPSender* sender = new UDPSender(UdpEndpoint::CLIENT, 0);
  //std::string ms11 = clue::moveGuess(3, 2, 1, {}, 3)->SerializeAsString();
  //std::string ms21 = clue::moveAck(3, 2, 2)->SerializeAsString();
  //std::string ms22 = clue::moveGuessAck(3, 2, 2)->SerializeAsString();
  //std::string ms31 = clue::moveGuessReply(3, 2, 3, msg::BALLROOM)->SerializeAsString();
  //std::string ms32 = clue::movePending(3, 2, 3, 30)->SerializeAsString();
  //std::string ms33 = clue::moveAck(3, 2, 3)->SerializeAsString();
  //sender->send(ms11, 1, std::chrono::seconds(1), false, UdpEndpoint::SERVER, 2, QHostAddress("127.0.0.1"), 8080);
  //sender->send(ms21, 2, std::chrono::seconds(1), false, UdpEndpoint::SERVER, 2, QHostAddress("127.0.0.1"), 8080);
  //sender->send(ms22, 2, std::chrono::seconds(1), true, UdpEndpoint::SERVER, 2, QHostAddress("127.0.0.1"), 8080);
  //sender->send(ms31, 3, std::chrono::seconds(1), false, UdpEndpoint::SERVER, 2, QHostAddress("127.0.0.1"), 8080);
  //sender->send(ms32, 3, std::chrono::seconds(5), false, UdpEndpoint::SERVER, 2, QHostAddress("127.0.0.1"), 8080);
  //sender->send(ms33, 3, std::chrono::seconds(5), true, UdpEndpoint::SERVER, 2, QHostAddress("127.0.0.1"), 8080);

  //std::this_thread::sleep_for(std::chrono::seconds(6));

  //auto hist = sender->getConvHist();

  //QVERIFY(hist[1].msgs.size() == 1);
  //QVERIFY(hist[2].msgs.size() == 6);
  //QVERIFY(hist[3].msgs.size() == 4);
}

void UDPSenderTest::cleanupTestCase()
{

}

QTEST_APPLESS_MAIN(UDPSenderTest)

#include "UDPSenderTest.moc"