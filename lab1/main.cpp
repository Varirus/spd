#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <climits>
#include <fstream>
#include <sstream>
#include <numeric>
#include <chrono>

using namespace std;

bool debugLog = false;

struct Job
{
    int id;
    int r;
    int p;
    int q;
};

// Comparator q
struct CompareQ
{
    bool operator()(const Job &a, const Job &b)
    {
        return a.q > b.q;
    }
};

// Comparator r
struct CompareR
{
    bool operator()(const Job &a, const Job &b)
    {
        return a.r < b.r;
    }
};

// Comparator q reversed
struct CompareQ_Rev
{
    bool operator()(const Job &a, const Job &b)
    {
        return a.q < b.q;
    }
};

// Comparator r reversed
struct CompareR_Rev
{
    bool operator()(const Job &a, const Job &b)
    {
        return a.r > b.r;
    }
};

// Read input
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

// Sort by r value
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

// Compute makespan using r and p
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

// Compute makespan using r and p and q
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

// Print Jobs
void print_Jobs(vector<Job> jobs)
{
    printf("id r p q\n");
    for (auto job : jobs)
    {
        printf("%d. %d %d %d\n", job.id, job.r, job.p, job.q);
    }
    printf("\n");
}

// Factorial function
int fact(int n)
{
    if (n == 1)
        return 1;
    else
        return n * fact(n - 1);
}

