#ifndef UTILITIES_H
#define UTILITIES_H

#include <vector>
#include "Structs.h"

std::vector<Job> readInput(int argc, char *argv[]);
std::vector<Job> sort_by_r(std::vector<Job> jobs);
std::vector<Job> sort_by_q(std::vector<Job> jobs);
int compute_makespan_rp(std::vector<Job> jobs);
int compute_makespan_rpq(std::vector<Job> jobs);
void print_Jobs(std::vector<Job> jobs);
int fact(int n);

#endif