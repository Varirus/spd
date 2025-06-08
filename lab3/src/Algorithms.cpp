#include <vector>
#include <numeric>
#include <algorithm>
#include <queue>
#include <iostream>
#include <climits>
#include <random>
#include <functional>
#include <cstdlib>
#include <ctime>
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
    // 1. OBLICZENIE SUM CZASU WYKONANIA ZADANIA 
    int n = jobs.size();
    std::vector<std::pair<int, int>> jobSum(n);
    for (int i = 0; i < n; ++i) {
        int sum = std::accumulate(jobs[i].times.begin(), jobs[i].times.end(), 0);
        jobSum[i] = {sum, i};
    }
    // 2. SORTOWANIE ZADAŃ MALEJĄCO
    std::sort(jobSum.rbegin(), jobSum.rend());
    // 3. DODANIE PIERWSZEGO ZADANIA
    std::vector<int> partialSeq;
    partialSeq.push_back(jobSum[0].second);
    // 4. WSTAWIANIE ZADAŃ DLA MINIMALNEGO C_MAX
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

    // 1. PRZYDZIAL ZADAN DO ODPOWIEDNICH PODGRUP
    for (int i = 0; i < n; ++i) {
        int t1 = jobs[i].times[0];
        int t2 = jobs[i].times[1];
        if (t1 <= t2)
            left.push_back(i);
        else
            right.push_back(i);
    }
    // 2. SORTOWANIE 1. GRUPY ROSNĄCO
    std::sort(left.begin(), left.end(), [&](int a, int b) {
        return jobs[a].times[0] < jobs[b].times[0];
    });
    // 3. SORTOWANIE 2. GRUPY MALEJĄCO
    std::sort(right.begin(), right.end(), [&](int a, int b) {
        return jobs[a].times[1] > jobs[b].times[1];
    });
    // 4. STWORZENIE PERMUTACJI
    std::vector<int> perm;
    perm.insert(perm.end(), left.begin(), left.end());
    perm.insert(perm.end(), right.begin(), right.end());
    // 5. STWORZENIE WEKTORA JOB W PERMUTACJI
    std::vector<Job> reducedJobs(n);
    for (int i = 0; i < n; ++i)
        reducedJobs[i].times = {jobs[i].times[0], jobs[i].times[1]};

    int cmax = calculateCmax(reducedJobs, perm, 2);
    return ScheduleResult{perm, cmax};
}

ScheduleResult FNEH(const std::vector<Job>& jobs, int m) {
    // 1. OBLICZENIE SUM CZASU WYKONANIA ZADANIA 
    int n = jobs.size();
    std::vector<std::pair<int, int>> jobSum(n);
    for (int i = 0; i < n; ++i) {
        int sum = std::accumulate(jobs[i].times.begin(), jobs[i].times.end(), 0);
        jobSum[i] = {sum, i};
    }
    // 2. SORTOWANIE ZADAŃ MALEJĄCO
    std::sort(jobSum.rbegin(), jobSum.rend());
    // 3. DODANIE PIERWSZEGO ZADANIA
    std::vector<int> sequence;
    sequence.push_back(jobSum[0].second);
    // 4. WSTAWIANIE ZADAŃ DLA MINIMALNEGO C_MAX
    std::vector<int> completionTimes(m, 0);

    for (int k = 1; k < n; ++k) {
        int jobId = jobSum[k].second;
        int bestCmax = -1;
        int bestPos = 0;

        for (int pos = 0; pos <= (int)sequence.size(); ++pos) {
            std::vector<int> tempSeq = sequence;
            tempSeq.insert(tempSeq.begin() + pos, jobId);
            // RÓZNICA OD NEH, BRAK OLBICZENIA PEŁNEGO C_CMAX
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

int lowerBound(const std::vector<Job>& jobs, const std::vector<int>& currentPerm, const std::vector<bool>& used, int m) {
    int n = jobs.size();

    // 1. OBLICZ PARTIALCMAX
    int partialCmax = 0;
    if (!currentPerm.empty()) {
        partialCmax = calculateCmax(jobs, currentPerm, m);
    }

    // 2. SUMA MIN CZASÓW POZOSTAŁYCH ZADAŃ NA MASZYNIE
    //    PIERWSZEJ I OSTATNIEJ
    int sumMinFirst = 0, sumMinLast = 0;
    for (int j = 0; j < n; ++j) {
        if (!used[j]) {
            sumMinFirst += jobs[j].times[0];
            sumMinLast  += jobs[j].times[m-1];
        }
    }

    // 3. LB TO MAX Z TYCH 3 OBLICZONYCH
    return std::max({partialCmax, sumMinFirst, sumMinLast});
}


ScheduleResult branchAndBound(const std::vector<Job>& jobs, int m) {
    int n = jobs.size();
    std::vector<int> bestPerm;
    int bestCmax = INT_MAX;
    std::vector<int> currentPerm;
    std::vector<bool> used(n, false);
    // REKURENCYJNA METODA DRZEWA
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

                // ODICINANIE
                int partialLB = lowerBound(jobs, currentPerm, used, m);
                if (partialLB < bestCmax) {
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

    while (iter < maxIter) {
        int bestNeighborCmax = __INT_MAX__;
        int swap_i = -1, swap_j = -1;

        for (int i = 0; i < n - 1; ++i) {
            for (int j = i + 1; j < n; ++j) {
                // JEŻELI TABU LIST NIE ZABRANIA TO WYKONUJEMY RUCH
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
        // JEŚLI ISTNIEJE ZAMIANA TO ZMIENIAMI I ZABRANIAMY RUCHU NA KOLEJNE ITERACJE
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

    std::srand(std::time(nullptr)); 

    double temp = startTemp;

    for (int iter = 0; iter < maxIter; ++iter) {
        int i = std::rand() % n;
        int j = std::rand() % n;
        if (i == j) continue;

        std::swap(currentPerm[i], currentPerm[j]);
        int newCmax = calculateCmax(jobs, currentPerm, m);

        double prob = static_cast<double>(std::rand()) / RAND_MAX;
        // WYOKNUJEMY RUCH JEŚLI DAJE LEPSZY REZULTAT LUB Z PEWNYM PRAWDOPODOBIEŃSTWWEM
        if (newCmax < currentCmax || prob < std::exp((currentCmax - newCmax) / temp)) {
            currentCmax = newCmax;
            if (newCmax < bestCmax) {
                bestCmax = newCmax;
                bestPerm = currentPerm;
            }
        } else {
            std::swap(currentPerm[i], currentPerm[j]);
        }
        // WYCHŁADZANIE
        temp *= coolingRate;
    }

    return {bestPerm, bestCmax};
}