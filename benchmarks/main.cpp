#include <iostream>
#include <string>
#include <optional>
#include <cstdint>
#include <vector>
#include <cstddef>
#include <algorithm>
#include <numeric>
#include <random>
#include <stdexcept>
#include <chrono>
#include "../include/randomized_quicksort.hpp"
#include "../include/deterministic_quicksort.hpp"

//Stucts for this
struct BenchmarkConfig {
    std::vector<std::size_t> counts {100, 1000, 5000, 10000};
    std::vector<unsigned int> randomizedSeeds {23, 48, 123321};
    std::vector<unsigned int> pivotSeeds {80, 40, 62};
    int repetitions {3};
    std::string outputDirectory {"results/"};
};

struct BenchmarkResult{
    std::string inputType;
    std::string algorithmName;
    std::size_t arraySize = 0;

    std::optional<unsigned int> inputSeed;
    std::optional<unsigned int> pivotSeed;

    int repetitionNumber = 0;

    std::string measurementMode; //"performance" or "analysis"

    std::optional<double> executionTimeMilliseconds;
    std::optional<std::uint64_t> comparisons;
    std::optional<std::uint64_t> swaps;
    std::optional<std::uint64_t> maximumRecursionDepth;
    std::optional<double> meanPartitionImbalance;

    bool correctnessPassed = false;
    std::string runStatus;
};

//Random arrays
std::vector<int> randomArrays(std::size_t size, unsigned int seed){
    std::vector<int> array;

    for(std::size_t i = 0; i < size; ++i){
        array.push_back(static_cast<int>(i + 1));
    }

    std::mt19937 gen(seed);

    std::shuffle(array.begin(), array.end(), gen);

    return array;
}

//Already sorted
std::vector<int> alreadySorted(std::size_t size){
    std::vector<int> array;
    
    for(std::size_t i = 0; i < size; ++i){
        array.push_back(static_cast<int>(i+1));
    }

    return(array);
}

//Reverse sorted
std::vector<int> reverseSorted(std::size_t size){
    std::vector<int> array;

    int i = size;
    do{
        array.push_back(i);
        --i;
    }while(i > 0);

    return array;
}

//Nearly sorted
std::vector<int> nearlySorted(std::size_t size, unsigned int seed){
    std::vector<int> array = alreadySorted(size);

    std::vector<std::size_t> indices;

    for(std::size_t i = 0; i < size; ++i){
        indices.push_back(static_cast<int>(i));
    }
    
    std::mt19937 gen(seed);
    std::shuffle(indices.begin(), indices.end(), gen);

    std::size_t swapCount = size/4;

    for(std::size_t x = 0; x < swapCount; ++x){
        std::size_t i = indices[x];

        if(i != size - 1){
            std::swap(array[i], array[i+1]);
        }else{
            std::swap(array[i], array[i-1]);
        }
    }

    return array;
}

//Many duplicates
std::vector<int> manyDuplicates(std::size_t size, unsigned int seed){
    std::vector<int> array;

    std::mt19937 gen(seed);
    std::uniform_int_distribution<int> distrib(0, 9);

    for(std::size_t i = 0; i < size; ++i){
        array.push_back(distrib(gen));
    }

    return array;
}

//All equal elements
std::vector<int> allEqualElements(std::size_t size){
    return std::vector<int>(size, 1);
}

//generate input function
std::vector<int> generateInput(const std::string& inputType, std::size_t size, unsigned int seed = 0){
    if(inputType == "random"){
        return randomArrays(size, seed);
    }else if(inputType == "sorted"){
        return alreadySorted(size);
    }else if(inputType == "reverse"){
        return reverseSorted(size);
    }else if(inputType == "nearly_sorted"){
        return nearlySorted(size, seed);
    }else if(inputType == "many_duplicates"){
        return manyDuplicates(size, seed);
    }else if(inputType == "all_equal"){
        return allEqualElements(size);
    }

    throw std::invalid_argument("Unknown input type: " + inputType);
}

//Run Benchmark algorithm
BenchmarkResult runSingleBenchmark(
    const std::vector<int>& originalArray,
    const std::string& inputType,
    const std::string& algorithmName,
    std::optional<unsigned int> inputSeed,
    std::optional<unsigned int> pivotSeed,
    int repetitionNumber
){

}

//Write functions

//Main benchmark algorithm

int main(){
    std::cout << "Benchmark framework ready\n";

    std::vector<int> test = generateInput("many_duplicates", 20, 42);

    for(int value: test){
        std::cout << value << " ";
    }
    std::cout << "\n";

    return 0;
}