#include <iostream>
#include <string>
#include <optional>
#include <cstdint>
#include <vector>
#include <cstdint>
#include <cstddef>

//Stucts
struct BenchmarkConfig {
    std::vector<std::size_t> counts {100, 1000, 5000, 10000};
    std::vector<unsigned int> randomizedSeeds {23, 48, 123321};
    std::vector<unsigned int> pivotSeeds {80, 40, 62};
    int repetitions {3};
    std::string outputDirectory {"results/"};
};

struct SortMetrics{
    std::uint64_t comparisons = 0;
    std::uint64_t swaps = 0;
    std::uint64_t maximumRecursionDepth = 0;
    double partitionImbalanaceSum = 0.0;
    std::uint64_t partitionCount = 0;
};

struct BenchmarkResult{
    std::string inputType;
    std::string algorithmName;
    std::size_t arraySize = 0;

    std::optional<unsigned int> inputSeed;
    std::optional<unsigned int> pivotSeed;

    int repetitionNumber = 0;

    std::string measurementMode; //"performance" or "analysis"

    std::optional<std::uint64_t> comparisons;
    std::optional<std::uint64_t> swaps;
    std::optional<std::uint64_t> maximumRecursionDepth;
    std::optional<double> meanPartitionImbalance;

    bool correctnessPassed = false;
    std::string runStatus;
};

//Random arrays

//Already sorted

//Reverse sorted

//Nearly sorted

//Many duplicates

//All equal elements

//Run Benchmark algorithm

//Write functions

//Main benchmark algorithm

int main(){
    std::cout << "Benchmark framework ready\n";
    return 0;
}