#include "Algorithms.h"
#include "Utilities.h"
#include "Comparators.h"
#include <set>
#include <queue>
#include <algorithm>
#include <cstdio>
#include <iostream>

Result lsa(int m, std::vector<Job> original_jobs)
{
    std::vector<Job> jobs = original_jobs;

    std::vector<Machine> machines(m);
    for (int i = 0; i < m; i++)
    {
        machines[i].id = i;
    }

    for (const auto &job : jobs)
    {
        auto min_machine = std::min_element(machines.begin(), machines.end(), CompareMachines());
        min_machine->jobs.push_back(job);
        min_machine->maxTime += job.p;
    }

    int C_max = 0;
    for (size_t i = 0; i < machines.size(); i++)
    {
        if (machines.at(i).maxTime > C_max)
            C_max = machines.at(i).maxTime;
    }

    return {machines, C_max};
}
Result lpt(int m, std::vector<Job> original_jobs)
{
    std::vector<Job> jobs = original_jobs;
    std::sort(jobs.begin(), jobs.end(), CompareJobs());

    std::vector<Machine> machines(m);
    for (int i = 0; i < m; i++)
    {
        machines[i].id = i;
    }

    for (const auto &job : jobs)
    {
        auto min_machine = std::min_element(machines.begin(), machines.end(), CompareMachines());
        min_machine->jobs.push_back(job);
        min_machine->maxTime += job.p;
    }
    int C_max = 0;
    for (size_t i = 0; i < machines.size(); i++)
    {
        if (machines.at(i).maxTime > C_max)
            C_max = machines.at(i).maxTime;
    }

    return {machines, C_max};
}

Result solveP2Cmax(std::vector<Job> original_jobs)
{
    std::vector<Job> jobs = original_jobs;
    int total_sum = 0;
    for (const auto &job : jobs)
        total_sum += job.p;
    int target = total_sum / 2;

    std::vector<std::vector<bool>> dp(jobs.size() + 1, std::vector<bool>(target + 1, false));
    dp[0][0] = true;

    for (int j = 1; j <= (int)jobs.size(); ++j)
    {
        int p = jobs[j - 1].p;
        for (int k = 0; k <= target; ++k)
        {
            if (dp[j - 1][k] || (k >= p && dp[j - 1][k - p]))
            {
                dp[j][k] = true;
            }
        }
    }

    int best_sum = 0;
    for (int k = target; k >= 0; --k)
    {
        if (dp[jobs.size()][k])
        {
            best_sum = k;
            break;
        }
    }

    Machine m1, m2;
    m1.id = 0;
    m1.maxTime = 0;
    m2.id = 1;
    m2.maxTime = 0;
    int remaining_sum = best_sum;

    for (int j = jobs.size(); j >= 1; --j)
    {
        if (remaining_sum >= jobs[j - 1].p && dp[j - 1][remaining_sum - jobs[j - 1].p])
        {
            m1.jobs.push_back(jobs[j - 1]);
            m1.maxTime += jobs[j - 1].p;
            remaining_sum -= jobs[j - 1].p;
        }
        else
        {
            m2.jobs.push_back(jobs[j - 1]);
            m2.maxTime += jobs[j - 1].p;
        }
    }

    int C_max = std::max(m1.maxTime, m2.maxTime);

    Result result;
    result.perm.push_back(m1);
    result.perm.push_back(m2);
    result.C_max = C_max;

    return result;
}

void branchAndBound(int jobIndex, const std::vector<Job> &jobs, int sumM1, int sumM2,
                    std::vector<Job> &jobsM1, std::vector<Job> &jobsM2, int &bestCmax,
                    std::vector<Job> &bestJobsM1, std::vector<Job> &bestJobsM2)
{

    if (jobIndex == jobs.size())
    {
        int Cmax = std::max(sumM1, sumM2);
        if (Cmax < bestCmax)
        {
            bestCmax = Cmax;
            bestJobsM1 = jobsM1; // Aktualizacja najlepszych przypisań
            bestJobsM2 = jobsM2;
        }
        return;
    }

    // Przypisanie do M1
    jobsM1.push_back(jobs[jobIndex]);
    branchAndBound(jobIndex + 1, jobs, sumM1 + jobs[jobIndex].p, sumM2,
                   jobsM1, jobsM2, bestCmax, bestJobsM1, bestJobsM2);
    jobsM1.pop_back();

    // Przypisanie do M2
    jobsM2.push_back(jobs[jobIndex]);
    branchAndBound(jobIndex + 1, jobs, sumM1, sumM2 + jobs[jobIndex].p,
                   jobsM1, jobsM2, bestCmax, bestJobsM1, bestJobsM2);
    jobsM2.pop_back();
}

Result solveP2Cmax_BnB(std::vector<Job> original_jobs)
{
    std::vector<Job> jobs = original_jobs;
    int bestCmax = INT_MAX;
    std::vector<Job> bestJobsM1, bestJobsM2;
    std::vector<Job> currentJobsM1, currentJobsM2; // Oddzielne wektory dla aktualnego przypisania

    branchAndBound(0, jobs, 0, 0, currentJobsM1, currentJobsM2, bestCmax, bestJobsM1, bestJobsM2);

    Result result;
    Machine m1, m2;
    m1.id = 0;
    m2.id = 1;
    m1.maxTime = 0;
    m2.maxTime = 0;

    for (const auto &job : bestJobsM1)
    {
        m1.jobs.push_back(job);
        m1.maxTime += job.p;
    }

    for (const auto &job : bestJobsM2)
    {
        m2.jobs.push_back(job);
        m2.maxTime += job.p;
    }

    result.perm.push_back(m1);
    result.perm.push_back(m2);
    result.C_max = bestCmax;

    return result;
}