#include "Utilities.h"
#include "Comparators.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <cstdio>
#include <cmath>
#include <string>

using namespace std;

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
    int n;
    ssa >> n;
    int id = 0;
    getline(*input, line);
    stringstream ss(line);
    // Read record to vector
    for (int i = 0; i < n; i++)
    {
        int p;
        ss >> p;
        id++;
        jobs.push_back({id, p});
    }
    return jobs;
}

void printResult(Result result){
    std::cout << "C_max: " << result.C_max << std::endl;
    
    for (size_t i = 0; i < result.perm.size(); i++)
    {
        std::cout << "Machine " << i << ": ";
        for (size_t j = 0; j < result.perm.at(i).jobs.size(); j++)
        {
            std::cout << result.perm.at(i).jobs.at(j).p << " ";
        }
        
        std::cout << std::endl;
    }
    std::cout << std::endl;
}