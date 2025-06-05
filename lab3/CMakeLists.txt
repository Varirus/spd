cmake_minimum_required(VERSION 3.10)
project(Lab3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra -pedantic -Werror=return-type -Werror=parentheses -Werror=vla -Werror=array-bounds -Wcast-qual -Wmaybe-uninitialized )

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/main.cpp
    src/Structs.cpp
    src/Algorithms.cpp
    src/Utilities.cpp
)

# Executable
add_executable(lab3 ${SOURCES})