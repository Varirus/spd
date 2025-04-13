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
    //print_Jobs(jobs);
    long int schrage_time = 0;
    long int preemptiveSchrage_time = 0;
    long int bruteForce_time = 0;
    long int carlier_time = 0;
    long int ownAlgorithm_time = 0;
    int count = 10;
    for (size_t i = 0; i < count; i++)
    {
        printf("------------------------------\n");

        printf("Schrage:\n");
        auto start = std::chrono::high_resolution_clock::now();
        Result schrage_result = schrage(jobs);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        printf("Time: %ldns\n", duration.count());
        schrage_time += duration.count();
        printf("C_max: %d\n", schrage_result.C_max);
        printf("Schedule:\n");
        //print_Jobs(schrage_result.permutation);

        printf("------------------------------\n");

        printf("Preemptive Schrage:\n");
        start = std::chrono::high_resolution_clock::now();
        Result preemptiveSchrage_result = preemptiveSchrage(jobs);
        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        printf("Time: %ldns\n", duration.count());
        preemptiveSchrage_time += duration.count();
        printf("C_max: %d\n", preemptiveSchrage_result.C_max);
        printf("Schedule:\n");
        //print_Jobs(preemptiveSchrage_result.permutation);
        
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
            //print_Jobs(bruteForce_result.first.permutation);
            printf("------------------------------\n");
            printf("Brute Force RPQ:\n");
            printf("Time: %ldns\n", duration.count());
            bruteForce_time += duration.count();
            printf("C_max: %d\n", bruteForce_result.second.C_max);
            printf("Schedule:\n");
            //print_Jobs(bruteForce_result.second.permutation);
        }

        printf("------------------------------\n");

        printf("Carlier:\n");
        start = std::chrono::high_resolution_clock::now();
        Result carlier_result = solveCarlier(jobs);
        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        printf("Time: %ldns\n", duration.count());
        carlier_time += duration.count();
        printf("C_max: %d\n", carlier_result.C_max);
        printf("Schedule:\n");
        //print_Jobs(carlier_result.permutation);

        printf("------------------------------\n");

        printf("Own Algorithm:\n");
        start = std::chrono::high_resolution_clock::now();
        Result ownAlgorithm_result = ownAlgorithm2(jobs);
        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        printf("Time: %ldns\n", duration.count());
        ownAlgorithm_time += duration.count();
        printf("C_max: %d\n", ownAlgorithm_result.C_max);
        printf("Schedule:\n");
        //print_Jobs(ownAlgorithm_result.permutation);

        printf("------------------------------\n");
    }
    printf("Time Schrage: %.3lfms\n", (double)schrage_time/1000*count);
    printf("Time Preemptive Schrage: %.3lfms\n", (double)preemptiveSchrage_time/1000*count);
    printf("Time Brute Force: %.3lfms\n", (double)bruteForce_time/1000*count);
    printf("Time Carlier: %.3lfms\n", (double)carlier_time/1000*count);
    printf("Time Own Algorithm: %.3lfms\n", (double)ownAlgorithm_time/1000*count);
    /*printf("------------------------------\n");

    printf("Schrage:\n");
    auto start = std::chrono::high_resolution_clock::now();
    Result schrage_result = schrage(jobs);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    printf("Time: %ldns\n", duration.count());
    printf("C_max: %d\n", schrage_result.C_max);
    printf("Schedule:\n");
    //print_Jobs(schrage_result.permutation);

    printf("------------------------------\n");

    printf("Preemptive Schrage:\n");
    start = std::chrono::high_resolution_clock::now();
    Result preemptiveSchrage_result = preemptiveSchrage(jobs);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    printf("Time: %ldns\n", duration.count());
    printf("C_max: %d\n", preemptiveSchrage_result.C_max);
    printf("Schedule:\n");
    //print_Jobs(preemptiveSchrage_result.permutation);
    
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
        //print_Jobs(bruteForce_result.first.permutation);
        printf("------------------------------\n");
        printf("Brute Force RPQ:\n");
        printf("Time: %ldns\n", duration.count());
        printf("C_max: %d\n", bruteForce_result.second.C_max);
        printf("Schedule:\n");
        //print_Jobs(bruteForce_result.second.permutation);
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
    //print_Jobs(carlier_result.permutation);

    printf("------------------------------\n");

    printf("Own Algorithm:\n");
    start = std::chrono::high_resolution_clock::now();
    Result ownAlgorithm_result = ownAlgorithm2(jobs);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    printf("Time: %ldns\n", duration.count());
    printf("C_max: %d\n", ownAlgorithm_result.C_max);
    printf("Schedule:\n");
    //print_Jobs(ownAlgorithm_result.permutation);

    printf("------------------------------\n");*/
}