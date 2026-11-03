#include <vector>
#include <iostream>
#include <random>
#include <algorithm>
#include "../include/quicksort.hpp"

void prtVec(std::vector<int> v) {
    for (int i : v) {
        std::cout << i << ',';
    }
    std::cout << '\n';
}

void helperRDA(std::vector<int>& input, int lowerLimit, int upperLimit, std::mt19937& gen){
    if(upperLimit - lowerLimit <= 1) return;

    std::uniform_int_distribution<int> distrib(lowerLimit, upperLimit-1);
    int pivotIndex = distrib(gen);
    std::swap(input[lowerLimit], input[pivotIndex]);

    int pivot = input[lowerLimit];
    int boundary = lowerLimit;

    for(int i = lowerLimit + 1; i < upperLimit; ++i){
        if(input[i] < pivot){
            ++boundary;
            std::swap(input[i], input[boundary]);
        }
    }

    std::swap(input[lowerLimit], input[boundary]);

    helperRDA(input, lowerLimit, boundary, gen);
    helperRDA(input, boundary + 1, upperLimit, gen);
}


std::vector<int> randomized_quicksort(std::vector<int> input, unsigned int seed) {
    std::mt19937 gen(seed);

    helperRDA(input, 0, static_cast<int>(input.size()), gen);

    return input;
}

int main(int argc, char* argv[]) {
    std::vector<int> test {5,4,3,2,3,2,0};
    std::vector<int> sorted = randomized_quicksort(test, 42);
    prtVec(test);
    prtVec(sorted);
    return {};
}