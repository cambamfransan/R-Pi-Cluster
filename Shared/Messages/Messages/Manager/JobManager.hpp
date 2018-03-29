#ifndef JOB_MANAGER_H
#define JOB_MANAGER_H

#include "Job.hpp"
#include <map>
#include <mutex>
#include <string>

namespace manager
{
  struct JobUpdateStruct
  {
    std::vector<Job> newJobs;
    std::vector<int> lostJobs;
    std::vector<std::vector<Result>> newResults;
    std::vector<ModifiedJob> modifiedJobs;
  };

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
    void setName(int id, std::string name);
    std::string getJobName(int id);
    std::string getJobExec(int id);
    void modifyTasksPerBundle(int id, int taskPerBundle);
    void modifyPriority(int id, int priority);
    JobUpdateStruct getUpdates();

  private:
    std::map<int, Job> m_jobs;
    int m_nextJobId;
    std::string m_database;

    int m_curJob;
    int m_curJobDone;
    std::mutex m_jobsMutex;

    std::mutex m_updateMutex;
    std::vector<Job> m_newJobs;
    std::vector<int> m_lostJobs;
    std::vector<std::vector<Result>> m_newResults;
    std::vector<ModifiedJob> m_modifiedJobs;

    static std::string m_cloneScript;
  };

} // namespace manager

#endif
