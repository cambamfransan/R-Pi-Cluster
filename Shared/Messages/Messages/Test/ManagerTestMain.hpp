#ifndef MANAGER_TEST_H
#define MANAGER_TEST_H

#include <QtTest>

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
  void addResults();
  void pi();
  void job();
  void cleanupTestCase();
};

#endif
