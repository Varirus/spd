#ifndef COMPARATORS_H
#define COMPARATORS_H
#include "Structs.h"

struct CompareJobs
{
    bool operator()(const Job &a, const Job &b);
};

struct CompareMachines
{
    bool operator()(const Machine &a, const Machine &b);
};

#endif