#include "Algorithms.h"
#include "Utilities.h"
#include "Comparators.h"
#include <queue>
#include <algorithm>
#include <cstdio>
#include <iostream>

using namespace std;

std::pair<Result, Result> bruteForce(vector<Job> jobs)
{

    Result best_result_rp;
    Result best_result_rpq;

    best_result_rp.C_max = compute_makespan_rp(jobs) + 1;
    best_result_rpq.C_max = compute_makespan_rpq(jobs) + 1;
    int x = 0;
    int facty = fact(jobs.size());
    for (int i = 0; i < facty; i++)
    {
        next_permutation(jobs.begin(), jobs.end(), [](const Job &a, const Job &b)
                         { return a.id > b.id; });
        int value_rp = compute_makespan_rp(jobs);
        if (value_rp < best_result_rp.C_max )
        {
            best_result_rp.C_max  = value_rp;
            best_result_rp.permutation= jobs;
        }
        int value_rpq = compute_makespan_rpq(jobs);
        if (value_rpq < best_result_rpq.C_max)
        {
            best_result_rpq.C_max = value_rpq;
            best_result_rpq.permutation = jobs;
        }
    }
    std::pair<Result, Result> best_result;
    best_result.first = best_result_rp;
    best_result.second = best_result_rpq;
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
    priority_queue<Job, vector<Job>, ComparePPlusQ> priorityJobsQ;
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

Result carlier(std::vector<Job> jobs, int UB) {
    
    // UB
    Result best_result = schrage(jobs);
    if (best_result.C_max < UB)
        UB = best_result.C_max;
    std::cout << "UB: " << UB << std::endl;

    /*// LB
    Result pmtn_result = preemptiveSchrage(jobs);
    int LB = pmtn_result.C_max;

    std::cout << "LB: " << LB << std::endl;

    // Check if we have a valid permutation
    if (best_result.permutation.empty()) {
        return best_result;
    }

    // Stop
    if (LB >= UB) {
        return best_result;
    }*/

    // Critical Path
    vector<Job> perm = best_result.permutation;
    int C_max = best_result.C_max;
    int b = -1, a = -1, c = -1;

    vector<int> perm_completationTimes(perm.size());
    vector<int> perm_startTimes(perm.size());
    int t = 0;
    for (int i = 0; i < perm.size(); i++) {
        perm_startTimes[i] = max(t, perm[i].r);
        perm_completationTimes[i] = perm_startTimes[i] + perm[i].p;
        t = perm_completationTimes[i];
    }
    
    // Find b
    for (int i = perm.size() - 1; i >= 0; i--) {
        if (perm_completationTimes[i] + perm[i].q == C_max) {
            b = i;
            break;
        }
    }
    //std::cout << "b: " << b << std::endl;
    // Znajdź a (pierwsze zadanie na ścieżce)
    int sum = 0;
    for (int i = 0; i <= b; i++) {
        int sum = 0;
        for (int s = i; s <= b; s++) {
            sum += perm[s].p;
        }
        if (perm[i].r + sum + perm[b].q == C_max) {
            a = i;
            break;
        }
    }
    //std::cout << "a: " << a << std::endl;
    // Znajdź c (pierwsze zadanie w [a, b] z q < q[b])
    for (int i = b; i >= a; i--) {
        if (perm[i].q < perm[b].q) {
            c = i;
            break;
        }
    }
    std::cout << "c: " << c << std::endl;
    // Check if c was found
    if (c == -1)
        return best_result;

    // Znajdź oryginalny indeks zadania c
    int original_c_id = perm[c].id;

    // Oblicz parametry podzbioru K = {c+1, ..., b}
    int r_K = INT_MAX, q_K = INT_MAX, p_K = 0;
    for (int i = c + 1; i <= b; i++) {
        r_K = std::min(r_K, perm[i].r);
        q_K = std::min(q_K, perm[i].q);
        p_K += perm[i].p;
    }
    int h_K = r_K + q_K + p_K;

    // Oblicz parametry podzbioru Kc = {c, ..., b}
    int r_Kc = INT_MAX, q_Kc = INT_MAX, p_Kc = 0;
    for (int i = c; i <= b; i++) {
        r_Kc = std::min(r_Kc, perm[i].r);
        q_Kc = std::min(q_Kc, perm[i].q);
        p_Kc += perm[i].p;
    }
    int h_Kc = r_Kc + q_Kc + p_Kc;

    // Lewe dziecko: modyfikuj r[c]
    Job original_job = jobs[original_c_id];
    jobs[original_c_id].r = std::max(jobs[original_c_id].r, r_K + p_K);
    
    // Przelicz LB dla lewej gałęzi
    Result pmtn_left = preemptiveSchrage(jobs);
    int LB_left = max(h_K, max(h_Kc, pmtn_left.C_max));
    
    if (LB_left < UB) {
        Result left_result = carlier(jobs, UB);
        if (left_result.C_max < best_result.C_max) {
            best_result = left_result;
            UB = best_result.C_max;
        }
    }
    std::cout << "pizda oko: " << c << std::endl;
    jobs[original_c_id].r = original_job.r; // Cofnij zmianę

    // Prawe dziecko: modyfikuj q[c]
    jobs[original_c_id].q = std::max(jobs[original_c_id].q, q_K + p_K);
    
    // Przelicz LB dla prawej gałęzi
    Result pmtn_right = preemptiveSchrage(jobs);
    int LB_right = max(h_K, max(h_Kc, pmtn_right.C_max));
    
    if (LB_right < UB) {
        Result right_result = carlier(jobs, UB);
        if (right_result.C_max < best_result.C_max) {
            best_result = right_result;
            UB = best_result.C_max;
        }
    }
    jobs[original_c_id].q = original_job.q; // Cofnij zmianę

    return best_result;
}