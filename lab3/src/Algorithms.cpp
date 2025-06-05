#include <vector>
#include <numeric>
#include <algorithm>
#include <queue>
#include <iostream>
#include <climits>
#include <random>
#include <functional>
#include "Structs.h"
#include "Utilities.h"

ScheduleResult bruteForce(const std::vector<Job>& jobs, int m) {
    int n = jobs.size();
    std::vector<int> perm(n);
    std::iota(perm.begin(), perm.end(), 0);

    int bestCmax = INT_MAX;
    std::vector<int> bestPerm;

    do {
        int currentCmax = calculateCmax(jobs, perm, m);
        if (currentCmax < bestCmax) {
            bestCmax = currentCmax;
            bestPerm = perm;
        }
    } while (std::next_permutation(perm.begin(), perm.end()));

    return ScheduleResult{perm, bestCmax};
}

ScheduleResult NEH(const std::vector<Job>& jobs, int m) {
    // 1. LICZENIE SUMY CZASU WYKONANIA ZADANIA 
    int n = jobs.size();
    std::vector<std::pair<int, int>> jobSum(n);
    for (int i = 0; i < n; ++i) {
        int sum = std::accumulate(jobs[i].times.begin(), jobs[i].times.end(), 0);
        jobSum[i] = {sum, i};
    }
    // 2. SORTOWANIE ZADAŃ MALEJĄCO
    std::sort(jobSum.rbegin(), jobSum.rend());

    std::vector<int> partialSeq;
    partialSeq.push_back(jobSum[0].second);
    // 3. WSTAWIANIE ZADAŃ DLA MINIMALNEGO C_MAX
    for (int k = 1; k < n; ++k) {
        int jobId = jobSum[k].second;
        int bestCmax = -1;
        int bestPos = 0;

        for (int pos = 0; pos <= (int)partialSeq.size(); ++pos) {
            std::vector<int> trialSeq = partialSeq;
            trialSeq.insert(trialSeq.begin() + pos, jobId);
            int currentCmax = calculateCmax(jobs, trialSeq, m);
            if (bestCmax == -1 || currentCmax < bestCmax) {
                bestCmax = currentCmax;
                bestPos = pos;
            }
        }
        partialSeq.insert(partialSeq.begin() + bestPos, jobId);
    }
    int finalCmax = calculateCmax(jobs, partialSeq, m);
    return ScheduleResult{partialSeq, finalCmax};
}

ScheduleResult johnson(const std::vector<Job>& jobs) {
    int n = jobs.size();
    std::vector<int> left, right;

    for (int i = 0; i < n; ++i) {
        int t1 = jobs[i].times[0];
        int t2 = jobs[i].times[1];
        if (t1 <= t2)
            left.push_back(i);
        else
            right.push_back(i);
    }

    std::sort(left.begin(), left.end(), [&](int a, int b) {
        return jobs[a].times[0] < jobs[b].times[0];
    });

    std::sort(right.begin(), right.end(), [&](int a, int b) {
        return jobs[a].times[1] > jobs[b].times[1];
    });

    std::vector<int> perm;
    perm.insert(perm.end(), left.begin(), left.end());
    perm.insert(perm.end(), right.begin(), right.end());

    std::vector<Job> reducedJobs(n);
    for (int i = 0; i < n; ++i)
        reducedJobs[i].times = {jobs[i].times[0], jobs[i].times[1]};

    int cmax = calculateCmax(reducedJobs, perm, 2);
    return ScheduleResult{perm, cmax};
}

ScheduleResult FNEH(const std::vector<Job>& jobs, int m) {
    int n = jobs.size();
    std::vector<std::pair<int, int>> jobSum(n);
    for (int i = 0; i < n; ++i)
        jobSum[i] = { std::accumulate(jobs[i].times.begin(), jobs[i].times.end(), 0), i };
    
    std::sort(jobSum.rbegin(), jobSum.rend());

    std::vector<int> sequence;
    sequence.push_back(jobSum[0].second);

    std::vector<int> completionTimes(m, 0);

    for (int k = 1; k < n; ++k) {
        int jobId = jobSum[k].second;
        int bestCmax = -1;
        int bestPos = 0;

        for (int pos = 0; pos <= (int)sequence.size(); ++pos) {
            std::vector<int> tempSeq = sequence;
            tempSeq.insert(tempSeq.begin() + pos, jobId);

            std::vector<int> cTimes(m, 0);
            for (int idx = 0; idx < (int)tempSeq.size(); ++idx) {
                int jId = tempSeq[idx];
                cTimes[0] += jobs[jId].times[0];
                for (int machine = 1; machine < m; ++machine) {
                    cTimes[machine] = std::max(cTimes[machine], cTimes[machine - 1]) + jobs[jId].times[machine];
                }
            }
            int currentCmax = cTimes[m - 1];

            if (bestCmax == -1 || currentCmax < bestCmax) {
                bestCmax = currentCmax;
                bestPos = pos;
            }
        }
        sequence.insert(sequence.begin() + bestPos, jobId);
    }

    int finalCmax = calculateCmax(jobs, sequence, m);
    return ScheduleResult{sequence, finalCmax};
}


