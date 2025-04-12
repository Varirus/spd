#ifndef COMPARATORS_H
#define COMPARATORS_H

#include "Job.h"

struct CompareQ
{
    bool operator()(const Job &a, const Job &b);
};

struct CompareR
{
    bool operator()(const Job &a, const Job &b);
};

struct CompareQ_Rev
{
    bool operator()(const Job &a, const Job &b);
};

struct CompareR_Rev
{
    bool operator()(const Job &a, const Job &b);
};

#endif // COMPARATORS_H