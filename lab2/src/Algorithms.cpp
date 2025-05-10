#include "Algorithms.h"
#include "Utilities.h"
#include "Comparators.h"
#include <set>
#include <queue>
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <cmath>

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

Result solveP2Cmax_PD(std::vector<Job> original_jobs)
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

Result solveP3Cmax_PD(std::vector<Job> original_jobs)
{
    std::vector<Job> jobs = original_jobs;
    int total_sum = 0;
    for (const auto &job : jobs)
        total_sum += job.p;

    int n = jobs.size();
    int limit = total_sum;

    // 3D tablica: dp[j][s1][s2] = czy możliwe jest osiągnięcie sum s1 i s2 przy pierwszych j zadaniach
    std::vector<std::vector<std::vector<bool>>> dp(n + 1,
        std::vector<std::vector<bool>>(limit + 1, std::vector<bool>(limit + 1, false)));
    dp[0][0][0] = true;

    for (int j = 1; j <= n; ++j)
    {
        int p = jobs[j - 1].p;
        for (int s1 = 0; s1 <= limit; ++s1)
        {
            for (int s2 = 0; s2 <= limit; ++s2)
            {
                if (dp[j - 1][s1][s2])
                {
                    dp[j][s1][s2] = true;
                    if (s1 + p <= limit) dp[j][s1 + p][s2] = true;
                    if (s2 + p <= limit) dp[j][s1][s2 + p] = true;
                }
            }
        }
    }

    int bestCmax = total_sum;
    int best_s1 = 0, best_s2 = 0;

    for (int s1 = 0; s1 <= limit; ++s1)
    {
        for (int s2 = 0; s2 <= limit - s1; ++s2)
        {
            if (dp[n][s1][s2])
            {
                int s3 = total_sum - s1 - s2;
                int Cmax = std::max({s1, s2, s3});
                if (Cmax < bestCmax)
                {
                    bestCmax = Cmax;
                    best_s1 = s1;
                    best_s2 = s2;
                }
            }
        }
    }

    Machine m1, m2, m3;
    m1.id = 0; m2.id = 1; m3.id = 2;
    m1.maxTime = m2.maxTime = m3.maxTime = 0;

    int s1 = best_s1, s2 = best_s2;

    // Odtwarzanie przydziału zadań do maszyn
    for (int j = n; j >= 1; --j)
    {
        int p = jobs[j - 1].p;
        if (s1 >= p && dp[j - 1][s1 - p][s2])
        {
            m1.jobs.push_back(jobs[j - 1]);
            m1.maxTime += p;
            s1 -= p;
        }
        else if (s2 >= p && dp[j - 1][s1][s2 - p])
        {
            m2.jobs.push_back(jobs[j - 1]);
            m2.maxTime += p;
            s2 -= p;
        }
        else
        {
            m3.jobs.push_back(jobs[j - 1]);
            m3.maxTime += p;
        }
    }

    Result result;
    result.perm.push_back(m1);
    result.perm.push_back(m2);
    result.perm.push_back(m3);
    result.C_max = bestCmax;

    return result;
}

Result solveP2Cmax_PZ(std::vector<Job> original_jobs)
{
    std::vector<Job> jobs = original_jobs;
    int n = jobs.size();
    int bestCmax = INT_MAX;
    std::vector<Machine> bestMachines(2);
    int total = 1 << n;
    int limit = total / 2;

    for (int i = 0; i < limit; ++i) {
        std::vector<Machine> machines(2);
        for (int x = 0; x < 2; x++)
        {
            machines[x].id = x;
        }

        std::vector<int> assignment(n);
        for (int j = 0; j < n; ++j) {
            assignment[j] = (i >> j) & 1;
        }

        for (int k = 0; k < n; ++k) {
            machines.at(assignment[k]).jobs.push_back(jobs[k]);
            machines.at(assignment[k]).maxTime += jobs[k].p;
        }
        int Cmax = std::max(machines.at(0).maxTime, machines.at(1).maxTime);
        if(Cmax < bestCmax){
            bestCmax = Cmax;
            bestMachines.at(0) = machines.at(0);
            bestMachines.at(1) = machines.at(1);
        }
    }
    Result result;
    result.perm.push_back(bestMachines.at(0));
    result.perm.push_back(bestMachines.at(1));
    result.C_max = bestCmax;
    return result;
}

