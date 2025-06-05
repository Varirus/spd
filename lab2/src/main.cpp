#include <iostream>
#include <vector>
#include "Structs.h"
#include "Algorithms.h"
#include "Utilities.h"
#include <chrono>
#include <algorithm>
#include <cmath>
#include <chrono>

int main(int argc, char *argv[])
{
    std::vector<Job> jobs = readInput(argc, argv);
    int n = jobs.size();

    std::cout << std::endl
              << "LSA" << std::endl;
    {
        auto start = std::chrono::high_resolution_clock::now();
        Result result_lsa = lsa(2, jobs);
        auto stop = std::chrono::high_resolution_clock::now();
        printResult(result_lsa);
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        std::cout << "Czas wykonania: " << duration.count() << " ms" << std::endl;
    }

    std::cout << std::endl
              << "LPT" << std::endl;
    {
        auto start = std::chrono::high_resolution_clock::now();
        Result result_lpt = lpt(2, jobs);
        auto stop = std::chrono::high_resolution_clock::now();
        printResult(result_lpt);
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        std::cout << "Czas wykonania: " << duration.count() << " ms" << std::endl;
    }

    std::cout << std::endl
              << "Programowanie dynamiczne P2" << std::endl;
    {
        auto start = std::chrono::high_resolution_clock::now();
        Result result_pdP2 = solveP2Cmax_PD(jobs);
        auto stop = std::chrono::high_resolution_clock::now();
        printResult(result_pdP2);
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        std::cout << "Czas wykonania: " << duration.count() << " ms" << std::endl;
    }
    if (jobs.size() < 21)
    {
        std::cout << std::endl
                  << "PTAS P2 K = n/2" << std::endl;
        {
            int x = n / 2;
            auto start = std::chrono::high_resolution_clock::now();
            Result result_PTASP2 = solveP2Cmax_PTAS(jobs, n / 2);
            auto stop = std::chrono::high_resolution_clock::now();
            printResult(result_PTASP2);
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
            std::cout << "Czas wykonania: " << duration.count() << " ms" << std::endl;
        }

        std::cout << std::endl
                  << "PTAS P2 K = 2n/3" << std::endl;
        {
            auto start = std::chrono::high_resolution_clock::now();
            Result result_PTASP22 = solveP2Cmax_PTAS(jobs, 2 * n / 3);
            auto stop = std::chrono::high_resolution_clock::now();
            printResult(result_PTASP22);
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
            std::cout << "Czas wykonania: " << duration.count() << " ms" << std::endl;
        }

        std::cout << std::endl
                  << "PTAS P2 K = 3n/4" << std::endl;
        {
            auto start = std::chrono::high_resolution_clock::now();
            Result result_PTASP2 = solveP2Cmax_PTAS(jobs, 3 * n / 4);
            auto stop = std::chrono::high_resolution_clock::now();
            printResult(result_PTASP2);
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
            std::cout << "Czas wykonania: " << duration.count() << " ms" << std::endl;
        }
    }
    std::cout << std::endl
              << "FPTAS P2 K = 2" << std::endl;
    {
        auto start = std::chrono::high_resolution_clock::now();
        Result result_FPTASP2 = solveP2Cmax_FPTAS(jobs, 2);
        auto stop = std::chrono::high_resolution_clock::now();
        printResult(result_FPTASP2);
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        std::cout << "Czas wykonania: " << duration.count() << " ms" << std::endl;
    }

    std::cout << std::endl
              << "FPTAS P2 K = 3" << std::endl;
    {
        auto start = std::chrono::high_resolution_clock::now();
        Result result_FPTASP2 = solveP2Cmax_FPTAS(jobs, 3);
        auto stop = std::chrono::high_resolution_clock::now();
        printResult(result_FPTASP2);
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        std::cout << "Czas wykonania: " << duration.count() << " ms" << std::endl;
    }

    std::cout << std::endl
              << "FPTAS P2 K = 4" << std::endl;
    {
        auto start = std::chrono::high_resolution_clock::now();
        Result result_FPTASP2 = solveP2Cmax_FPTAS(jobs, 4);
        auto stop = std::chrono::high_resolution_clock::now();
        printResult(result_FPTASP2);
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        std::cout << "Czas wykonania: " << duration.count() << " ms" << std::endl;
    }

    std::cout << std::endl
              << "Przeglad zupelny P2" << std::endl;
    {
        auto start = std::chrono::high_resolution_clock::now();
        Result result_pzP2 = solveP2Cmax_PZ(jobs);
        auto stop = std::chrono::high_resolution_clock::now();
        printResult(result_pzP2);
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        std::cout << "Czas wykonania: " << duration.count() << " ms" << std::endl;
    }

    if (jobs.size() < 17)
    {
        std::cout << std::endl
                  << "Przeglad zupelny P3" << std::endl;
        {
            auto start = std::chrono::high_resolution_clock::now();
            Result result_pzP3 = solveP3Cmax_PZ(jobs);
            auto stop = std::chrono::high_resolution_clock::now();
            printResult(result_pzP3);
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
            std::cout << "Czas wykonania: " << duration.count() << " ms" << std::endl;
        }
    }

    std::cout << std::endl
              << "Programowanie dynamiczne P3" << std::endl;
    {
        auto start = std::chrono::high_resolution_clock::now();
        Result result_pdP3 = solveP3Cmax_PD(jobs);
        auto stop = std::chrono::high_resolution_clock::now();
        printResult(result_pdP3);
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        std::cout << "Czas wykonania: " << duration.count() << " ms" << std::endl;
    }
    if (jobs.size() < 21)
    {
        std::cout << std::endl
                  << "PTAS P3  K = n/2" << std::endl;
        {
            auto start = std::chrono::high_resolution_clock::now();
            Result result_PTASP3 = solveP3Cmax_PTAS(jobs, n / 2);
            auto stop = std::chrono::high_resolution_clock::now();
            printResult(result_PTASP3);
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
            std::cout << "Czas wykonania: " << duration.count() << " ms" << std::endl;
        }

        std::cout << std::endl
                  << "PTAS P3  K = 2n/3" << std::endl;
        {
            auto start = std::chrono::high_resolution_clock::now();
            Result result_PTASP3 = solveP3Cmax_PTAS(jobs, 2 * n / 3);
            auto stop = std::chrono::high_resolution_clock::now();
            printResult(result_PTASP3);
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
            std::cout << "Czas wykonania: " << duration.count() << " ms" << std::endl;
        }

        std::cout << std::endl
                  << "PTAS P3  K = 3n/4" << std::endl;
        {
            auto start = std::chrono::high_resolution_clock::now();
            Result result_PTASP3 = solveP3Cmax_PTAS(jobs, 3 * n / 4);
            auto stop = std::chrono::high_resolution_clock::now();
            printResult(result_PTASP3);
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
            std::cout << "Czas wykonania: " << duration.count() << " ms" << std::endl;
        }
    }
    return 0;
}