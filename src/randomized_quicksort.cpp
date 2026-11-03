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

void randomizedQSHelper(std::vector<int>& input, int lowerLimit, int upperLimit, std::mt19937& gen){
    if(lowerLimit >= upperLimit) return;

    std::uniform_int_distribution<int> distrib(lowerLimit, upperLimit);
    int pivotIndex = distrib(gen);
    std::swap(input[pivotIndex], input[upperLimit]);

    int pivot = input[upperLimit];
    int boundary = lowerLimit;

    for(int i = lowerLimit; i < upperLimit; ++i){
        if(input[i] < pivot){
            std::swap(input[i], input[boundary]);
            ++boundary;
        }
    }

    std::swap(input[boundary], input[upperLimit]);

    randomizedQSHelper(input, lowerLimit, boundary - 1, gen);
    randomizedQSHelper(input, boundary + 1, upperLimit, gen);
}

void randomizedQSHelperAnalysis(std::vector<int>& input, int lowerLimit, int upperLimit, std::mt19937& gen, SortMetrics& metrics, std::uint64_t depth){
    metrics.maximumRecursionDepth = std::max(metrics.maximumRecursionDepth, depth);

    if(lowerLimit >= upperLimit) return;

    std::uniform_int_distribution<int> distrib(lowerLimit, upperLimit);
    int pivotIndex = distrib(gen);
    std::swap(input[pivotIndex], input[upperLimit]);
    ++metrics.swaps;

    int pivot = input[upperLimit];
    int boundary = lowerLimit;

    for(int i = lowerLimit; i < upperLimit; ++i){
        ++metrics.comparisons;

        if(input[i]<pivot){
            std::swap(input[i], input[boundary]);
            ++boundary;
            ++metrics.swaps;
        }
    }

    std::swap(input[boundary], input[upperLimit]);
    ++metrics.swaps;

    int leftSize = boundary - lowerLimit;
    int rightSize = upperLimit - boundary;

    int total = leftSize + rightSize;
    if(total > 0){
        double imbalance = static_cast<double>(std::abs(leftSize-rightSize))/total;

        metrics.partitionImbalanceSum += imbalance;
        ++metrics.partitionCount;
    }

    randomizedQSHelperAnalysis(input, lowerLimit, boundary-1, gen, metrics, depth+1);
    randomizedQSHelperAnalysis(input, boundary+1, upperLimit, gen, metrics, depth+1);
}

std::vector<int> randomized_quicksort(std::vector<int> input, unsigned int seed) {
    std::mt19937 gen(seed);

    randomizedQSHelper(input, 0, static_cast<int>(input.size()) - 1, gen);

    return input;
}

AnalysisResult randomized_quicksort_analysis(std::vector<int> input, unsigned int seed){
    std::mt19937 gen(seed);
    AnalysisResult analysisResult;
    SortMetrics metrics;

    randomizedQSHelperAnalysis(input, 0, static_cast<int>(input.size()) - 1, gen, metrics, 1);

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
    prtVec(analysisResult.sortedArray);

    std::cout << "Same sorted result: " << (sorted == analysisResult.sortedArray) << '\n';

    std::cout << "Comparisons: " << analysisResult.metrics.comparisons << '\n';

    std::cout << "Swaps: " << analysisResult.metrics.swaps << '\n';

    std::cout << "Maximum recursion depth: " << analysisResult.metrics.maximumRecursionDepth << '\n';

    if (analysisResult.metrics.partitionCount > 0) {
        std::cout << "Mean partition imbalance: " << analysisResult.metrics.partitionImbalanceSum / analysisResult.metrics.partitionCount << '\n';
    }

    return 0;
}