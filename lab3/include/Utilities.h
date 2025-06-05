#ifndef UTILITIES_H
#define UTILITIES_H

#include <vector>
#include "Structs.h"
#include <string>

bool loadInstance(const std::string& filename, int instanceIndex, int& n, int& m, std::vector<Job>& jobs);
void printJobs(const std::vector<Job>& jobs, int m);
int calculateCmax(const std::vector<Job>& jobs, const std::vector<int>& permutation, int m);
int calculateCsum(const std::vector<Job>& jobs, const std::vector<int>& permutation, int m);

#endif