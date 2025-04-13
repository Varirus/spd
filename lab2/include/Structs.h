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
    int timeLeft;
    std::vector<Job> jobs;

    bool operator<(const Machine& other) const {
        if (timeLeft != other.timeLeft)
            return timeLeft < other.timeLeft;
        return id < other.id;
    }
};

struct Result {
    std::vector<Machine> perm;
    int C_max;
};

#endif