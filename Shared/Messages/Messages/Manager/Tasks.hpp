#ifndef TASKS_CLASS_H
#define TASKS_CLASS_H

#include <string>
#include <vector>

namespace manager
{

struct Task
{
  Task(int page, int task, std::string execute);
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

  class Tasks
  {
  public:
    Tasks(int id, int size, std::string TasksList);

    ~Tasks();

    std::vector<Task> getNextTasks(int howManyTasks);

    bool removeFromResults(Task task);

  private:
    int m_myId;
    int m_maxSize;
    bool m_valid;
    std::vector<TaskFile> m_taskFiles;
  };
} // namespace manager
#endif