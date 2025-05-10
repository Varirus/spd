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

    std::cout << std::endl << "LSA" << std::endl;
    Result result_lsa = lsa(2, jobs);
    printResult(result_lsa);

    std::cout << std::endl << "LPT" << std::endl;
    Result result_lpt = lpt(2, jobs);
    printResult(result_lpt);

    std::cout << std::endl << "Programowanie dynamiczne P2" << std::endl;
    Result result_pdP2 = solveP2Cmax_PD(jobs);
    printResult(result_pdP2);

    std::cout << std::endl << "PTAS P2" << std::endl;
    Result result_PTASP2 = solveP2Cmax_PTAS(jobs, 1);
    printResult(result_PTASP2);

    std::cout << std::endl << "Przeglad zupelny P2" << std::endl;
    Result result_pzP2 = solveP2Cmax_PZ(jobs);
    printResult(result_pzP2);
    
    std::cout << std::endl << "Przeglad zupelny P3" << std::endl;
    Result result_pzP3 = solveP3Cmax_PZ(jobs);
    printResult(result_pzP3);

    std::cout << std::endl << "Programowanie dynamiczne P3" << std::endl;
    Result result_pdP3 = solveP3Cmax_PD(jobs);
    printResult(result_pdP3);

}
