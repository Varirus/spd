#include <iostream>
#include <vector>
#include "Job.h"
#include "Algorithms.h"
#include "Utilities.h"

using namespace std;

int main(int argc, char *argv[])
{
    vector<Job> jobs = readInput(argc, argv);
    vector<Job> best_permutation_rp;
    vector<Job> best_permutation_rpq;
    vector<Job> schrage_permutation;
    vector<Job> preemptiveSchrage_permutation;

    printf("------------------------------\n");
    printf("Input:\n");
    print_Jobs(jobs);

    /*printf("Sortowane po r\n");
    vector<Job> sort_r = sort_by_r(jobs);
    print_Jobs(sort_r);

    printf("Sortowane po q\n");
    vector<Job> sort_q = sort_by_q(jobs);
    print_Jobs(sort_q);*/
    printf("------------------------------\n");
    printf("Schrage:\n");
    int C_max_schrage;
    schrage(jobs, schrage_permutation, C_max_schrage);

    printf("------------------------------\n");
    printf("Preemptive Schrage:\n");
    int C_max_preemptiveSchrage;
    preemptiveSchrage(jobs, preemptiveSchrage_permutation, C_max_preemptiveSchrage);

    if (jobs.size() < 11)
    {
        printf("------------------------------\n");
        printf("Brute Force:\n");
        bruteForce(jobs, best_permutation_rp, best_permutation_rpq);
    }
    printf("------------------------------\n");
}