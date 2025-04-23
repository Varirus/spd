#ifndef COMPARATORS_H
#define COMPARATORS_H

#include "Structs.h"

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

struct ComparePPlusQ {
    bool operator()(const Job& a, const Job& b);
};
#endif