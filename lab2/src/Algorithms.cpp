#include "Algorithms.h"
#include "Utilities.h"
#include "Comparators.h"
#include <set>
#include <algorithm>
#include <cstdio>
#include <iostream>

/*std::pair<Result, Result> bruteForce(vector<Job> jobs)
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
}*/

Result lsa(int m, std::vector<Job> original_jobs){
    std::vector<Job> jobs = original_jobs;
    std::set<Machine> machines;
    for (int i = 0; i < m; i++)
    {
        Machine machine;
        machine.id = i;
        machine.timeLeft = 0;
        machines.emplace(machine);
    }
    int t = 0;
    while(!jobs.empty() || machines.begin()->timeLeft > 0){
        if(machines.begin()->timeLeft > 0){
            int timeToSkip = machines.begin()->timeLeft;
            t += timeToSkip;
            std::set<Machine> updatedMachines;
            for (const auto& machine : machines) {
                Machine updated = machine;
                updated.timeLeft -= timeToSkip;
                updatedMachines.insert(updated);
            }
            machines = std::move(updatedMachines);
        }
        else if(!jobs.empty()){
            while(machines.begin()->timeLeft  == 0){
                Machine first_machine = *machines.begin();
                first_machine.jobs.push_back(jobs[0]);
                first_machine.timeLeft += jobs[0].p;
                machines.erase(machines.begin());
                machines.insert(first_machine);
                jobs.erase(jobs.begin());
            }
        }
        else if(jobs.empty()){
            int timeToSkip = machines.rbegin()->timeLeft;
            t += timeToSkip;
            std::set<Machine> updatedMachines;
            for (const auto& machine : machines) {
                Machine updated = machine;
                updated.timeLeft = 0;
                updatedMachines.insert(updated);
            }
            machines = std::move(updatedMachines);
        }
    }
    Result result;
    result.C_max = t;
    std::vector<Machine> machines_result(machines.begin(), machines.end());
    //machines_result.insert(machines.begin(), machines.end());
    result.perm = machines_result;
    return result;
}
