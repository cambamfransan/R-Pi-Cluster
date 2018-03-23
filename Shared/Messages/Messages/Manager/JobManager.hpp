#ifndef JOB_MANAGER_H
#define JOB_MANAGER_H

#include "Job.hpp"
#include <string>
#include <map>

namespace manager
{
  class JobManager
  {
  public:
    JobManager(std::string m_database);
    ~JobManager();

    int addJob(int size, int pri, int taskpb, std::string gitUrl);
    std::vector<manager::Task> getTasks(int amount);
    void addJobResults(int id, std::vector<manager::Result> results);
    void removeJob(int id);
    void pauseJob(int id);
    void playJob(int id);
    void stopJob(int id);
    std::string getJobName(int id);
    std::string getJobExec(int id);
    void modifyTasksPerBundle(int id, int taskPerBundle);
    void modifyPriority(int id, int priority);

  private:
    std::map<int, Job> m_jobs;
    int m_nextJobId;
    std::string m_database;

    int m_curJob;
    int m_curJobDone;
    

    static std::string m_cloneScript;

  };

} // namespace manager

#endif
