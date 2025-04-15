#include <iostream>
#include <vector>
#include "Structs.h"
#include "Algorithms.h"
#include "Utilities.h"
#include <chrono>
#include <cstdlib>  
#include <ctime>    

using namespace std;

int main(int argc, char *argv[])
{
    std::srand(std::time(0)); 
    vector<Job> jobs = readInput(argc, argv);

    // printf("------------------------------\n");
    // printf("Input:\n");
    // print_Jobs(jobs);
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

    int sort_r_cmax1 = 0;
    int sort_q_cmax1 = 0;
    int schrage_cmax1 = 0;
    int preemptiveSchrage_cmax1 = 0;
    int bruteForce_cmax1 = 0;
    int carlier_cmax1 = 0;
    int ownAlgorithm_cmax1 = 0;
    int ownAlgorithm2_cmax1 = 0;

    int sort_r_error_max = 0;
    int sort_q_error_max = 0;
    int schrage_error_max = 0;
    int preemptiveSchrage_error_max = 0;
    int carlier_error_max = 0;
    int ownAlgorithm_error_max = 0;
    int ownAlgorithm2_error_max = 0;

    int sort_r_error = 0;
    int sort_q_error = 0;
    int schrage_error = 0;
    int preemptiveSchrage_error = 0;
    int carlier_error = 0;
    int ownAlgorithm_error = 0;
    int ownAlgorithm2_error = 0;

    double sort_r_error_sum = 0;
    double sort_q_error_sum = 0;
    double schrage_error_sum = 0;
    double preemptiveSchrage_error_sum = 0;
    double carlier_error_sum = 0;
    double ownAlgorithm_error_sum = 0;
    double ownAlgorithm2_error_sum = 0;

    int count = 50;

    for (size_t i = 0; i < count; i++)
    {

        int cmax_optimal = INT32_MAX;
    

    
        if (jobs.size() < 11)
        {
            // printf("------------------------------\n");
            // printf("Brute Force:\n");
            // printf("------------------------------\n");

            auto start = std::chrono::high_resolution_clock::now();
            Result bruteForce_result = bruteForce(jobs);
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
            // printf("Brute Force RP:\n");
            // printf("Time: %ldns\n", duration.count());
            // printf("C_max: %d\n", bruteForce_result.first.C_max);
            // printf("Schedule:\n");
            // print_Jobs(bruteForce_result.first.permutation);
            // printf("------------------------------\n");
            // printf("Brute Force RPQ:\n");
            // printf("Time: %ldns\n", duration.count());
            bruteForce_time += duration.count();
            bruteForce_cmax = bruteForce_result.C_max;

            // printf("C_max: %d\n", bruteForce_result.second.C_max);
            // printf("Schedule:\n");
            // print_Jobs(bruteForce_result.second.permutation);

            cmax_optimal = bruteForce_cmax;
        }

        // printf("------------------------------\n");

        // printf("Sort r:\n");
        auto start = std::chrono::high_resolution_clock::now();
        Result sort_r_result = sort_r(jobs);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        // printf("Time: %ldns\n", duration.count());
        sort_r_time += duration.count();
        sort_r_cmax = sort_r_result.C_max;
        // printf("C_max: %d\n", sort_r_result.C_max);
        // printf("Schedule:\n");
        // print_Jobs(sort_r_result.permutation);

        // printf("------------------------------\n");

        // printf("Sort q:\n");
        start = std::chrono::high_resolution_clock::now();
        Result sort_q_result = sort_q(jobs);
        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        // printf("Time: %ldns\n", duration.count());
        sort_q_time += duration.count();
        sort_q_cmax = sort_q_result.C_max;
        // printf("C_max: %d\n", sort_q_result.C_max);
        // printf("Schedule:\n");
        // print_Jobs(sort_q_result.permutation);

        // printf("------------------------------\n");

        // printf("Schrage:\n");
        start = std::chrono::high_resolution_clock::now();
        Result schrage_result = schrage(jobs);
        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        // printf("Time: %ldns\n", duration.count());
        schrage_time += duration.count();
        schrage_cmax = schrage_result.C_max;
        // printf("C_max: %d\n", schrage_result.C_max);
        // printf("Schedule:\n");
        // print_Jobs(schrage_result.permutation);

        // printf("------------------------------\n");

        // printf("Preemptive Schrage:\n");
        start = std::chrono::high_resolution_clock::now();
        Result preemptiveSchrage_result = preemptiveSchrage(jobs);
        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        // printf("Time: %ldns\n", duration.count());
        preemptiveSchrage_time += duration.count();
        preemptiveSchrage_cmax = preemptiveSchrage_result.C_max;
        // printf("C_max: %d\n", preemptiveSchrage_result.C_max);
        // printf("Schedule:\n");
        // print_Jobs(preemptiveSchrage_result.permutation);

        // printf("------------------------------\n");

        // printf("Carlier:\n");
        start = std::chrono::high_resolution_clock::now();
        Result carlier_result = solveCarlier(jobs);
        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        // printf("Time: %ldns\n", duration.count());
        carlier_time += duration.count();
        carlier_cmax = carlier_result.C_max;
        // printf("C_max: %d\n", carlier_result.C_max);
        // printf("Schedule:\n");
        // print_Jobs(carlier_result.permutation);

        // printf("------------------------------\n");

        // printf("Own Algorithm:\n");
        start = std::chrono::high_resolution_clock::now();
        Result ownAlgorithm_result = ownAlgorithm(jobs);
        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        // printf("Time: %ldns\n", duration.count());
        ownAlgorithm_time += duration.count();
        ownAlgorithm_cmax = ownAlgorithm_result.C_max;
        // printf("C_max: %d\n", ownAlgorithm_result.C_max);
        // printf("Schedule:\n");
        // print_Jobs(ownAlgorithm_result.permutation);

        // printf("------------------------------\n");

        // printf("Own Algorithm:\n");
        start = std::chrono::high_resolution_clock::now();
        Result ownAlgorithm2_result = ownAlgorithm2(jobs);
        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        // printf("Time: %ldns\n", duration.count());
        ownAlgorithm2_time += duration.count();
        ownAlgorithm2_cmax = ownAlgorithm2_result.C_max;
        // printf("C_max: %d\n", ownAlgorithm2_result.C_max);
        // printf("Schedule:\n");
        // print_Jobs(ownAlgorithm_result.permutation);

        // printf("------------------------------\n");
        if (jobs.size() >= 11)
        {
            if (sort_r_cmax < cmax_optimal)
                cmax_optimal = sort_r_cmax;
            if (sort_q_cmax < cmax_optimal)
                cmax_optimal = sort_q_cmax;
            if (schrage_cmax < cmax_optimal)
                cmax_optimal = schrage_cmax;
            if (preemptiveSchrage_cmax < cmax_optimal)
                cmax_optimal = preemptiveSchrage_cmax;
            if (carlier_cmax < cmax_optimal)
                cmax_optimal = carlier_cmax;
            if (ownAlgorithm_cmax < cmax_optimal)
                cmax_optimal = ownAlgorithm_cmax;
            if (ownAlgorithm2_cmax < cmax_optimal)
                cmax_optimal = ownAlgorithm2_cmax;
        }
        sort_r_error = (sort_r_cmax - cmax_optimal) * 100 / cmax_optimal;
        if (sort_r_error > sort_r_error_max)
            sort_r_error_max = sort_r_error;
        sort_r_error_sum += sort_r_error;

        sort_q_error = (sort_q_cmax - cmax_optimal) * 100 / cmax_optimal;
        if (sort_q_error > sort_q_error_max)
            sort_q_error_max = sort_q_error;
        sort_q_error_sum += sort_q_error;

        schrage_error = (schrage_cmax - cmax_optimal) * 100 / cmax_optimal;
        if (schrage_error > schrage_error_max)
            schrage_error_max = schrage_error;
        schrage_error_sum += schrage_error;

        preemptiveSchrage_error = (preemptiveSchrage_cmax - cmax_optimal) * 100 / cmax_optimal;
        if (preemptiveSchrage_error > preemptiveSchrage_error_max)
            preemptiveSchrage_error_max = preemptiveSchrage_error;
        preemptiveSchrage_error_sum += preemptiveSchrage_error;

        carlier_error = (carlier_cmax - cmax_optimal) * 100 / cmax_optimal;
        if (carlier_error > carlier_error_max)
            carlier_error_max = carlier_error;
        carlier_error_sum += carlier_error;

        ownAlgorithm_error = (ownAlgorithm_cmax - cmax_optimal) * 100 / cmax_optimal;
        if (ownAlgorithm_error > ownAlgorithm_error_max)
            ownAlgorithm_error_max = ownAlgorithm_error;
        ownAlgorithm_error_sum += ownAlgorithm_error;

        ownAlgorithm2_error = (ownAlgorithm2_cmax - cmax_optimal) * 100 / cmax_optimal;
        if (ownAlgorithm2_error > ownAlgorithm2_error_max)
            ownAlgorithm2_error_max = ownAlgorithm2_error;
        ownAlgorithm2_error_sum += ownAlgorithm2_error;
        if(i==0){
            sort_r_cmax1 = sort_r_cmax;
            sort_q_cmax1 = sort_q_cmax;
            schrage_cmax1 = schrage_cmax;
            preemptiveSchrage_cmax1 = preemptiveSchrage_cmax;
            bruteForce_cmax1 = bruteForce_cmax;
            carlier_cmax1 = carlier_cmax;
            ownAlgorithm_cmax1 = ownAlgorithm_cmax;
            ownAlgorithm2_cmax1 = ownAlgorithm2_cmax;
        }
        printf("Error sredni Own Algorithm 2: %lf\n", ownAlgorithm2_error_sum);
        for(auto& job : jobs){
            job.r = rand() % 1000;
            job.p = rand() % 100;
            job.q = rand() % 400;
        }
    }
    printf("------------------------------\n");
    printf("--Results %s --\n", argv[1]);
    printf("------------------------------\n");
    printf("Time Sort R: %.3lfms\n", (double)sort_r_time / (1000000 * count));
    printf("C_max Sort R: %d\n", sort_r_cmax1);
    printf("Error sredni Sort R: %lf\n", (double)sort_r_error_sum / count);
    printf("Error max Sort R: %d\n", sort_r_error_max);
    printf("------------------------------\n");

    printf("Time Sort Q: %.3lfms\n", (double)sort_q_time / (1000000 * count));
    printf("C_max Sort Q: %d\n", sort_q_cmax1);
    printf("Error sredni Sort Q: %lf\n", (double)sort_q_error_sum / count);
    printf("Error max Sort Q: %d\n", sort_q_error_max);
    printf("------------------------------\n");

    printf("Time Schrage: %.3lfms\n", (double)schrage_time / (1000000 * count));
    printf("C_max Schrage: %d\n", schrage_cmax1);
    printf("Error sredni Schrage: %lf\n", (double)schrage_error_sum / count);
    printf("Error max Schrage: %d\n", schrage_error_max);
    printf("------------------------------\n");

    printf("Time Preemptive Schrage: %.3lfms\n", (double)preemptiveSchrage_time / (1000000 * count));
    printf("C_max Preemptive Schrage: %d\n", preemptiveSchrage_cmax1);
    printf("Error sredni Preemptive Schrage: %lf\n", (double)preemptiveSchrage_error_sum / count);
    printf("Error max Preemptive Schrage: %d\n", preemptiveSchrage_error_max);
    printf("------------------------------\n");

    printf("Time Brute Force: %.3lfms\n", (double)bruteForce_time / (1000000 * count));
    printf("C_max Brute Force: %d\n", bruteForce_cmax1);
    printf("------------------------------\n");

    printf("Time Carlier: %.3lfms\n", (double)carlier_time / (1000000 * count));
    printf("C_max Carlier: %d\n", carlier_cmax1);
    printf("Error sredni Carlier: %lf\n", carlier_error_sum / count);
    printf("Error max Carlier: %d\n", carlier_error_max);
    printf("------------------------------\n");

    printf("Time Own Algorithm: %.3lfms\n", (double)ownAlgorithm_time / (1000000 * count));
    printf("C_max Own Algorithm: %d\n", ownAlgorithm_cmax1);
    printf("Error sredni Own Algorithm: %lf\n", ownAlgorithm_error_sum / count);
    printf("Error max Own Algorithm: %d\n", ownAlgorithm_error_max);
    printf("------------------------------\n");

    printf("Time Own Algorithm 2 - schrage po p+q: %.3lfms\n", (double)ownAlgorithm2_time / (1000000 * count));
    printf("C_max Own Algorithm 2 - schrage po p+q: %d\n", ownAlgorithm2_cmax1);
    printf("Error sredni Own Algorithm 2: %lf\n", ownAlgorithm2_error_sum / count);
    printf("Error max Own Algorithm 2: %d\n", ownAlgorithm2_error_max);
    printf("------------------------------\n");
}