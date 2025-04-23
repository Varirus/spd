#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <vector>
#include "Structs.h"
#include <limits.h>

Result lsa(int m, std::vector<Job> original_jobs);
Result lpt(int m, std::vector<Job> original_jobs);
Result solveP2Cmax(std::vector<Job> original_jobs);
void branchAndBound(int jobIndex, const std::vector<Job> &jobs, int sumM1, int sumM2,
    std::vector<Job> &jobsM1, std::vector<Job> &jobsM2, int &bestCmax,
    std::vector<Job> &bestJobsM1, std::vector<Job> &bestJobsM2);
Result solveP2Cmax_BnB(std::vector<Job> original_jobs);
#endif