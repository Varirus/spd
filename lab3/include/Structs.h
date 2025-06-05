#ifndef STRUCTS_H
#define STRUCTS_H

#include <vector>

struct Job {
    int id;
    std::vector<int> times;
};

struct ScheduleResult {
    std::vector<int> permutation;
    int C_max;
};

#endif