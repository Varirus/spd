#include "Comparators.h"

bool CompareJobs::operator()(const Job &a, const Job &b)
{
    return a.p > b.p;
}

bool CompareMachines::operator()(const Machine &a, const Machine &b)
{
    return a.maxTime < b.maxTime;
}