ScheduleResult branchAndBound(const std::vector<Job>& jobs, int m) {
    int n = jobs.size();
    std::vector<int> bestPerm;
    int bestCmax = INT_MAX;

    std::vector<int> currentPerm;
    std::vector<bool> used(n, false);

    std::vector<std::vector<int>> completionTimes(m, std::vector<int>(n, 0));

    std::function<void(int)> bnb = [&](int depth) {
        if (depth == n) {
            int cmax = calculateCmax(jobs, currentPerm, m);
            if (cmax < bestCmax) {
                bestCmax = cmax;
                bestPerm = currentPerm;
            }
            return;
        }

        for (int i = 0; i < n; ++i) {
            if (!used[i]) {
                used[i] = true;
                currentPerm.push_back(i);

                // Przycinanie: obliczamy C_max dla częściowej permutacji
                int partialCmax = calculateCmax(jobs, currentPerm, m);
                if (partialCmax < bestCmax) {
                    bnb(depth + 1);
                }

                currentPerm.pop_back();
                used[i] = false;
            }
        }
    };

    bnb(0);
    return ScheduleResult{bestPerm, bestCmax};
}

ScheduleResult tabuSearch(const std::vector<Job>& jobs, int m, int maxIter, int tabuTenure) {
    int n = jobs.size();
    std::vector<int> currentPerm(n);
    std::iota(currentPerm.begin(), currentPerm.end(), 0);
    
    int bestCmax = calculateCmax(jobs, currentPerm, m);
    std::vector<int> bestPerm = currentPerm;

    std::vector<std::vector<int>> tabuList(n, std::vector<int>(n, 0));

    int iter = 0;
    std::mt19937 rng(std::random_device{}());

    while (iter < maxIter) {
        int bestNeighborCmax = std::numeric_limits<int>::max();
        int swap_i = -1, swap_j = -1;

        for (int i = 0; i < n - 1; ++i) {
            for (int j = i + 1; j < n; ++j) {
                if (tabuList[i][j] > iter) continue;

                std::swap(currentPerm[i], currentPerm[j]);
                int currentCmax = calculateCmax(jobs, currentPerm, m);
                if (currentCmax < bestNeighborCmax) {
                    bestNeighborCmax = currentCmax;
                    swap_i = i;
                    swap_j = j;
                }
                std::swap(currentPerm[i], currentPerm[j]);
            }
        }

        if (swap_i == -1) break;

        std::swap(currentPerm[swap_i], currentPerm[swap_j]);

        if (bestNeighborCmax < bestCmax) {
            bestCmax = bestNeighborCmax;
            bestPerm = currentPerm;
        }

        tabuList[swap_i][swap_j] = iter + tabuTenure;
        tabuList[swap_j][swap_i] = iter + tabuTenure;

        iter++;
    }

    return {bestPerm, bestCmax};
}

ScheduleResult simulatedAnnealing(const std::vector<Job>& jobs, int m, int maxIter, double startTemp, double coolingRate) {
    int n = jobs.size();
    std::vector<int> currentPerm(n);
    std::iota(currentPerm.begin(), currentPerm.end(), 0);

    int currentCmax = calculateCmax(jobs, currentPerm, m);
    std::vector<int> bestPerm = currentPerm;
    int bestCmax = currentCmax;

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> distIndex(0, n - 1);
    std::uniform_real_distribution<double> distProb(0.0, 1.0);

    double temp = startTemp;

    for (int iter = 0; iter < maxIter; ++iter) {
        int i = distIndex(rng);
        int j = distIndex(rng);
        if (i == j) continue;

        std::swap(currentPerm[i], currentPerm[j]);
        int newCmax = calculateCmax(jobs, currentPerm, m);

        if (newCmax < currentCmax || distProb(rng) < std::exp((currentCmax - newCmax) / temp)) {
            currentCmax = newCmax;
            if (newCmax < bestCmax) {
                bestCmax = newCmax;
                bestPerm = currentPerm;
            }
        } else {
            std::swap(currentPerm[i], currentPerm[j]);
        }

        temp *= coolingRate;
        if (temp < 1e-8) temp = 1e-8;
    }

    return {bestPerm, bestCmax};
}