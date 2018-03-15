#include "ManagerTestMain.hpp"
#include "Messages/Manager/Tasks.hpp"

ManagerTest::ManagerTest() {}

ManagerTest::~ManagerTest() {}

void ManagerTest::initTestCase() {}

void ManagerTest::cleanupTestCase() {}

void ManagerTest::getTasksTest()
{
  manager::Tasks tasks(1, 10, std::string("..//..//..//..//..//R-Pi-Cluster//Shared//Messages//Messages//Test//TasksList.txt"), std::string("FirstTest"));

  auto toExecute = tasks.getNextTasks(5);
  QVERIFY(toExecute[0].first == 0 && toExecute[0].second == "0");
  QVERIFY(toExecute[1].first == 0 && toExecute[1].second == "1");
  QVERIFY(toExecute[2].first == 0 && toExecute[2].second == "2");
  QVERIFY(toExecute[3].first == 0 && toExecute[3].second == "3");
  QVERIFY(toExecute[4].first == 0 && toExecute[4].second == "4");

  toExecute = tasks.getNextTasks(10);
  QVERIFY(toExecute[0].first == 0 && toExecute[0].second == "5");
  QVERIFY(toExecute[1].first == 0 && toExecute[1].second == "6");
  QVERIFY(toExecute[2].first == 0 && toExecute[2].second == "7");
  QVERIFY(toExecute[3].first == 0 && toExecute[3].second == "8");
  QVERIFY(toExecute[4].first == 0 && toExecute[4].second == "9");
  QVERIFY(toExecute[5].first == 1 && toExecute[5].second == "10");
  QVERIFY(toExecute[6].first == 1 && toExecute[6].second == "11");
  QVERIFY(toExecute[7].first == 1 && toExecute[7].second == "12");
  QVERIFY(toExecute[8].first == 1 && toExecute[8].second == "13");
  QVERIFY(toExecute[9].first == 1 && toExecute[9].second == "14");

  toExecute = tasks.getNextTasks(5);
  QVERIFY(toExecute[0].first == 1 && toExecute[0].second == "15");
  QVERIFY(toExecute[1].first == 1 && toExecute[1].second == "16");
  QVERIFY(toExecute[2].first == 1 && toExecute[2].second == "17");
  QVERIFY(toExecute[3].first == 1 && toExecute[3].second == "18");
  QVERIFY(toExecute[4].first == 1 && toExecute[4].second == "19");

  toExecute = tasks.getNextTasks(10);
  QVERIFY(toExecute.size() == 6);

  QVERIFY(toExecute[0].first == 2 && toExecute[0].second == "20");
  QVERIFY(toExecute[1].first == 2 && toExecute[1].second == "21");
  QVERIFY(toExecute[2].first == 2 && toExecute[2].second == "22");
  QVERIFY(toExecute[3].first == 2 && toExecute[3].second == "23");
  QVERIFY(toExecute[4].first == 2 && toExecute[4].second == "24");
  QVERIFY(toExecute[5].first == 2 && toExecute[5].second == "25");
}

void ManagerTest::removeTasksTest()
{
  manager::Tasks tasks(1, 10, std::string("..//..//..//..//..//R-Pi-Cluster//Shared//Messages//Messages//Test//TasksList.txt"), std::string("FirstTest"));

  QVERIFY(tasks.removeFromResults(Task(0, "0")));
  QVERIFY(tasks.removeFromResults(Task(0, "5")));
  QVERIFY(tasks.removeFromResults(Task(0, "9")));
  QVERIFY(tasks.removeFromResults(Task(1, "10")));
  QVERIFY(tasks.removeFromResults(Task(2, "21")));
  QVERIFY(!tasks.removeFromResults(Task(2, "21")));
  QVERIFY(!tasks.removeFromResults(Task(1, "0")));
}
