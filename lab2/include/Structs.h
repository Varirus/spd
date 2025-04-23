#ifndef STRUCTS_H
#define STRUCTS_H
#include <vector>

struct Job
{
    int id;
    int p;
};

struct Machine
{
    int id;
    int maxTime;
    std::vector<Job> jobs;

    bool operator<(const Machine& other) const {
        if (maxTime != other.maxTime)
            return maxTime < other.maxTime;
        return id < other.id;
    }
};

struct Result {
    std::vector<Machine> perm;
    int C_max;
};

#endif