#include <vector>
#include <iostream>
#include <random>
#include <algorithm>
#include "../include/randomized_quicksort.hpp"

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

void helperRDAAnalysis(std::vector<int>& input, int lowerLimit, int upperLimit, std::mt19937& gen, SortMetrics& metrics, std::uint64_t depth){
    metrics.maximumRecursionDepth = std::max(metrics.maximumRecursionDepth, depth);

    if(upperLimit - lowerLimit <= 1) return;

    std::uniform_int_distribution<int> distrib(lowerLimit, upperLimit-1);
    int pivotIndex = distrib(gen);
    std::swap(input[lowerLimit], input[pivotIndex]);
    ++metrics.swaps;

    int pivot = input[lowerLimit];
    int boundary = lowerLimit;

    for(int i = lowerLimit + 1; i < upperLimit; ++i){
        ++metrics.comparisons;

        if(input[i]<pivot){
            ++boundary;
            std::swap(input[i], input[boundary]);
            ++metrics.swaps;
        }
    }

    std::swap(input[lowerLimit], input[boundary]);
    ++metrics.swaps;

    int leftSize = boundary - lowerLimit;
    int rightSize = upperLimit - boundary - 1;

    int total = leftSize + rightSize;
    if(total > 0){
        double imbalance = static_cast<double>(std::abs(leftSize-rightSize)/total);

        metrics.partitionImbalanceSum += imbalance;
        ++metrics.partitionCount;
    }

    helperRDAAnalysis(input, lowerLimit, boundary, gen, metrics, depth+1);
    helperRDAAnalysis(input, boundary+1, upperLimit, gen, metrics, depth+1);
}

std::vector<int> randomized_quicksort(std::vector<int> input, unsigned int seed) {
    std::mt19937 gen(seed);

    helperRDA(input, 0, static_cast<int>(input.size()), gen);

    return input;
}

AnalysisResult randomized_quicksort_analysis(std::vector<int> input, unsigned int seed){
    std::mt19937 gen(seed);
    AnalysisResult analysisResult;
    SortMetrics metrics;

    helperRDAAnalysis(input, 0, static_cast<int>(input.size()), gen, metrics, 1);

    analysisResult.sortedArray = input;
    analysisResult.metrics = metrics;

    return analysisResult;
}

int main(int argc, char* argv[]) {
    std::vector<int> test {3,5,2,4,1};
    std::vector<int> sorted = randomized_quicksort(test, 1233);
    AnalysisResult analysisResult = randomized_quicksort_analysis(test, 1233);

    prtVec(test);
    prtVec(sorted);

    std::cout << "Same sorted result: "
              << (sorted == analysisResult.sortedArray) << '\n';

    std::cout << "Comparisons: "
              << analysisResult.metrics.comparisons << '\n';

    std::cout << "Swaps: "
              << analysisResult.metrics.swaps << '\n';

    std::cout << "Maximum recursion depth: "
              << analysisResult.metrics.maximumRecursionDepth << '\n';

    if (analysisResult.metrics.partitionCount > 0) {
        std::cout << "Mean partition imbalance: "
                  << analysisResult.metrics.partitionImbalanceSum
                     / analysisResult.metrics.partitionCount
                  << '\n';
    }

    return 0;
}