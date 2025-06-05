#include "Utilities.h"
#include <fstream>
#include <sstream>
#include <iostream>

bool loadInstance(const std::string& filename, int instanceIndex, int& n, int& m, std::vector<Job>& jobs) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    int totalInstances;
    file >> totalInstances;

    if (instanceIndex >= totalInstances) return false;

    for (int i = 0; i <= instanceIndex; ++i) {
        file >> n >> m;
        if (i < instanceIndex) {
            int skipJob, skipMachine, skipTime;
            for (int j = 0; j < n * m; ++j) {
                file >> skipMachine >> skipTime;
            }
        }
    }

    jobs.clear();
    jobs.resize(n);
    for (int i = 0; i < n; ++i) {
        Job job;
        job.id = i;
        job.times.resize(m);
        for (int j = 0; j < m; ++j) {
            int machineId, time;
            file >> machineId >> time;
            job.times[machineId] = time;
        }
        jobs[i] = job;
    }

    return true;
}

void printJobs(const std::vector<Job>& jobs, int m) {
    std::cout << jobs.size() << " " << m << "\n";
    for (const auto& job : jobs) {
        std::cout << job.id << ":";
        for (int t : job.times) {
            std::cout << " " << t;
        }
        std::cout << "\n";
    }
}

int calculateCmax(const std::vector<Job>& jobs, const std::vector<int>& permutation, int m) {
    int n = permutation.size();
    std::vector<std::vector<int>> C(n, std::vector<int>(m, 0));

    for (int i = 0; i < n; ++i) {
        int jobId = permutation[i];
        for (int machine = 0; machine < m; ++machine) {
            int prevJobCompletion = 0;
            if (i != 0) {
                prevJobCompletion = C[i - 1][machine];
            }
            
            int prevMachineCompletion = 0;
            if (machine != 0) {
                prevMachineCompletion = C[i][machine - 1];
            }
            
            C[i][machine] = std::max(prevJobCompletion, prevMachineCompletion) + jobs[jobId].times[machine];
        }
    }
    return C[n - 1][m - 1];
}

int calculateCsum(const std::vector<Job>& jobs, const std::vector<int>& permutation, int m) {
    int n = permutation.size();
    std::vector<std::vector<int>> completionTimes(n, std::vector<int>(m, 0));

    completionTimes[0][0] = jobs[permutation[0]].times[0];
    for (int i = 1; i < n; ++i) {
        completionTimes[i][0] = completionTimes[i - 1][0] + jobs[permutation[i]].times[0];
    }
    for (int j = 1; j < m; ++j) {
        completionTimes[0][j] = completionTimes[0][j - 1] + jobs[permutation[0]].times[j];
    }
    for (int i = 1; i < n; ++i) {
        for (int j = 1; j < m; ++j) {
            completionTimes[i][j] = std::max(completionTimes[i - 1][j], completionTimes[i][j - 1]) + jobs[permutation[i]].times[j];
        }
    }

    int csum = 0;
    for (int i = 0; i < n; ++i) {
        csum += completionTimes[i][m - 1];
    }
    return csum;
}
