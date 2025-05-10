#ifndef UTILITIES_H
#define UTILITIES_H

#include <vector>
#include <string>
#include "Structs.h"

std::vector<Job> readInput(int argc, char *argv[]);
void printResult(Result result);
#endif