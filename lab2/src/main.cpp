#include <iostream>
#include <vector>
#include "Structs.h"
#include "Algorithms.h"
#include "Utilities.h"
#include <chrono>


int main(int argc, char *argv[])
{
    std::pair<int, std::vector<Job>> input = readInput(argc, argv);
    int m = input.first;
    std::vector<Job> jobs = input.second;
    Result result_lsa = lsa(m, jobs);
    std::cout << "C_max: " << result_lsa.C_max << std::endl;
    for (int i = 0; i < result_lsa.perm.size(); i++)
    {
        for (int j = 0; j < result_lsa.perm.at(i).jobs.size(); j++)
        {
            std::cout << result_lsa.perm.at(i).jobs.at(j).p << " ";
        }
        
        std::cout << std::endl;
    }
    std::cout << std::endl;
    
}