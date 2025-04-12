#include "Algorithms.h"
#include "Utilities.h"
#include "Comparators.h"
#include <queue>
#include <algorithm>
#include <chrono>
#include <cstdio>

using namespace std;

void bruteForce(vector<Job> jobs, vector<Job> &best_permutation_rp, vector<Job> &best_permutation_rpq)
{

    int best_value_rp = compute_makespan_rp(jobs) + 1, best_value_rpq = compute_makespan_rpq(jobs) + 1;
    auto start = std::chrono::high_resolution_clock::now();
    int x = 0;
    int facty = fact(jobs.size());
    for (int i = 0; i < facty; i++)
    {
        next_permutation(jobs.begin(), jobs.end(), [](const Job &a, const Job &b)
                         { return a.id > b.id; });
        int value_rp = compute_makespan_rp(jobs);
        if (value_rp < best_value_rp)
        {
            best_value_rp = value_rp;
            best_permutation_rp = jobs;
        }
        int value_rpq = compute_makespan_rpq(jobs);
        if (value_rpq < best_value_rpq)
        {
            best_value_rpq = value_rpq;
            best_permutation_rpq = jobs;
        }
        if (debugLog)
        {
            auto endnow = std::chrono::high_resolution_clock::now();
            auto durationnow = std::chrono::duration_cast<std::chrono::seconds>(endnow - start);
            if ((int)durationnow.count() >= x)
            {
                x++;
                printf("Czas : %ds\n", (int)durationnow.count());
                printf("Iteracja: %d/%d\n", i, facty);
            }
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    printf("Czas wykonania: %d.%.3ds\n\n", (int)duration.count() / 1000, (int)duration.count() % 1000);
    printf("Makespan po rp\n");
    print_Jobs(best_permutation_rp);
    printf("C_max: %d\n\n", best_value_rp);
    printf("Makespan po rpq\n");
    print_Jobs(best_permutation_rpq);
    printf("C_max: %d\n\n", best_value_rpq);
}


void schrage(vector<Job> jobs, vector<Job> &best_permutation, int &C_max)
{
    priority_queue<Job, vector<Job>, CompareR_Rev> priorityJobsR(jobs.begin(), jobs.end());
    priority_queue<Job, vector<Job>, CompareQ_Rev> priorityJobsQ;
    C_max = 0;
    int t = 0;
    auto start = std::chrono::high_resolution_clock::now();

    while (!(priorityJobsR.empty()) || !(priorityJobsQ.empty()))
    {
        // Debug Logging
        if (debugLog)
        {
            priority_queue<Job, vector<Job>, CompareR_Rev> prioR = priorityJobsR;
            priority_queue<Job, vector<Job>, CompareQ_Rev> prioQ = priorityJobsQ;
            std::vector<Job> vecQ;
            for (; !prioQ.empty(); prioQ.pop())
                vecQ.emplace_back(prioQ.top());
            std::vector<Job> vecR;
            for (; !prioR.empty(); prioR.pop())
                vecR.emplace_back(prioR.top());
            printf("======= t = %d ==============\n", t);
            printf("Priority Queue R\n");
            print_Jobs(vecR);
            printf("Priority Queue Q\n");
            print_Jobs(vecQ);
            printf("==========================\n");
        }

        // If job becomes avaliable move it to priorityJobQ
        while (!priorityJobsR.empty() && priorityJobsR.top().r <= t)
        {
            priorityJobsQ.push(priorityJobsR.top());
            priorityJobsR.pop();
        }

        // If we don't have job, time skip
        if (priorityJobsQ.empty())
        {
            t = priorityJobsR.top().r;
        }
        // Complete currently avaliable most profitable job
        else
        {
            Job current = priorityJobsQ.top();
            priorityJobsQ.pop();
            best_permutation.push_back(current);
            t += current.p;
            C_max = max(C_max, t + current.q);
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

    printf("Czas wykonania: %d.%.3dus\n\n", (int)duration.count() / 1000, (int)duration.count() % 1000);
    print_Jobs(best_permutation);
    printf("C_max: %d\n\n", C_max);
}


void preemptiveSchrage(vector<Job> jobs, vector<Job> &best_permutation, int &C_max)
{
    priority_queue<Job, vector<Job>, CompareR_Rev> priorityJobsR(jobs.begin(), jobs.end());
    priority_queue<Job, vector<Job>, CompareQ_Rev> priorityJobsQ;
    C_max = 0;
    int t = 0;
    Job current;
    bool hasJob = false;
    auto start = std::chrono::high_resolution_clock::now();

    while (!(priorityJobsR.empty()) || !(priorityJobsQ.empty()) || hasJob)
    {
        // Debug Logging
        if (debugLog)
        {
            priority_queue<Job, vector<Job>, CompareR_Rev> prioR = priorityJobsR;
            priority_queue<Job, vector<Job>, CompareQ_Rev> prioQ = priorityJobsQ;
            std::vector<Job> vecQ;
            for (; !prioQ.empty(); prioQ.pop())
                vecQ.emplace_back(prioQ.top());
            std::vector<Job> vecR;
            for (; !prioR.empty(); prioR.pop())
                vecR.emplace_back(prioR.top());
            printf("======= t = %d ==============\n", t);
            printf("Priority Queue R\n");
            print_Jobs(vecR);
            printf("Priority Queue Q\n");
            print_Jobs(vecQ);
            printf("==========================\n");
        }

        // If job becomes avaliable move it to priorityJobQ
        while (!priorityJobsR.empty() && priorityJobsR.top().r <= t)
        {
            priorityJobsQ.push(priorityJobsR.top());
            priorityJobsR.pop();
        }

        // If we have no current job, take one from priorityJobQ
        if (!hasJob && !priorityJobsQ.empty())
        {
            current = priorityJobsQ.top();
            priorityJobsQ.pop();
            hasJob = true;
        }

        if (hasJob)
        {
            // Computing time we can skip to next operation, either current job time or time till new job becomes avaliable
            int timeToSkip;
            if (!priorityJobsR.empty())
            {
                int timeToNextJobFromR = priorityJobsR.top().r;
                timeToSkip = min(current.p, timeToNextJobFromR - t);
            }
            else
                timeToSkip = current.p;

            t += timeToSkip;
            current.p -= timeToSkip;

            // If during time skip job becomes avaliable, move it to priorityJobQ
            while (!priorityJobsR.empty() && priorityJobsR.top().r <= t)
            {
                priorityJobsQ.push(priorityJobsR.top());
                priorityJobsR.pop();
            }
            
            // Complete job
            if (current.p == 0)
            {
                best_permutation.push_back(*find_if(jobs.begin(), jobs.end(),[current](const Job &job){ return job.id == current.id; }));
                C_max = max(C_max, t + current.q);
                hasJob = false;
            }
            // If there is avaliable job that is more profitable, swap job (push current job back to priorityJobQ)
            else if (!priorityJobsQ.empty() && priorityJobsQ.top().q > current.q)
            {
                priorityJobsQ.push(current);
                hasJob = false;
            }
        }
        // If we don't have job and there are still jobs, but not avaliable, time skip
        else if (!priorityJobsR.empty())
        {
            t = priorityJobsR.top().r;
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

    printf("Czas wykonania: %d.%.3dus\n\n", (int)duration.count() / 1000, (int)duration.count() % 1000);
    print_Jobs(best_permutation);
    printf("C_max: %d\n\n", C_max);
}
