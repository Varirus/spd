#include <iostream>
#include <vector>
#include "Structs.h"
#include "Algorithms.h"
#include "Utilities.h"
#include <chrono>
#include <algorithm>
#include <cmath>

int main(int argc, char *argv[])
{
    std::vector<Job> jobs = readInput(argc, argv);

    int  n = jobs.size();

    std::cout << std::endl << "LSA" << std::endl;
    Result result_lsa = lsa(2, jobs);
    printResult(result_lsa);

    std::cout << std::endl << "LPT" << std::endl;
    Result result_lpt = lpt(2, jobs);
    printResult(result_lpt);

    std::cout << std::endl << "Programowanie dynamiczne P2" << std::endl;
    Result result_pdP2 = solveP2Cmax_PD(jobs);
    printResult(result_pdP2);

    std::cout << std::endl << "PTAS P2 K = 25" << std::endl;
    Result result_PTASP2 = solveP2Cmax_PTAS(jobs, 25);
    printResult(result_PTASP2);

    std::cout << std::endl << "FPTAS P2 K = 4" << std::endl;
    Result result_FPTASP2 = solveP2Cmax_FPTAS(jobs, 4);
    printResult(result_FPTASP2);

    std::cout << std::endl << "Przeglad zupelny P2" << std::endl;
    Result result_pzP2 = solveP2Cmax_PZ(jobs);
    printResult(result_pzP2);

    if(jobs.size() < 17){
        std::cout << std::endl << "Przeglad zupelny P3" << std::endl;
        Result result_pzP3 = solveP3Cmax_PZ(jobs);
        printResult(result_pzP3);
    }

    std::cout << std::endl << "Programowanie dynamiczne P3" << std::endl;
    Result result_pdP3 = solveP3Cmax_PD(jobs);
    printResult(result_pdP3);

    std::cout << std::endl << "PTAS P3  K = 25" << std::endl;
    Result result_PTASP3 = solveP3Cmax_PTAS(jobs, 25);
    printResult(result_PTASP3);

}
