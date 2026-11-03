cmake_minimum_required(VERSION 3.16)

project(AdvAlgoA2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(benchmark
    benchmarks/main.cpp
    src/deterministic_quicksort.cpp
    src/randomized_quicksort.cpp
)