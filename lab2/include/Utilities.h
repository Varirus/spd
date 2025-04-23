#ifndef UTILITIES_H
#define UTILITIES_H

#include <vector>
#include "Structs.h"

std::pair<int, std::vector<Job>> readInput(int argc, char *argv[]);
void printResult(Result result);

#endif