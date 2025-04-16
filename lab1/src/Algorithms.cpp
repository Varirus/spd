#include "Algorithms.h"
#include "Utilities.h"
#include "Comparators.h"
#include <queue>
#include <algorithm>
#include <cstdio>
#include <iostream>

using namespace std;

Result sort_r(vector<Job> jobs){
    Result best_result;
    vector<Job>sorted_jobs = sort_by_r(jobs);
    int cmax = compute_makespan(sorted_jobs);
    best_result.permutation = sorted_jobs;
    best_result.C_max = cmax;
    return best_result;
}

Result sort_q(vector<Job> jobs){
    Result best_result;
    vector<Job>sorted_jobs = sort_by_q(jobs);
    int cmax = compute_makespan(sorted_jobs);
    best_result.permutation = sorted_jobs;
    best_result.C_max = cmax;
    return best_result;
}

Result bruteForce(vector<Job> jobs)
{

    Result best_result;

    best_result.C_max = compute_makespan(jobs) + 1;
    int x = 0;
    int facty = fact(jobs.size());
    for (int i = 0; i < facty; i++)
    {
        next_permutation(jobs.begin(), jobs.end(), [](const Job &a, const Job &b)
                         { return a.id > b.id; });
        int value = compute_makespan(jobs);
        if (value < best_result.C_max )
        {
            best_result.C_max  = value;
            best_result.permutation= jobs;
        }
    }
    return best_result;
}

Result schrage(vector<Job> jobs)
{
    Result best_result;
    priority_queue<Job, vector<Job>, CompareR_Rev> priorityJobsR(jobs.begin(), jobs.end());
    priority_queue<Job, vector<Job>, CompareQ_Rev> priorityJobsQ;
    best_result.C_max = 0;
    int t = 0;

    while (!(priorityJobsR.empty()) || !(priorityJobsQ.empty()))
    {
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
            best_result.permutation.push_back(current);
            t += current.p;
            best_result.C_max = max(best_result.C_max, t + current.q);
        }
    }
    return best_result;
}

