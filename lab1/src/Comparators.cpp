#include "Comparators.h"
#include "Structs.h"

bool CompareQ::operator()(const Job &a, const Job &b)
{
    return a.q > b.q;
}

bool CompareR::operator()(const Job &a, const Job &b)
{
    return a.r < b.r;
}

bool CompareQ_Rev::operator()(const Job &a, const Job &b)
{
    return a.q < b.q;
}

bool CompareR_Rev::operator()(const Job &a, const Job &b)
{
    return a.r > b.r;
}

bool ComparePPlusQ::operator()(const Job& a, const Job& b) {
    return (a.p + a.q) > (b.p + b.q);
}

bool CompareMinusPPlusQ::operator()(const Job& a, const Job& b) {
    return (a.p + a.q) > (b.p + b.q);
}