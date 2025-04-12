#include "Utilities.h"
#include "Comparators.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <cstdio>

using namespace std;

bool debugLog = false;

vector<Job> readInput(int argc, char *argv[])
{
    if (argc < 2)
    {
        cerr << "Correct usage : " << argv[0] << " [input file]" << endl;
        exit(1);
    }

    string filename = argv[1];
    istream *input;
    ifstream file;

    // Opening file
    if (filename == "-")
    {
        input = &cin;
    }
    else
    {
        file.open(filename);
        if (!file.is_open())
        {
            cerr << "Error during opening file " << filename << endl;
            exit(1);
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
    int id = 0;

    // Read record to vector
    for (int i = 0; i < amount; i++)
    {
        getline(*input, line);
        stringstream ss(line);
        int r, p, q;
        ss >> r >> p >> q;
        id++;
        jobs.push_back({id, r, p, q});
    }
    return jobs;
}

vector<Job> sort_by_r(vector<Job> jobs)
{
    sort(jobs.begin(), jobs.end(), CompareR());
    return jobs;
}

// Sort by q value
vector<Job> sort_by_q(vector<Job> jobs)
{
    sort(jobs.begin(), jobs.end(), CompareQ());
    return jobs;
}

int compute_makespan_rp(vector<Job> jobs)
{
    int C_prev = 0;
    int C_act = 0;
    bool prime = true;
    for (auto job : jobs)
    {
        if (prime)
            C_prev = job.r + job.p;
        else
        {
            C_act = max(C_prev, job.r) + job.p;
            C_prev = C_act;
        }

        prime = false;
    }
    return C_act;
}

int compute_makespan_rpq(vector<Job> jobs)
{
    int C_prev = 0;
    int C_act = 0;
    bool prime = true;
    for (auto job : jobs)
    {
        if (prime)
            C_prev = job.r + job.p + job.q;
        else
        {
            C_act = max(C_prev, job.r) + job.p + job.q;
            C_prev = C_act;
        }

        prime = false;
    }
    return C_act;
}

void print_Jobs(vector<Job> jobs)
{
    printf("id r p q\n");
    for (auto job : jobs)
    {
        printf("%d. %d %d %d\n", job.id, job.r, job.p, job.q);
    }
    printf("\n");
}

int fact(int n)
{
    if (n == 1)
        return 1;
    else
        return n * fact(n - 1);
}
