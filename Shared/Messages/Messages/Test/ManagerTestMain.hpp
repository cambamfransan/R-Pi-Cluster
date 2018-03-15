#ifndef MANAGER_TEST_H
#define MANAGER_TEST_H

#include <QtTest>
#include <iostream>
#include "Messages/Manager/Tasks.hpp"

class ManagerTest : public QObject
{
  Q_OBJECT

public:
  ManagerTest();
  ~ManagerTest();

private slots:
  void initTestCase();
  void getTasksTest();
  void removeTasksTest();
  void cleanupTestCase();
};

#endif
