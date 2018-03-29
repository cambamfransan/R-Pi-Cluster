#ifndef MESSAGES_TEST_H
#define MESSAGES_TEST_H

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

#endif
