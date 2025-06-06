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

Result solveP2Cmax_PZ(std::vector<Job> original_jobs)
{
    std::vector<Job> jobs = original_jobs;
    int n = jobs.size();
    int bestCmax = INT_MAX;
    std::vector<Machine> bestMachines(2);
    // Tylko pierwsze 2^n / 2 zadan bo potem powtorzenia
    int total = 1 << n;
    int limit = total / 2;

    for (int i = 0; i < limit; ++i) {
        std::vector<Machine> machines(2);
        for (int x = 0; x < 2; x++)
        {
            machines[x].id = x;
        }

        // Ciag binarny po ktorym wkladamy dane zadania na maszyny
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

Result solveP2Cmax_PD(std::vector<Job> original_jobs)
{
    std::vector<Job> jobs = original_jobs;
    int total_sum = 0;
    for (const auto &job : jobs)
        total_sum += job.p;
    int target = total_sum / 2;

    std::vector<std::vector<bool>> dp(jobs.size() + 1, std::vector<bool>(target + 1, false));
    dp[0][0] = true;

    // Wypelenienie tablicy programowanie dynammiczny
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

    // Znajdywanie najlepszego k
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

    // Backtracking
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


Result solveP3Cmax_PZ(std::vector<Job> original_jobs) {
    std::vector<Job> jobs = original_jobs;
    int n = jobs.size();
    int bestCmax = INT_MAX;
    std::vector<Machine> bestMachines(3);

    for (int a = 0; a <= n; ++a) {
        for (int b = a; b <= n - a; ++b) {
            int c = n - a - b;
            if (b > c) continue;

            std::vector<int> counts = {a, b, c};

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

Result solveP2Cmax_FPTAS(std::vector<Job> original_jobs, int k) {
    int n = original_jobs.size();
    k = std::min(k, n);

    std::sort(original_jobs.begin(), original_jobs.end(), [](const Job& a, const Job& b) {
        return a.p > b.p;
    });

    std::vector<Job> topK(original_jobs.begin(), original_jobs.begin() + k);
    std::vector<Job> rest(original_jobs.begin() + k, original_jobs.end());

    int bestCmax = INT_MAX;
    std::vector<Job> bestM1, bestM2;

    int S_rest = 0;
    for (const auto& job : rest) {
        S_rest += job.p;
    }

    for (int mask = 0; mask < (1 << k); ++mask) {
        std::vector<Job> m1, m2;
        int sum1 = 0, sum2 = 0;

        for (int i = 0; i < k; ++i) {
            if (mask & (1 << i)) {
                m1.push_back(topK[i]);
                sum1 += topK[i].p;
            } else {
                m2.push_back(topK[i]);
                sum2 += topK[i].p;
            }
        }

        if (!rest.empty()) {
            double epsilon = 1.0 / k;
            int m = rest.size();
            double K_val = (epsilon * S_rest) / (2.0 * m);
            K_val = std::max(K_val, 1.0);

            std::vector<Job> scaled = rest;
            for (auto& job : scaled) {
                job.p = static_cast<int>(job.p / K_val);
            }

            int total_scaled = 0;
            for (const auto& job : scaled) {
                total_scaled += job.p;
            }

            std::vector<bool> dp(total_scaled + 1, false);
            std::vector<int> parent(total_scaled + 1, -1);
            dp[0] = true;

            for (size_t i = 0; i < scaled.size(); ++i) {
                int p = scaled[i].p;
                for (int j = total_scaled; j >= p; --j) {
                    if (dp[j - p] && !dp[j]) {
                        dp[j] = true;
                        parent[j] = i;
                    }
                }
            }

            int target = total_scaled / 2;
            while (target >= 0 && !dp[target]) {
                --target;
            }

            std::vector<bool> onM1(rest.size(), false);
            int current = target;
            while (current > 0) {
                int job_idx = parent[current];
                if (job_idx == -1) break;
                onM1[job_idx] = true;
                current -= scaled[job_idx].p;
            }

            for (size_t i = 0; i < rest.size(); ++i) {
                if (onM1[i]) {
                    m1.push_back(rest[i]);
                    sum1 += rest[i].p;
                } else {
                    m2.push_back(rest[i]);
                    sum2 += rest[i].p;
                }
            }
        }

        int currentCmax = std::max(sum1, sum2);
        if (currentCmax < bestCmax) {
            bestCmax = currentCmax;
            bestM1 = m1;
            bestM2 = m2;
        }
    }

    Machine m1 = {0, 0, bestM1}, m2 = {1, 0, bestM2};
    for (const auto &job : bestM1) m1.maxTime += job.p;
    for (const auto &job : bestM2) m2.maxTime += job.p;

    return {{m1, m2}, bestCmax};
}

Result solveP2Cmax_PTAS(std::vector<Job> original_jobs, int k) {
    std::vector<Job> jobs = original_jobs;
    int n = jobs.size();
    std::sort(jobs.begin(), jobs.end(), [](const Job& a, const Job& b) { return a.p > b.p; });

    long long bestCmax = std::numeric_limits<long long>::max();
    std::vector<Machine> bestMachines(2);
    for (int x = 0; x < 2; x++) {
        bestMachines[x].id = x;
    }

    long long total = 1;
    for (int j = 0; j < k; j++) {
        total *= 2;
        if (total > 1e9) break;
    }
    for (int i = 0; i < total; i++) {
        std::vector<Job> jobs0, jobs1;
        long long load0 = 0, load1 = 0;
        for (int j = 0; j < k; j++) {
            int machine = (i >> j) & 1;
            if (machine == 0) {
                jobs0.push_back(jobs[j]);
                load0 += jobs[j].p;
            } else {
                jobs1.push_back(jobs[j]);
                load1 += jobs[j].p;
            }
        }
        long long current_load0 = load0;
        long long current_load1 = load1;
        std::vector<Job> temp_jobs0 = jobs0;
        std::vector<Job> temp_jobs1 = jobs1;
        for (int j = k; j < n; j++) {
            if (current_load0 <= current_load1) {
                temp_jobs0.push_back(jobs[j]);
                current_load0 += jobs[j].p;
            } else {
                temp_jobs1.push_back(jobs[j]);
                current_load1 += jobs[j].p;
            }
        }
        long long Cmax = std::max(current_load0, current_load1);
        if (Cmax < bestCmax) {
            bestCmax = Cmax;
            bestMachines[0].jobs = temp_jobs0;
            bestMachines[0].maxTime = current_load0;
            bestMachines[1].jobs = temp_jobs1;
            bestMachines[1].maxTime = current_load1;
        }
    }
    Result result;
    result.perm = bestMachines;
    result.C_max = bestCmax;
    return result;
}

Result solveP3Cmax_PTAS(std::vector<Job> original_jobs, int k) {
    std::vector<Job> jobs = original_jobs;
    int n = jobs.size();
    std::sort(jobs.begin(), jobs.end(), [](const Job& a, const Job& b) { return a.p > b.p; });

    int bestCmax = std::numeric_limits<int>::max();
    std::vector<Machine> bestMachines(3);
    for (int x = 0; x < 3; x++) {
        bestMachines[x].id = x;
    }

    long long total = 1;
    for (int j = 0; j < k; j++) {
        total *= 3;
        if (total > 1e9) break;
    }

    for (long long i = 0; i < total; i++) {
        std::vector<Job> jobs0, jobs1, jobs2;
        int load0 = 0, load1 = 0, load2 = 0;
        long long temp = i;
        for (int j = 0; j < k; j++) {
            int machine = temp % 3;
            temp /= 3;
            if (machine == 0) {
                jobs0.push_back(jobs[j]);
                load0 += jobs[j].p;
            } else if (machine == 1) {
                jobs1.push_back(jobs[j]);
                load1 += jobs[j].p;
            } else {
                jobs2.push_back(jobs[j]);
                load2 += jobs[j].p;
            }
        }
        int current_load0 = load0;
        int current_load1 = load1;
        int current_load2 = load2;
        std::vector<Job> temp_jobs0 = jobs0;
        std::vector<Job> temp_jobs1 = jobs1;
        std::vector<Job> temp_jobs2 = jobs2;
        for (int j = k; j < n; j++) {
            int min_load = std::min({current_load0, current_load1, current_load2});
            if (current_load0 == min_load) {
                temp_jobs0.push_back(jobs[j]);
                current_load0 += jobs[j].p;
            } else if (current_load1 == min_load) {
                temp_jobs1.push_back(jobs[j]);
                current_load1 += jobs[j].p;
            } else {
                temp_jobs2.push_back(jobs[j]);
                current_load2 += jobs[j].p;
            }
        }
        int Cmax = std::max({current_load0, current_load1, current_load2});
        if (Cmax < bestCmax) {
            bestCmax = Cmax;
            bestMachines[0].jobs = temp_jobs0;
            bestMachines[0].maxTime = current_load0;
            bestMachines[1].jobs = temp_jobs1;
            bestMachines[1].maxTime = current_load1;
            bestMachines[2].jobs = temp_jobs2;
            bestMachines[2].maxTime = current_load2;
        }
    }
    Result result;
    result.perm = bestMachines;
    result.C_max = bestCmax;
    return result;
}