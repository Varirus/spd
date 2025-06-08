#include <iostream>
#include <vector>
#include "Structs.h"
#include "Algorithms.h"
#include "Utilities.h"

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        return 1;
    }
    int instanceIndex = strtol(argv[2], NULL, 10);
    int n, m;
    std::vector<Job> jobs;

    if (!loadInstance(argv[1], instanceIndex, n, m, jobs))
    {
        std::cerr << "Failed to load instance\n";
        return 2;
    }
    printJobs(jobs, m);
    std::cout << "\n";
    if (n < 11)
    {
        ScheduleResult bruteForce_Result = bruteForce(jobs, m);
        std::cout << "Brute Force solution C_sum: " << calculateCsum(jobs, bruteForce_Result.permutation, m) << "\n";
        std::cout << "Brute Force solution C_max: " << bruteForce_Result.C_max << "\nBrute Force Permutation:\n";
        for (int jobId : bruteForce_Result.permutation)
        {
            std::cout << jobId << " ";
        }
        std::cout << "\n\n";
    }

    ScheduleResult NEH_Result = NEH(jobs, m);
    std::cout << "NEH solution C_sum: " << calculateCsum(jobs, NEH_Result.permutation, m) << "\n";
    std::cout << "NEH solution C_max: " << NEH_Result.C_max << "\nNEH Permutation:\n";
    for (int jobId : NEH_Result.permutation)
    {
        std::cout << jobId << " ";
    }
    std::cout << "\n\n";

    ScheduleResult Johnson_Result = johnson(jobs);
    std::cout << "Johnson dla m = 2 solution C_sum: " << calculateCsum(jobs, Johnson_Result.permutation, 2) << "\n";
    std::cout << "Johnson dla m = 2 C_max: " << Johnson_Result.C_max << "\nJohnson dla m = 2 Permutation:\n";
    for (int jobId : Johnson_Result.permutation)
    {
        std::cout << jobId << " ";
    }
    std::cout << "\n\n";

    ScheduleResult FNEH_Result = FNEH(jobs, m);
    std::cout << "FNEH solution C_sum: " << calculateCsum(jobs, FNEH_Result.permutation, m) << "\n";
    std::cout << "FNEH solution C_max: " << FNEH_Result.C_max << "\nFNEH Permutation:\n";
    for (int jobId : FNEH_Result.permutation)
    {
        std::cout << jobId << " ";
    }
    std::cout << "\n\n";
    if (n < 11)
    {
        ScheduleResult BnB_Result = branchAndBound(jobs, m);
        std::cout << "BnB solution C_sum: " << calculateCsum(jobs, BnB_Result.permutation, m) << "\n";
        std::cout << "BnB solution C_max: " << BnB_Result.C_max << "\nBnB Permutation:\n";
        for (int jobId : BnB_Result.permutation)
        {
            std::cout << jobId << " ";
        }
        std::cout << "\n\n";
    }

    ScheduleResult TS_Result = tabuSearch(jobs, m, 100, 10);
    std::cout << "Tabu Search solution C_sum: " << calculateCsum(jobs, TS_Result.permutation, m) << "\n";
    std::cout << "Tabu Search solution C_max: " << TS_Result.C_max << "\nTabu Search Permutation:\n";
    for (int jobId : TS_Result.permutation)
    {
        std::cout << jobId << " ";
    }
    std::cout << "\n\n";

    ScheduleResult SA_Result = simulatedAnnealing(jobs, m, 1000, 135.0, 0.95);
    std::cout << "Simulated Annealing solution C_sum: " << calculateCsum(jobs, SA_Result.permutation, m) << "\n";
    std::cout << "Simulated Annealing solution C_max: " << SA_Result.C_max << "\nSimulated Annealing Permutation:\n";
    for (int jobId : SA_Result.permutation)
    {
        std::cout << jobId << " ";
    }
    std::cout << "\n\n";

    return 0;
}
