#ifndef STRUCTS_H
#define STRUCTS_H
#include <vector>

struct Job
{
    int id;
    int r;
    int p;
    int q;
};

struct Result {
    std::vector<Job> permutation;
    int C_max;
};

#endif