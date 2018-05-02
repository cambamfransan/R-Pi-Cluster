#ifndef JOB_MANAGER_H
#define JOB_MANAGER_H

#include "Job.hpp"
#include <map>
#include <mutex>
#include <string>
#include <memory>

namespace manager
{
  struct JobUpdateStruct
  {
    std::vector<JobInfo> newJobs;
    std::vector<int> lostJobs;
    std::vector<std::vector<Result>> newResults;
    std::vector<ModifiedJob> modifiedJobs;
  };

  class JobManager
  {
  public:
#ifdef __GNUC__
    JobManager(
      std::string m_database,
      std::string cloneScript = ".//..//..//R-Pi-Cluster/Scripts/cloneUrl.sh");
#else
    JobManager(
      std::string m_database,
      std::string cloneScript = ".\\..\\..\\R-Pi-Cluster\\Scripts\\cloneUrl.bat");
#endif
    ~JobManager();

    int getNextId();
    int addJob(int size, int pri, int taskpb, std::string gitUrl, int id);
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
    std::map<int, int> getProgress();
    bool moreJobsToProcess();
    std::vector<JobInfo> getCurrentJobs();
    std::vector<std::pair<int, std::string>> getResults(int id);
    std::vector<int> getJobIds();

  private:
    std::map<int, std::shared_ptr<Job>> m_jobs;
    int m_nextJobId;
    std::string m_database;

    int m_curJob;
    int m_curJobDone;
    std::mutex m_jobsMutex;

    std::mutex m_updateMutex;
    std::vector<JobInfo> m_newJobs;
    std::vector<int> m_lostJobs;
    std::vector<std::vector<Result>> m_newResults;
    std::vector<ModifiedJob> m_modifiedJobs;

    std::string m_cloneScript;
  };

} // namespace manager

#endif
