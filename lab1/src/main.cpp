#include <iostream>
#include <vector>
#include "Structs.h"
#include "Algorithms.h"
#include "Utilities.h"
#include <chrono>

using namespace std;

int main(int argc, char *argv[])
{
    vector<Job> jobs = readInput(argc, argv);

    printf("------------------------------\n");
    printf("Input:\n");
    print_Jobs(jobs);

    printf("------------------------------\n");

    printf("Schrage:\n");
    auto start = std::chrono::high_resolution_clock::now();
    Result schrage_result = schrage(jobs);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    printf("Time: %ldns\n", duration.count());
    printf("C_max: %d\n", schrage_result.C_max);
    printf("Schedule:\n");
    print_Jobs(schrage_result.permutation);

    printf("------------------------------\n");

    printf("Preemptive Schrage:\n");
    start = std::chrono::high_resolution_clock::now();
    Result preemptiveSchrage_result = preemptiveSchrage(jobs);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    printf("Time: %ldns\n", duration.count());
    printf("C_max: %d\n", preemptiveSchrage_result.C_max);
    printf("Schedule:\n");
    print_Jobs(preemptiveSchrage_result.permutation);
    
    if (jobs.size() < 11)
    {
        printf("------------------------------\n");
        printf("Brute Force:\n");
        printf("------------------------------\n");

        start = std::chrono::high_resolution_clock::now();
        std::pair<Result, Result> bruteForce_result = bruteForce(jobs);
        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        printf("Brute Force RP:\n");
        printf("Time: %ldns\n", duration.count());
        printf("C_max: %d\n", bruteForce_result.first.C_max);
        printf("Schedule:\n");
        print_Jobs(bruteForce_result.first.permutation);
        printf("------------------------------\n");
        printf("Brute Force RPQ:\n");
        printf("Time: %ldns\n", duration.count());
        printf("C_max: %d\n", bruteForce_result.second.C_max);
        printf("Schedule:\n");
        print_Jobs(bruteForce_result.second.permutation);
    }

    printf("------------------------------\n");

    printf("Carlier:\n");
    start = std::chrono::high_resolution_clock::now();
    Result carlier_result = carlier(jobs);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    printf("Time: %ldns\n", duration.count());
    printf("C_max: %d\n", carlier_result.C_max);
    printf("Schedule:\n");
    print_Jobs(carlier_result.permutation);

    printf("------------------------------\n");
}