void bruteForce(vector<Job> jobs, vector<Job> &best_permutation_rp, vector<Job> &best_permutation_rpq)
{

    int best_value_rp = compute_makespan_rp(jobs) + 1, best_value_rpq = compute_makespan_rpq(jobs) + 1;
    auto start = std::chrono::high_resolution_clock::now();
    int x = 0;
    int facty = fact(jobs.size());
    for (int i = 0; i < facty; i++)
    {
        next_permutation(jobs.begin(), jobs.end(), [](const Job &a, const Job &b)
                         { return a.id > b.id; });
        int value_rp = compute_makespan_rp(jobs);
        if (value_rp < best_value_rp)
        {
            best_value_rp = value_rp;
            best_permutation_rp = jobs;
        }
        int value_rpq = compute_makespan_rpq(jobs);
        if (value_rpq < best_value_rpq)
        {
            best_value_rpq = value_rpq;
            best_permutation_rpq = jobs;
        }
        if (debugLog)
        {
            auto endnow = std::chrono::high_resolution_clock::now();
            auto durationnow = std::chrono::duration_cast<std::chrono::seconds>(endnow - start);
            if ((int)durationnow.count() >= x)
            {
                x++;
                printf("Czas : %ds\n", (int)durationnow.count());
                printf("Iteracja: %d/%d\n", i, facty);
            }
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    printf("Czas wykonania: %d.%.3ds\n\n", (int)duration.count() / 1000, (int)duration.count() % 1000);
    printf("Makespan po rp\n");
    print_Jobs(best_permutation_rp);
    printf("C_max: %d\n\n", best_value_rp);
    printf("Makespan po rpq\n");
    print_Jobs(best_permutation_rpq);
    printf("C_max: %d\n\n", best_value_rpq);
}

void schrage(vector<Job> jobs, vector<Job> &best_permutation, int &C_max)
{
    priority_queue<Job, vector<Job>, CompareR_Rev> priorityJobsR(jobs.begin(), jobs.end());
    priority_queue<Job, vector<Job>, CompareQ_Rev> priorityJobsQ;
    C_max = 0;
    int t = 0;
    auto start = std::chrono::high_resolution_clock::now();

    while (!(priorityJobsR.empty()) || !(priorityJobsQ.empty()))
    {
        // Debug Logging
        if (debugLog)
        {
            priority_queue<Job, vector<Job>, CompareR_Rev> prioR = priorityJobsR;
            priority_queue<Job, vector<Job>, CompareQ_Rev> prioQ = priorityJobsQ;
            std::vector<Job> vecQ;
            for (; !prioQ.empty(); prioQ.pop())
                vecQ.emplace_back(prioQ.top());
            std::vector<Job> vecR;
            for (; !prioR.empty(); prioR.pop())
                vecR.emplace_back(prioR.top());
            printf("======= t = %d ==============\n", t);
            printf("Priority Queue R\n");
            print_Jobs(vecR);
            printf("Priority Queue Q\n");
            print_Jobs(vecQ);
            printf("==========================\n");
        }

        // If job becomes avaliable move it to priorityJobQ
        while (!priorityJobsR.empty() && priorityJobsR.top().r <= t)
        {
            priorityJobsQ.push(priorityJobsR.top());
            priorityJobsR.pop();
        }

        // If we don't have job, time skip
        if (priorityJobsQ.empty())
        {
            t = priorityJobsR.top().r;
        }
        // Complete currently avaliable most profitable job
        else
        {
            Job current = priorityJobsQ.top();
            priorityJobsQ.pop();
            best_permutation.push_back(current);
            t += current.p;
            C_max = max(C_max, t + current.q);
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

    printf("Czas wykonania: %d.%.3dus\n\n", (int)duration.count() / 1000, (int)duration.count() % 1000);
    print_Jobs(best_permutation);
    printf("C_max: %d\n\n", C_max);
}

void preemptiveSchrage(vector<Job> jobs, vector<Job> &best_permutation, int &C_max)
{
    priority_queue<Job, vector<Job>, CompareR_Rev> priorityJobsR(jobs.begin(), jobs.end());
    priority_queue<Job, vector<Job>, CompareQ_Rev> priorityJobsQ;
    C_max = 0;
    int t = 0;
    Job current;
    bool hasJob = false;
    auto start = std::chrono::high_resolution_clock::now();

    while (!(priorityJobsR.empty()) || !(priorityJobsQ.empty()) || hasJob)
    {
        // Debug Logging
        if (debugLog)
        {
            priority_queue<Job, vector<Job>, CompareR_Rev> prioR = priorityJobsR;
            priority_queue<Job, vector<Job>, CompareQ_Rev> prioQ = priorityJobsQ;
            std::vector<Job> vecQ;
            for (; !prioQ.empty(); prioQ.pop())
                vecQ.emplace_back(prioQ.top());
            std::vector<Job> vecR;
            for (; !prioR.empty(); prioR.pop())
                vecR.emplace_back(prioR.top());
            printf("======= t = %d ==============\n", t);
            printf("Priority Queue R\n");
            print_Jobs(vecR);
            printf("Priority Queue Q\n");
            print_Jobs(vecQ);
            printf("==========================\n");
        }

        // If job becomes avaliable move it to priorityJobQ
        while (!priorityJobsR.empty() && priorityJobsR.top().r <= t)
        {
            priorityJobsQ.push(priorityJobsR.top());
            priorityJobsR.pop();
        }

        // If we have no current job, take one from priorityJobQ
        if (!hasJob && !priorityJobsQ.empty())
        {
            current = priorityJobsQ.top();
            priorityJobsQ.pop();
            hasJob = true;
        }

        if (hasJob)
        {
            // Computing time we can skip to next operation, either current job time or time till new job becomes avaliable
            int timeToSkip;
            if (!priorityJobsR.empty())
            {
                int timeToNextJobFromR = priorityJobsR.top().r;
                timeToSkip = min(current.p, timeToNextJobFromR - t);
            }
            else
                timeToSkip = current.p;

            t += timeToSkip;
            current.p -= timeToSkip;

            // If during time skip job becomes avaliable, move it to priorityJobQ
            while (!priorityJobsR.empty() && priorityJobsR.top().r <= t)
            {
                priorityJobsQ.push(priorityJobsR.top());
                priorityJobsR.pop();
            }
            
            // Complete job
            if (current.p == 0)
            {
                best_permutation.push_back(*find_if(jobs.begin(), jobs.end(),[current](const Job &job){ return job.id == current.id; }));
                C_max = max(C_max, t + current.q);
                hasJob = false;
            }
            // If there is avaliable job that is more profitable, swap job (push current job back to priorityJobQ)
            else if (!priorityJobsQ.empty() && priorityJobsQ.top().q > current.q)
            {
                priorityJobsQ.push(current);
                hasJob = false;
            }
        }
        // If we don't have job and there are still jobs, but not avaliable, time skip
        else if (!priorityJobsR.empty())
        {
            t = priorityJobsR.top().r;
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

    printf("Czas wykonania: %d.%.3dus\n\n", (int)duration.count() / 1000, (int)duration.count() % 1000);
    print_Jobs(best_permutation);
    printf("C_max: %d\n\n", C_max);
}

int main(int argc, char *argv[])
{
    vector<Job> jobs = readInput(argc, argv);
    vector<Job> best_permutation_rp;
    vector<Job> best_permutation_rpq;
    vector<Job> schrage_permutation;
    vector<Job> preemptiveSchrage_permutation;

    printf("------------------------------\n");
    printf("Input:\n");
    print_Jobs(jobs);

    /*printf("Sortowane po r\n");
    vector<Job> sort_r = sort_by_r(jobs);
    print_Jobs(sort_r);

    printf("Sortowane po q\n");
    vector<Job> sort_q = sort_by_q(jobs);
    print_Jobs(sort_q);*/
    printf("------------------------------\n");
    printf("Schrage:\n");
    int C_max_schrage;
    schrage(jobs, schrage_permutation, C_max_schrage);

    printf("------------------------------\n");
    printf("Preemptive Schrage:\n");
    int C_max_preemptiveSchrage;
    preemptiveSchrage(jobs, preemptiveSchrage_permutation, C_max_preemptiveSchrage);

    if (jobs.size() < 11)
    {
        printf("------------------------------\n");
        printf("Brute Force:\n");
        bruteForce(jobs, best_permutation_rp, best_permutation_rpq);
    }
    printf("------------------------------\n");
}