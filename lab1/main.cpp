#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <climits>
#include <fstream>
#include <sstream>
#include <numeric>

using namespace std;

struct Job {
    int r;
    int p;
    int q; 
};

// Sort by r value
vector<Job> sort_by_r(vector<Job> jobs) {
    sort(jobs.begin(), jobs.end(), [](const Job& a, const Job& b) {
        return a.r < b.r;
    });
    return jobs;
}

// Sort by q value
vector<Job> sort_by_q(vector<Job> jobs) {
    sort(jobs.begin(), jobs.end(), [](const Job& a, const Job& b) {
        return a.q > b.q;
    });
    return jobs;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Correct usage : " << argv[0] << " [input file]" << endl;
        return 1;
    }

    string filename = argv[1];
    istream* input;
    ifstream file;

    // Opening file
    if (filename == "-") {
        input = &cin;
    } else {
        file.open(filename);
        if (!file.is_open()) {
            cerr << "Error during opening file " << filename << endl;
            return 1;
        }
        input = &file;
    }

    vector<Job> jobs;
    string line;

    // Get amount of record
    getline(*input, line);
    stringstream ssa(line);
    int amount;
    ssa >> amount;

    // Read record to vector
    for (int i = 0; i < amount; i++)
    {
        getline(*input, line);
        stringstream ss(line);
        int r,p,q;
        ss >> r >> p >> q;
        jobs.push_back({r, p, q});
    }
    printf("Nie posortowane\n");
    for (auto job : jobs)
    {
        printf("%d %d %d\n", job.r, job.p, job.q);
    }
    vector<Job> sort_r = sort_by_r(jobs);
    printf("Sortowane po r\n");
    for (auto job : sort_r)
    {
        printf("%d %d %d\n", job.r, job.p, job.q);
    }
    vector<Job> sort_q = sort_by_q(jobs);
    printf("Sortowane po q\n");
    for (auto job : sort_q)
    {
        printf("%d %d %d\n", job.r, job.p, job.q);
    }
    
}