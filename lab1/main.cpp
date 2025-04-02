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

// Print Jobs
void print_Jobs(vector<Job> jobs){
    printf("id r p q\n");
    for (auto job : jobs)
    {
        printf("%d. %d %d %d\n", job.id, job.r, job.p, job.q);
        
    }
    printf("Makespan rp: %d\n", compute_makespan_rp(jobs));
    printf("Makespan rpq: %d\n\n", compute_makespan_rpq(jobs));
}

// Factorial function
int fact(int n){
    if(n == 1)
        return 1;
    else
        return n*fact(n-1);
}

// Read input 
vector<Job> readInput(int argc, char* argv[]){
    if (argc < 2) {
        cerr << "Correct usage : " << argv[0] << " [input file]" << endl;
        exit(1);
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
        int r,p,q;
        ss >> r >> p >> q;
        id++;
        jobs.push_back({id, r, p, q});
    }
    return jobs;
}

void bruteForce(vector<Job> jobs, vector<Job> &best_permutation_rp, vector<Job> &best_permutation_rpq){

    int best_value_rp = 0, best_value_rpq = 0;

    for (int i = 0; i < fact(jobs.size()); i++)
    {
        next_permutation(jobs.begin(), jobs.end(), [](const Job& a, const Job& b) {
            return a.id > b.id;
        });
        int value_rp = compute_makespan_rp(jobs);
        if(value_rp > best_value_rp){
            best_value_rp = value_rp;
            best_permutation_rp = jobs;
        }
        int value_rpq = compute_makespan_rpq(jobs);
        if(value_rpq > best_value_rpq){
            best_value_rpq = value_rpq;
            best_permutation_rpq = jobs;
        }
    }
}

int main(int argc, char* argv[]) {
    vector<Job> jobs = readInput(argc, argv);
    vector<Job> best_permutation_rp;
    vector<Job> best_permutation_rpq;
    
    printf("Nie posortowane\n");
    print_Jobs(jobs);

    printf("Sortowane po r\n");
    vector<Job> sort_r = sort_by_r(jobs);
    print_Jobs(sort_r);

    printf("Sortowane po q\n");
    vector<Job> sort_q = sort_by_q(jobs);
    print_Jobs(sort_q);
    
    if(jobs.size() < 11){
        bruteForce(jobs, best_permutation_rp, best_permutation_rpq);
        printf("Best Permutation rp\n");
        print_Jobs(best_permutation_rp);

        printf("Best Permutation rpq\n");
        print_Jobs(best_permutation_rpq);
    }
    
}