Result solveP3Cmax_PZ(std::vector<Job> original_jobs) {
    std::vector<Job> jobs = original_jobs;
    int n = jobs.size();
    int bestCmax = INT_MAX;
    std::vector<Machine> bestMachines(3);

    // Generate all combinations (a, b, c) with a + b + c = n and a <= b <= c
    for (int a = 0; a <= n; ++a) {
        for (int b = a; b <= n - a; ++b) {
            int c = n - a - b;
            if (b > c) continue;

            // All valid patterns (a <= b <= c)
            std::vector<int> counts = {a, b, c};

            // Generate all possible assignments of n jobs split into groups of size a, b, c
            std::vector<int> assignment(n);
            std::fill(assignment.begin(), assignment.begin() + a, 0);
            std::fill(assignment.begin() + a, assignment.begin() + a + b, 1);
            std::fill(assignment.begin() + a + b, assignment.end(), 2);

            do {
                std::vector<Machine> machines(3);
                for (int i = 0; i < 3; ++i) machines[i].id = i;

                for (int k = 0; k < n; ++k) {
                    int m = assignment[k];
                    machines[m].jobs.push_back(jobs[k]);
                    machines[m].maxTime += jobs[k].p;
                }

                int Cmax = std::max({machines[0].maxTime, machines[1].maxTime, machines[2].maxTime});
                if (Cmax < bestCmax) {
                    bestCmax = Cmax;
                    bestMachines = machines;
                }

            } while (std::next_permutation(assignment.begin(), assignment.end()));
        }
    }

    Result result;
    for (const auto& m : bestMachines)
        result.perm.push_back(m);

    result.C_max = bestCmax;
    return result;
}



Result solveP2Cmax_PTAS(std::vector<Job> original_jobs, double epsilon)
{
    int n = original_jobs.size();
    int k = std::min((int)std::ceil(1.0 / epsilon), n);

    // Posortuj po czasie przetwarzania malejąco
    std::vector<Job> jobs = original_jobs;
    std::sort(jobs.begin(), jobs.end(), [](const Job &a, const Job &b) {
        return a.p > b.p;
    });

    std::vector<Job> topK(jobs.begin(), jobs.begin() + k);
    std::vector<Job> rest(jobs.begin() + k, jobs.end());

    int bestCmax = INT_MAX;
    std::vector<Job> bestM1, bestM2;

    // Przejdź przez wszystkie możliwe przydziały k największych zadań
    for (int mask = 0; mask < (1 << k); ++mask)
    {
        std::vector<Job> m1_jobs, m2_jobs;
        int sum1 = 0, sum2 = 0;

        for (int i = 0; i < k; ++i)
        {
            if (mask & (1 << i))
            {
                m1_jobs.push_back(topK[i]);
                sum1 += topK[i].p;
            }
            else
            {
                m2_jobs.push_back(topK[i]);
                sum2 += topK[i].p;
            }
        }

        // Zachłannie przydziel pozostałe zadania
        for (const Job &job : rest)
        {
            if (sum1 <= sum2)
            {
                m1_jobs.push_back(job);
                sum1 += job.p;
            }
            else
            {
                m2_jobs.push_back(job);
                sum2 += job.p;
            }
        }

        int Cmax = std::max(sum1, sum2);
        if (Cmax < bestCmax)
        {
            bestCmax = Cmax;
            bestM1 = m1_jobs;
            bestM2 = m2_jobs;
        }
    }

    // Stwórz wynik
    Machine m1, m2;
    m1.id = 0;
    m2.id = 1;
    m1.maxTime = m2.maxTime = 0;

    for (const auto &job : bestM1)
    {
        m1.jobs.push_back(job);
        m1.maxTime += job.p;
    }

    for (const auto &job : bestM2)
    {
        m2.jobs.push_back(job);
        m2.maxTime += job.p;
    }

    Result result;
    result.perm.push_back(m1);
    result.perm.push_back(m2);
    result.C_max = bestCmax;
    return result;
}