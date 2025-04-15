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

    //printf("------------------------------\n");
    //printf("Input:\n");
    //print_Jobs(jobs);
    long int sort_r_time = 0;
    long int sort_q_time = 0;
    long int schrage_time = 0;
    long int preemptiveSchrage_time = 0;
    long int bruteForce_time = 0;
    long int carlier_time = 0;
    long int ownAlgorithm_time = 0;
    long int ownAlgorithm2_time = 0;
    int sort_r_cmax = 0;
    int sort_q_cmax = 0;
    int schrage_cmax = 0;
    int preemptiveSchrage_cmax = 0;
    int bruteForce_cmax = 0;
    int carlier_cmax = 0;
    int ownAlgorithm_cmax = 0;
    int ownAlgorithm2_cmax = 0;
    int count = 10;
    for (size_t i = 0; i < count; i++)
    {
        //printf("------------------------------\n");

        //printf("Sort r:\n");
        auto start = std::chrono::high_resolution_clock::now();
        Result sort_r_result = sort_r(jobs);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        //printf("Time: %ldns\n", duration.count());
        sort_r_time += duration.count();
        sort_r_cmax = sort_r_result.C_max;
        //printf("C_max: %d\n", sort_r_result.C_max);
        //printf("Schedule:\n");
        //print_Jobs(sort_r_result.permutation);

        //printf("------------------------------\n");

        //printf("Sort q:\n");
        start = std::chrono::high_resolution_clock::now();
        Result sort_q_result = sort_q(jobs);
        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        //printf("Time: %ldns\n", duration.count());
        sort_q_time += duration.count();
        sort_q_cmax = sort_q_result.C_max;
        //printf("C_max: %d\n", sort_q_result.C_max);
        //printf("Schedule:\n");
        //print_Jobs(sort_q_result.permutation);

        //printf("------------------------------\n");

        //printf("Schrage:\n");
        start = std::chrono::high_resolution_clock::now();
        Result schrage_result = schrage(jobs);
        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        //printf("Time: %ldns\n", duration.count());
        schrage_time += duration.count();
        schrage_cmax = schrage_result.C_max;
        //printf("C_max: %d\n", schrage_result.C_max);
        //printf("Schedule:\n");
        //print_Jobs(schrage_result.permutation);

        //printf("------------------------------\n");

        //printf("Preemptive Schrage:\n");
        start = std::chrono::high_resolution_clock::now();
        Result preemptiveSchrage_result = preemptiveSchrage(jobs);
        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        //printf("Time: %ldns\n", duration.count());
        preemptiveSchrage_time += duration.count();
        preemptiveSchrage_cmax = preemptiveSchrage_result.C_max;
        //printf("C_max: %d\n", preemptiveSchrage_result.C_max);
        //printf("Schedule:\n");
        //print_Jobs(preemptiveSchrage_result.permutation);
        
        if (jobs.size() < 11)
        {
            //printf("------------------------------\n");
            //printf("Brute Force:\n");
            //printf("------------------------------\n");

            start = std::chrono::high_resolution_clock::now();
            Result bruteForce_result = bruteForce(jobs);
            end = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
            //printf("Brute Force RP:\n");
            //printf("Time: %ldns\n", duration.count());
            //printf("C_max: %d\n", bruteForce_result.first.C_max);
            //printf("Schedule:\n");
            //print_Jobs(bruteForce_result.first.permutation);
            //printf("------------------------------\n");
            //printf("Brute Force RPQ:\n");
            //printf("Time: %ldns\n", duration.count());
            bruteForce_time += duration.count();
            bruteForce_cmax = bruteForce_result.C_max;
            //printf("C_max: %d\n", bruteForce_result.second.C_max);
            //printf("Schedule:\n");
            //print_Jobs(bruteForce_result.second.permutation);
        }

        //printf("------------------------------\n");

        //printf("Carlier:\n");
        start = std::chrono::high_resolution_clock::now();
        Result carlier_result = solveCarlier(jobs);
        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        //printf("Time: %ldns\n", duration.count());
        carlier_time += duration.count();
        carlier_cmax = carlier_result.C_max;
        //printf("C_max: %d\n", carlier_result.C_max);
        //printf("Schedule:\n");
        //print_Jobs(carlier_result.permutation);

        //printf("------------------------------\n");

        //printf("Own Algorithm:\n");
        start = std::chrono::high_resolution_clock::now();
        Result ownAlgorithm_result = ownAlgorithm(jobs);
        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        //printf("Time: %ldns\n", duration.count());
        ownAlgorithm_time += duration.count();
        ownAlgorithm_cmax = ownAlgorithm_result.C_max;
        //printf("C_max: %d\n", ownAlgorithm_result.C_max);
        //printf("Schedule:\n");
        //print_Jobs(ownAlgorithm_result.permutation);

        //printf("------------------------------\n");

        //printf("Own Algorithm:\n");
        start = std::chrono::high_resolution_clock::now();
        Result ownAlgorithm2_result = ownAlgorithm2(jobs);
        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        //printf("Time: %ldns\n", duration.count());
        ownAlgorithm2_time += duration.count();
        ownAlgorithm2_cmax = ownAlgorithm2_result.C_max;
        //printf("C_max: %d\n", ownAlgorithm2_result.C_max);
        //printf("Schedule:\n");
        //print_Jobs(ownAlgorithm_result.permutation);

        //printf("------------------------------\n");
    }
    printf("------------------------------\n");
    printf("--Results %s --\n", argv[1]);
    printf("------------------------------\n");
    printf("Time Sort R: %.3lfms\n", (double)sort_r_time/(1000000*count));
    printf("C_max Sort R: %d\n", sort_r_cmax);
    printf("------------------------------\n");
    printf("Time Sort Q: %.3lfms\n", (double)sort_q_time/(1000000*count));
    printf("C_max Sort Q: %d\n", sort_q_cmax);
    printf("------------------------------\n");
    printf("Time Schrage: %.3lfms\n", (double)schrage_time/(1000000*count));
    printf("C_max Schrage: %d\n", schrage_cmax);
    printf("------------------------------\n");
    printf("Time Preemptive Schrage: %.3lfms\n", (double)preemptiveSchrage_time/(1000000*count));
    printf("C_max Preemptive Schrage: %d\n", preemptiveSchrage_cmax);
    printf("------------------------------\n");
    printf("Time Brute Force: %.3lfms\n", (double)bruteForce_time/(1000000*count));
    printf("C_max Brute Force: %d\n", bruteForce_cmax);
    printf("------------------------------\n");
    printf("Time Carlier: %.3lfms\n", (double)carlier_time/(1000000*count));
    printf("C_max Carlier: %d\n", carlier_cmax);
    printf("------------------------------\n");
    printf("Time Own Algorithm: %.3lfms\n", (double)ownAlgorithm_time/(1000000*count));
    printf("C_max Own Algorithm: %d\n", ownAlgorithm_cmax);
    printf("------------------------------\n");
    printf("Time Own Algorithm 2 - schrage po p+q: %.3lfms\n", (double)ownAlgorithm2_time/(1000000*count));
    printf("C_max Own Algorithm 2 - schrage po p+q:: %d\n", ownAlgorithm2_cmax);
    printf("------------------------------\n");
}