#include <iostream>
#include <vector>
#include <chrono>
#include "Structs.h"
#include "Algorithms.h"
#include "Utilities.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        return 1;
    }
    for (int i = 0; i < 14; i++)
{
    int n, m;
    std::vector<Job> jobs;

    if (!loadInstance(argv[1], i, n, m, jobs))
    {
        std::cerr << "Failed to load instance\n";
        return 2;
    }
    std::cout << "INSTANCJA "<< i << " - " << m << " MASZYN, " << n << " JOBÓW\n";
    printJobs(jobs, m);
    std::cout << "\n";

    if (n < 11)
    {
        auto start = std::chrono::steady_clock::now();
        ScheduleResult bruteForce_Result = bruteForce(jobs, m);
        auto end = std::chrono::steady_clock::now();
        auto duration = end - start;
        auto sec = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() % 1000;

        std::cout << "Brute Force solution C_max: " << bruteForce_Result.C_max 
                  << " (czas: " << sec << "s " << ms << "ms)\nBrute Force Permutation:\n";
        for (int jobId : bruteForce_Result.permutation)
        {
            std::cout << jobId << " ";
        }
        std::cout << "\n\n";
    }

    {
        auto start = std::chrono::steady_clock::now();
        ScheduleResult NEH_Result = NEH(jobs, m);
        auto end = std::chrono::steady_clock::now();
        auto duration = end - start;
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
        auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count() % 1000000;

        std::cout << "NEH solution C_max: " << NEH_Result.C_max 
                  << " (czas: " << ms << "ms " << ns << "ns)\nNEH Permutation:\n";
        for (int jobId : NEH_Result.permutation)
        {
            std::cout << jobId << " ";
        }
        std::cout << "\n\n";
    }

    {
        auto start = std::chrono::steady_clock::now();
        ScheduleResult Johnson_Result = johnson(jobs);
        auto end = std::chrono::steady_clock::now();
        auto duration = end - start;
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
        auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count() % 1000000;

        std::cout << "Johnson dla m = 2 solution C_max: " << Johnson_Result.C_max 
                  << " (czas: " << ms << "ms " << ns << "ns)\nJohnson dla m = 2 Permutation:\n";
        for (int jobId : Johnson_Result.permutation)
        {
            std::cout << jobId << " ";
        }
        std::cout << "\n\n";
    }

    {
        auto start = std::chrono::steady_clock::now();
        ScheduleResult FNEH_Result = FNEH(jobs, m);
        auto end = std::chrono::steady_clock::now();
        auto duration = end - start;
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
        auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count() % 1000000;

        std::cout << "FNEH solution C_max: " << FNEH_Result.C_max 
                  << " (czas: " << ms << "ms " << ns << "ns)\nFNEH Permutation:\n";
        for (int jobId : FNEH_Result.permutation)
        {
            std::cout << jobId << " ";
        }
        std::cout << "\n\n";
    }

    if (n < 11)
    {
        auto start = std::chrono::steady_clock::now();
        ScheduleResult BnB_Result = branchAndBound(jobs, m);
        auto end = std::chrono::steady_clock::now();
        auto duration = end - start;
        auto sec = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() % 1000;

        std::cout << "BnB solution C_max: " << BnB_Result.C_max 
                  << " (czas: " << sec << "s " << ms << "ms)\nBnB Permutation:\n";
        for (int jobId : BnB_Result.permutation)
        {
            std::cout << jobId << " ";
        }
        std::cout << "\n\n";
    }

    {
        auto start = std::chrono::steady_clock::now();
        ScheduleResult TS_Result = tabuSearch(jobs, m, 100, 10);
        auto end = std::chrono::steady_clock::now();
        auto duration = end - start;
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
        auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count() % 1000000;

        std::cout << "Tabu Search solution C_max: " << TS_Result.C_max 
                  << " (czas: " << ms << "ms " << ns << "ns)\nTabu Search Permutation:\n";
        for (int jobId : TS_Result.permutation)
        {
            std::cout << jobId << " ";
        }
        std::cout << "\n\n";
    }

    {
        auto start = std::chrono::steady_clock::now();
        ScheduleResult SA_Result = simulatedAnnealing(jobs, m, 1000, 135.0, 0.95);
        auto end = std::chrono::steady_clock::now();
        auto duration = end - start;
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
        auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count() % 1000000;

        std::cout << "Simulated Annealing solution C_max: " << SA_Result.C_max 
                  << " (czas: " << ms << "ms " << ns << "ns)\nSimulated Annealing Permutation:\n";
        for (int jobId : SA_Result.permutation)
        {
            std::cout << jobId << " ";
        }
        std::cout << "\n\n-------------------------------------------------------------------------------------------------------------------------\n\n";
    }
}

    return 0;
}
