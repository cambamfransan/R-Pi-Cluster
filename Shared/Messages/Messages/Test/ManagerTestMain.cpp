#include "ManagerTestMain.hpp"
#include "Messages/Manager/Results.hpp"
#include "Messages/Manager/Tasks.hpp"
#include <iostream>

ManagerTest::ManagerTest() {}

ManagerTest::~ManagerTest() {}

void ManagerTest::initTestCase() {}

void ManagerTest::cleanupTestCase() {}

void ManagerTest::getTasksTest()
{
  manager::Tasks tasks(1,
                       10,
                       std::string("..//..//..//..//..//R-Pi-Cluster//Shared//"
                                   "Messages//Messages//Test//TasksList.txt"));

  auto toExecute = tasks.getNextTasks(5);
  QVERIFY(toExecute[0].pageNumber == 0 && toExecute[0].toExecute == "0" &&
          toExecute[0].taskId == 0);
  QVERIFY(toExecute[1].pageNumber == 0 && toExecute[1].toExecute == "1" &&
          toExecute[1].taskId == 1);
  QVERIFY(toExecute[2].pageNumber == 0 && toExecute[2].toExecute == "2" &&
          toExecute[2].taskId == 2);
  QVERIFY(toExecute[3].pageNumber == 0 && toExecute[3].toExecute == "3" &&
          toExecute[3].taskId == 3);
  QVERIFY(toExecute[4].pageNumber == 0 && toExecute[4].toExecute == "4" &&
          toExecute[4].taskId == 4);

  toExecute = tasks.getNextTasks(10);
  QVERIFY(toExecute[0].pageNumber == 0 && toExecute[0].toExecute == "5" &&
          toExecute[0].taskId == 5);
  QVERIFY(toExecute[1].pageNumber == 0 && toExecute[1].toExecute == "6" &&
          toExecute[1].taskId == 6);
  QVERIFY(toExecute[2].pageNumber == 0 && toExecute[2].toExecute == "7" &&
          toExecute[2].taskId == 7);
  QVERIFY(toExecute[3].pageNumber == 0 && toExecute[3].toExecute == "8" &&
          toExecute[3].taskId == 8);
  QVERIFY(toExecute[4].pageNumber == 0 && toExecute[4].toExecute == "9" &&
          toExecute[4].taskId == 9);
  QVERIFY(toExecute[5].pageNumber == 1 && toExecute[5].toExecute == "10" &&
          toExecute[5].taskId == 10);
  QVERIFY(toExecute[6].pageNumber == 1 && toExecute[6].toExecute == "11" &&
          toExecute[6].taskId == 11);
  QVERIFY(toExecute[7].pageNumber == 1 && toExecute[7].toExecute == "12" &&
          toExecute[7].taskId == 12);
  QVERIFY(toExecute[8].pageNumber == 1 && toExecute[8].toExecute == "13" &&
          toExecute[8].taskId == 13);
  QVERIFY(toExecute[9].pageNumber == 1 && toExecute[9].toExecute == "14" &&
          toExecute[9].taskId == 14);

  toExecute = tasks.getNextTasks(5);
  QVERIFY(toExecute[0].pageNumber == 1 && toExecute[0].toExecute == "15" &&
          toExecute[0].taskId == 15);
  QVERIFY(toExecute[1].pageNumber == 1 && toExecute[1].toExecute == "16" &&
          toExecute[1].taskId == 16);
  QVERIFY(toExecute[2].pageNumber == 1 && toExecute[2].toExecute == "17" &&
          toExecute[2].taskId == 17);
  QVERIFY(toExecute[3].pageNumber == 1 && toExecute[3].toExecute == "18" &&
          toExecute[3].taskId == 18);
  QVERIFY(toExecute[4].pageNumber == 1 && toExecute[4].toExecute == "19" &&
          toExecute[4].taskId == 19);

  toExecute = tasks.getNextTasks(10);
  QVERIFY(toExecute.size() == 6);

  QVERIFY(toExecute[0].pageNumber == 2 && toExecute[0].toExecute == "20" &&
          toExecute[0].taskId == 20);
  QVERIFY(toExecute[1].pageNumber == 2 && toExecute[1].toExecute == "21" &&
          toExecute[1].taskId == 21);
  QVERIFY(toExecute[2].pageNumber == 2 && toExecute[2].toExecute == "22" &&
          toExecute[2].taskId == 22);
  QVERIFY(toExecute[3].pageNumber == 2 && toExecute[3].toExecute == "23" &&
          toExecute[3].taskId == 23);
  QVERIFY(toExecute[4].pageNumber == 2 && toExecute[4].toExecute == "24" &&
          toExecute[4].taskId == 24);
  QVERIFY(toExecute[5].pageNumber == 2 && toExecute[5].toExecute == "25" &&
          toExecute[5].taskId == 25);
}

void ManagerTest::removeTasksTest()
{
  manager::Tasks tasks(1,
                       10,
                       std::string("..//..//..//..//..//R-Pi-Cluster//Shared//"
                                   "Messages//Messages//Test//TasksList.txt"));

  QVERIFY(tasks.removeFromResults(manager::Task(0, 0, "0")));
  QVERIFY(tasks.removeFromResults(manager::Task(0, 5, "5")));
  QVERIFY(tasks.removeFromResults(manager::Task(0, 9, "9")));
  QVERIFY(tasks.removeFromResults(manager::Task(1, 10, "10")));
  QVERIFY(tasks.removeFromResults(manager::Task(2, 21, "21")));
  QVERIFY(!tasks.removeFromResults(manager::Task(2, 22, "21")));
  QVERIFY(!tasks.removeFromResults(manager::Task(1, 23, "0")));
}

void ManagerTest::addResults()
{
  manager::Results results(2);
  results.addResult(manager::Result(manager::Task(0, 0, "0"), "3"));
  results.addResult(manager::Result(manager::Task(0, 1, "1"), "1"));
  results.addResult(manager::Result(manager::Task(0, 2, "2"), "4"));
  results.addResult(manager::Result(manager::Task(0, 3, "3"), "1"));
  results.addResult(manager::Result(manager::Task(0, 4, "4"), "5"));
  results.addResult(manager::Result(manager::Task(1, 10, "10"), "5"));

  QVERIFY(
    results.getResults() ==
    "0:~:0:~:3\n1:~:1:~:1\n2:~:2:~:4\n3:~:3:~:1\n4:~:4:~:5\n10:~:10:~:5\n");
}