Result preemptiveSchrage(vector<Job> jobs)
{
    Result best_result;
    priority_queue<Job, vector<Job>, CompareR_Rev> priorityJobsR(jobs.begin(), jobs.end());
    priority_queue<Job, vector<Job>, CompareQ_Rev> priorityJobsQ;
    best_result.C_max = 0;
    int t = 0;
    Job current;
    bool hasJob = false;
    

    while (!(priorityJobsR.empty()) || !(priorityJobsQ.empty()) || hasJob)
    {
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
                best_result.permutation.push_back(*find_if(jobs.begin(), jobs.end(),[current](const Job &job){ return job.id == current.id; }));
                best_result.C_max = max(best_result.C_max, t + current.q);
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
    return best_result;
}

Result ownAlgorithm(vector<Job> jobs) {
    Result best_result;
    best_result.C_max = 0;

    priority_queue<Job, vector<Job>, CompareR> priorityJobsN(jobs.begin(), jobs.end());
    priority_queue<Job, vector<Job>, ComparePPlusQ> priorityJobsG;

    int t = 0;
    Job current;
    bool hasJob = false;

    while (!priorityJobsN.empty() || !priorityJobsG.empty() || hasJob) {
        // Move jobs from N to G where r <= t
        while (!priorityJobsN.empty() && priorityJobsN.top().r <= t) {
            priorityJobsG.push(priorityJobsN.top());
            priorityJobsN.pop();
        }

        if (hasJob) {
            // Process current job for (p-1) units
            int timeToProcess = current.p - 1;
            t += timeToProcess;
            current.p -= timeToProcess; // Remaining p is 1

            // Move new jobs from N to G
            while (!priorityJobsN.empty() && priorityJobsN.top().r <= t) {
                priorityJobsG.push(priorityJobsN.top());
                priorityJobsN.pop();
            }

            // Check if any job in G has (p + q) < (current.p + current.q)
            bool preempt = false;
            if (!priorityJobsG.empty()) {
                Job topJob = priorityJobsG.top();
                if ((topJob.p + topJob.q) < (current.p + current.q)) {
                    preempt = true;
                }
            }

            if (preempt) {
                priorityJobsG.push(current);
                current = priorityJobsG.top();
                priorityJobsG.pop();
                // Continue processing the new current job
            } else {
                // Process remaining 1 unit
                t += 1;
                current.p -= 1; // p becomes 0
                best_result.C_max = max(best_result.C_max, t + current.q);
                // Add to permutation if needed
                best_result.permutation.push_back(current);
                hasJob = false;
            }
        } else if (!priorityJobsG.empty()) {
            // Select job with smallest p + q
            current = priorityJobsG.top();
            priorityJobsG.pop();
            hasJob = true;
        } else {
            // Jump to the next release time
            if (!priorityJobsN.empty()) {
                t = priorityJobsN.top().r;
            } else {
                // All jobs are processed
                break;
            }
        }
    }

    return best_result;
}

Result ownAlgorithm2(vector<Job> jobs)
{
    Result best_result;
    priority_queue<Job, vector<Job>, CompareR_Rev> priorityJobsR(jobs.begin(), jobs.end());
    priority_queue<Job, vector<Job>, CompareMinusPPlusQ> priorityJobsQ;
    best_result.C_max = 0;
    int t = 0;
    Job current;
    bool hasJob = false;
    

    while (!(priorityJobsR.empty()) || !(priorityJobsQ.empty()) || hasJob)
    {
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
                best_result.permutation.push_back(*find_if(jobs.begin(), jobs.end(),[current](const Job &job){ return job.id == current.id; }));
                best_result.C_max = max(best_result.C_max, t + current.q);
                hasJob = false;
            }
            // If there is avaliable job that is more profitable, swap job (push current job back to priorityJobQ)
            else if (!priorityJobsQ.empty() && (priorityJobsQ.top().q + priorityJobsQ.top().p) > (current.q + current.p))
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
    return best_result;
}

void carlier(vector<Job>& jobs, Result& best_result) {
    
    Result current_result = schrage(jobs);
    
    if (current_result.C_max < best_result.C_max) {
        best_result = current_result;
    }
    vector<Job>& perm = current_result.permutation;

    // Creating completion times vector
    vector<int> completion_times(perm.size());
    vector<int> start_times(perm.size());
    int t = 0;
    for (int i = 0; i < perm.size(); i++) {
        start_times[i] = max(t, perm[i].r);
        completion_times[i] = start_times[i] + perm[i].p;
        t = completion_times[i];
    }
    // Find b
    int b = -1;
    int C_max = current_result.C_max;
    for (int i = perm.size() - 1; i >= 0; i--) {
        if (completion_times[i] + perm[i].q == C_max) {
            b = i;
            break;
        }
    }

    // Find a
    int a = -1;
    for (int i = 0; i <= b; i++) {
        int sum_p = 0;
        for (int j = i; j <= b; j++) {
            sum_p += perm[j].p;
        }
        if (perm[i].r + sum_p + perm[b].q == C_max) {
            a = i;
            break;
        }
    }

    // Find c
    int c = -1;
    for (int i = b; i >= a; i--) {
        if (perm[i].q < perm[b].q) {
            c = i;
            break;
        }
    }
    // Find c
    if (c == -1)
        return;

    // K parameters, h_K and h_Kc
    int r_K = INT_MAX, q_K = INT_MAX, p_K = 0;
    for (int i = c + 1; i <= b; i++) {
        r_K = min(r_K, perm[i].r);
        q_K = min(q_K, perm[i].q);
        p_K += perm[i].p;
    }
    int h_K = r_K + p_K + q_K;

    int r_Kc = INT_MAX, q_Kc = INT_MAX, p_Kc = 0;
    for (int i = c; i <= b; i++) {
        r_Kc = min(r_Kc, perm[i].r);
        q_Kc = min(q_Kc, perm[i].q);
        p_Kc += perm[i].p;
    }
    int h_Kc = r_Kc + p_Kc + q_Kc;

    // Finding job by id
    int job_id = perm[c].id;
    Job& jc = *std::find_if(jobs.begin(), jobs.end(), [&](const Job& j) { return j.id == job_id; });

    // Left - modify r
    int original_r = jc.r;
    jc.r = max(jc.r, r_K + p_K);

    Result pmtn_left = preemptiveSchrage(jobs);
    int LB_left = max({h_K, h_Kc, pmtn_left.C_max});

    if (LB_left < best_result.C_max) {
        carlier(jobs, best_result);
    }

    // Revert r
    jc.r = original_r;

    // Right - modify q
    int original_q = jc.q;
    jc.q = max(jc.q, q_K + p_K);

    Result pmtn_right = preemptiveSchrage(jobs);
    int LB_right = max({h_K, h_Kc, pmtn_right.C_max});

    if (LB_right < best_result.C_max) {
        carlier(jobs, best_result);
    }

    // Revert q
    jc.q = original_q;
}

Result solveCarlier(vector<Job> jobs) {

    Result initial_result = schrage(jobs);
    Result best_result = initial_result;
    
    carlier(jobs, best_result);
    
    return best_result;
}