#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <vector>
#include "Job.h"

void bruteForce(std::vector<Job> jobs, std::vector<Job> &best_permutation_rp, 
               std::vector<Job> &best_permutation_rpq);
void schrage(std::vector<Job> jobs, std::vector<Job> &best_permutation, int &C_max);
void preemptiveSchrage(std::vector<Job> jobs, std::vector<Job> &best_permutation, int &C_max);

#endif // ALGORITHMS_H