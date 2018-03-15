#ifndef TASKS_CLASS_H
#define TASKS_CLASS_H

#include <string>
#include <vector>

typedef std::pair<int, std::string> Task;

namespace manager
{

  class Tasks
  {
  public:
    Tasks(int id, int size, std::string TasksList, std::string destDir);

    ~Tasks();

    std::vector<Task> getNextTasks(int howManyTasks);

    bool removeFromResults(Task task);

  private:
    int m_myId;
    int m_maxSize;
    bool m_valid;
    std::string m_destDir;
    std::vector<std::pair<int, std::string>> m_taskFiles;
  };
} // namespace manager
#endif