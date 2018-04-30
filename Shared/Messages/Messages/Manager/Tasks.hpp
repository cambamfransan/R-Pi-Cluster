#ifndef TASKS_CLASS_H
#define TASKS_CLASS_H

#include <string>
#include <vector>

namespace manager
{

struct Task
{
  Task(int jobId, int page, int task, std::string execute);

  bool operator==(const Task& t);

  int jobId;

  int pageNumber;

  int taskId;

  std::string toExecute;
};

struct TaskFile
{
  TaskFile(int page, int task, std::string name);
  int pageNumber;

  int nextTaskId;

  std::string pageName;
};

  class TaskManager
  {
  public:
    TaskManager(int id, int size, std::string database, std::string tasksList = "");
    TaskManager();

    void populateFields(std::string taskList);

    ~TaskManager();

    std::vector<Task> getNextTasks(int howManyTasks);

    bool removeFromResults(Task task);

    bool removeFromResults(std::vector<Task> task);

    int getSize();

    int getTotal();

  private:
    
    int m_myId;
    int m_maxSize;
    bool m_valid;
    std::string m_database;
    std::vector<TaskFile> m_taskFiles;
    int m_total;
  };
} // namespace manager
#endif
