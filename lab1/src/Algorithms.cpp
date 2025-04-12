#include "Algorithms.h"
#include "Utilities.h"
#include "Comparators.h"
#include <queue>
#include <algorithm>
#include <cstdio>

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
    //printf("Czas wykonania: %d.%.3dus\n\n", (int)duration.count() / 1000, (int)duration.count() % 1000);
    //print_Jobs(best_permutation);
    //printf("C_max: %d\n\n", C_max);
}

Result carlier(vector<Job> jobs, int UB) {
    Result best_result;
    best_result.C_max = UB;

    // 1. Oblicz Schrage (górne ograniczenie)
    Result schrage_result = schrage(jobs);

    if (schrage_result.C_max < best_result.C_max) {
        best_result.permutation = schrage_result.permutation;
        best_result.C_max = schrage_result.C_max;
        UB = schrage_result.C_max;
    }

    // 2. Oblicz Schrage z przerwaniami (dolne ograniczenie)
    Result preemptiveSchrage_result = preemptiveSchrage(jobs);
    int LB = preemptiveSchrage_result.C_max;

    // 3. Warunek stopu
    if (LB >= UB) {
        return best_result;
    }

    // 4. Znajdź ścieżkę krytyczną
    int b = -1, a = -1, c = -1;
    int C_max = schrage_result.C_max;

    // Znajdź b
    for (int i = schrage_result.permutation.size() - 1; i >= 0; --i) {
        if (schrage_result.permutation[i].r + schrage_result.permutation[i].p + schrage_result.permutation[i].q == C_max) {
            b = i;
            break;
        }
    }

    // Znajdź a
    int sum_p = 0;
    for (int i = b; i >= 0; --i) {
        sum_p += schrage_result.permutation[i].p;
        if (schrage_result.permutation[i].r + sum_p + schrage_result.permutation[b].q == C_max) {
            a = i;
            break;
        }
    }

    // Znajdź c
    for (int i = a; i <= b; ++i) {
        if (schrage_result.permutation[i].q < schrage_result.permutation[b].q) {
            c = i;
            break;
        }
    }

    if (c == -1) {
        return best_result;
    }

    // 5. Oblicz parametry dla podzbioru K
    vector<Job> K;
    for (int i = c + 1; i <= b; ++i) {
        K.push_back(schrage_result.permutation[i]);
    }

    int r_p = INT_MAX, q_p = INT_MAX, p_p = 0;
    for (const auto& job : K) {
        r_p = min(r_p, job.r);
        q_p = min(q_p, job.q);
        p_p += job.p;
    }

    // 6. Rekurencja - lewe dziecko
    int original_r = jobs[c].r;
    jobs[c].r = max(jobs[c].r, r_p + p_p);
    int LB_left = r_p + p_p + q_p;
    
    if (LB_left < UB) {
        Result left_result = carlier(jobs, UB);
        if (left_result.C_max < best_result.C_max) {
            best_result = left_result;
            UB = left_result.C_max;
        }
    }
    jobs[c].r = original_r; // Cofnij zmianę

    // 7. Rekurencja - prawe dziecko
    int original_q = jobs[c].q;
    jobs[c].q = max(jobs[c].q, q_p + p_p);
    int LB_right = r_p + p_p + q_p;
    
    if (LB_right < UB) {
        Result right_result = carlier(jobs, UB);
        if (right_result.C_max < best_result.C_max) {
            best_result = right_result;
            UB = right_result.C_max;
        }
    }
    jobs[c].q = original_q; // Cofnij zmianę

    return best_result;
}