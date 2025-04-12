#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <vector>
#include "Structs.h"
#include <limits.h>

std::pair<Result, Result> bruteForce(std::vector<Job> jobs);
Result schrage(std::vector<Job> jobs);
Result preemptiveSchrage(std::vector<Job> jobs);
Result carlier(std::vector<Job> jobs, int UB = INT_MAX);

#endif