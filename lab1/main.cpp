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
    int id;
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

// Compute makespan using r and p
int compute_makespan_rp(vector<Job> jobs) {
    int C_prev = 0;
    int C_act = 0;
    bool prime = true;
    for (auto job : jobs)
    {
        if(prime)
            C_prev = job.r + job.p;
        else{
            C_act = max(C_prev, job.r) + job.p;
            C_prev = C_act;
        }
            
        prime = false;
    }
    return C_act;
}
// Compute makespan using r and p and q
int compute_makespan_rpq(vector<Job> jobs) {
    int C_prev = 0;
    int C_act = 0;
    bool prime = true;
    for (auto job : jobs)
    {
        if(prime)
            C_prev = job.r + job.p + job.q;
        else{
            C_act = max(C_prev, job.r) + job.p + job.q;
            C_prev = C_act;
        }
            
        prime = false;
    }
    return C_act;
}

// Factorial function
int fact(int n){
    return (n==0) || (n==1) ? 1 : n* fact(n-1);
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
    int id = 0;
    // Read record to vector
    for (int i = 0; i < amount; i++)
    {
        getline(*input, line);
        stringstream ss(line);
        int r,p,q;
        ss >> r >> p >> q;
        id++;
        jobs.push_back({id, r, p, q});
    }
   
    vector<Job> best_permutation_rp = jobs;
    int best_value_rp = 0;

    vector<Job> best_permutation_rpq = jobs;
    int best_value_rpq = 0;
   
    printf("Nie posortowane\n");
    for (auto job : jobs)
    {
        printf("%d %d %d %d\n", job.id, job.r, job.p, job.q);
        
    }
    printf("Makespan rp: %d\n", compute_makespan_rp(jobs));
    printf("Makespan rpq: %d\n\n", compute_makespan_rpq(jobs));
    vector<Job> sort_r = sort_by_r(jobs);
    printf("Sortowane po r\n");
    for (auto job : sort_r)
    {
        printf("%d %d %d %d\n",job.id, job.r, job.p, job.q);
    }
    printf("Makespan rp: %d\n", compute_makespan_rp(sort_r));
    printf("Makespan rpq: %d\n\n",compute_makespan_rpq(sort_r));
    vector<Job> sort_q = sort_by_q(jobs);
    printf("Sortowane po q\n");
    for (auto job : sort_q)
    {
        printf("%d %d %d %d\n", job.id, job.r, job.p, job.q);
    }
    printf("Makespan rp: %d\n", compute_makespan_rp(sort_q));
    printf("Makespan rpq: %d\n\n", compute_makespan_rpq(sort_q));
    // Permutations
    printf("Factorial z %d to %d\n\n", amount, fact(amount));
    for (int i = 0; i < fact(amount); i++)
    {
        next_permutation(jobs.begin(), jobs.end(), [](const Job& a, const Job& b) {
            return a.id > b.id;
        });
        //printf("Permutacja %d\n", i);
        /*for (auto job : jobs)
        {
            printf("%d %d %d %d\n", job.id, job.r, job.p, job.q);
        }*/
        int value = compute_makespan_rp(jobs);
        if(value > best_value_rp){
            best_value_rp = value;
            best_permutation_rp = jobs;
        }
        value = compute_makespan_rpq(jobs);
        if(value > best_value_rpq){
            best_value_rpq = value;
            best_permutation_rpq = jobs;
        }
    }
    printf("Best Permutation rp\n");
    for (auto job : best_permutation_rp)
    {
        printf("%d %d %d %d\n", job.id, job.r, job.p, job.q);
    }
    printf("Makespan rp: %d\n", best_value_rpq);
    printf("Makespan rpq: %d\n\n", compute_makespan_rpq(best_permutation_rp));
    printf("Best Permutation rpq\n");
    for (auto job : best_permutation_rpq)
    {
        printf("%d %d %d %d\n", job.id, job.r, job.p, job.q);
    }
    printf("Makespan rp: %d\n", compute_makespan_rp(best_permutation_rpq));
    printf("Makespan rpq: %d\n\n", best_value_rpq);
}