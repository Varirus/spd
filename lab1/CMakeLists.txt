cmake_minimum_required(VERSION 3.10)
project(SchedulingAlgorithms)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/main.cpp
    src/Job.cpp
    src/Comparators.cpp
    src/Algorithms.cpp
    src/Utilities.cpp
)

# Executable
add_executable(scheduling_algorithms ${SOURCES})