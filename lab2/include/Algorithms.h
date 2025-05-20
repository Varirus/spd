#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <vector>
#include "Structs.h"
#include <limits.h>

Result lsa(int m, std::vector<Job> original_jobs);
Result lpt(int m, std::vector<Job> original_jobs);
Result solveP2Cmax_PD(std::vector<Job> original_jobs);
Result solveP3Cmax_PD(std::vector<Job> original_jobs);
Result solveP2Cmax_PZ(std::vector<Job> original_jobs);
Result solveP3Cmax_PZ(std::vector<Job> original_jobs);
Result solveP2Cmax_PTAS(std::vector<Job> original_jobs, int k);
Result solveP2Cmax_FPTAS(std::vector<Job> original_jobs, int k);
Result solveP3Cmax_PTAS(std::vector<Job> original_jobs, int k);

#include <functional>

struct pair_hash {
    std::size_t operator()(const std::pair<int, int> &p) const noexcept {
        std::size_t h1 = std::hash<int>{}(p.first);
        std::size_t h2 = std::hash<int>{}(p.second);
        return h1 ^ (h2 << 1); // or use boost::hash_combine-like trick
    }
};
#endif