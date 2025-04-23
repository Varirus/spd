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
    std::cout << std::endl << "LSA" << std::endl;
    Result result_lsa = lsa(2, jobs);
    printResult(result_lsa);
    std::cout << std::endl << "LPT" << std::endl;
    Result result_lpt = lpt(2, jobs);
    printResult(result_lpt);
    std::cout << std::endl << "Programowanie dynamiczne" << std::endl;
    Result result_pd = solveP2Cmax(jobs);
    printResult(result_pd);
    std::cout << std::endl << "Przeglad zupelny" << std::endl;
    Result result_bnb = solveP2Cmax_BnB(jobs);
    printResult(result_bnb);
    
}