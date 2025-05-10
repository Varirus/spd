#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <vector>
#include "Structs.h"
#include <limits.h>

Result lsa(int m, std::vector<Job> original_jobs);
Result lpt(int m, std::vector<Job> original_jobs);
Result solveP2Cmax_PD(std::vector<Job> original_jobs);
Result solveP3Cmax_PD(std::vector<Job> original_jobs);
Result solveP2Cmax_PZ(std::vector<Job> original_jobs);
Result solveP3Cmax_PZ(std::vector<Job> original_jobs);
Result solveP2Cmax_PTAS(std::vector<Job> original_jobs, double epsilon);
#endif