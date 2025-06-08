#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "Utilities.h"
#include <vector>

ScheduleResult bruteForce(const std::vector<Job>& jobs, int m);
ScheduleResult NEH(const std::vector<Job>& jobs, int m);
ScheduleResult johnson(const std::vector<Job>& jobs);
ScheduleResult FNEH(const std::vector<Job>& jobs, int m);
int lowerBound(const std::vector<Job>& jobs, const std::vector<int>& currentPerm, const std::vector<bool>& used, int m);
ScheduleResult branchAndBound(const std::vector<Job>& jobs, int m);
ScheduleResult tabuSearch(const std::vector<Job>& jobs, int m, int maxIter, int tabuTenure);
ScheduleResult simulatedAnnealing(const std::vector<Job>& jobs, int m, int maxIter, double startTemp, double coolingRate);

#endif