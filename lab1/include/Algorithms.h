#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <vector>
#include "Structs.h"
#include <limits.h>

Result sort_r(std::vector<Job> jobs);
Result sort_q(std::vector<Job> jobs);
std::pair<Result, Result> bruteForce(std::vector<Job> jobs);
Result schrage(std::vector<Job> jobs);
Result preemptiveSchrage(std::vector<Job> jobs);
Result ownAlgorithm(std::vector<Job> jobs);
Result ownAlgorithm2(std::vector<Job> jobs);
void carlier(std::vector<Job>& jobs, Result& best_result);
Result solveCarlier(std::vector<Job> jobs);

#endif