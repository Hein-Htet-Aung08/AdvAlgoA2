#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include "../include/deterministic_quicksort.hpp"

void prtVec(std::vector<int> v) {
    for (int i : v) {
        std::cout << i << ',';
    }
    std::cout << '\n';
}

void deterministic_quicksort(std::vector<int> &input, int lowerLimit, int upperLimit) {
    if (lowerLimit >= upperLimit) {
        return;
    }

    int pivot = input[upperLimit];
    int boundary = lowerLimit;
    for (int i = lowerLimit; i <= upperLimit - 1; i++) {
        if (input[i] < pivot) {
            std::swap(input[boundary], input[i]);
            boundary++;
        }
    }
    std::swap(input[boundary], input[upperLimit]);

    //lower
    deterministic_quicksort(input, lowerLimit, boundary - 1);
    
    //upper
    deterministic_quicksort(input, boundary + 1, upperLimit);
}

void deterministicQSHelperAnalysis(std::vector<int>& input, int lowerLimit, int upperLimit, SortMetrics& metrics, std::uint64_t depth){
    metrics.maximumRecursionDepth = std::max(metrics.maximumRecursionDepth, depth);

    if(lowerLimit >= upperLimit) return;

    int pivot = input[upperLimit];
    int boundary = lowerLimit;
    for(int i = lowerLimit; i < upperLimit; i++){
        ++metrics.comparisons;

        if(input[i] < pivot){
            std::swap(input[boundary], input[i]);
            boundary++;
            ++metrics.swaps;
        }
    }
    std::swap(input[boundary], input[upperLimit]);
    ++metrics.swaps;

    int leftSize = boundary - lowerLimit;
    int rightSize = upperLimit - boundary;
    int total = leftSize + rightSize;

    if (total > 0){
        double imbalance = static_cast<double>(std::abs(leftSize - rightSize))/total;

        metrics.partitionImbalanceSum += imbalance;
        ++metrics.partitionCount;
    }

    //lower
    deterministicQSHelperAnalysis(input, lowerLimit, boundary - 1, metrics, depth + 1);

    //upper
    deterministicQSHelperAnalysis(input, boundary + 1, upperLimit, metrics, depth + 1);
}

void deterministic_quicksort_benchmark(std::vector<int>& input){
    deterministic_quicksort(input, 0, static_cast<int>(input.size()) - 1);
}

AnalysisResult deterministic_quicksort_analysis(std::vector<int> input){
    AnalysisResult analysisResult;
    SortMetrics metrics;

    deterministicQSHelperAnalysis(input, 0, static_cast<int>(input.size()) - 1, metrics, 1);

    analysisResult.sortedArray = input;
    analysisResult.metrics = metrics;

    return analysisResult;
}
    
int main(int argc, char* argv[]) {
    std::fstream inputFile ("./src/input");
        if (!inputFile.is_open()) {
            std::cout << "file bonton\n";
        return {0};
    }

    std::vector<int> input {};

    std::string line {};
    while (std::getline(inputFile, line, ',')) {
        // std::cout << line << 'x';
        input.push_back(std::stoi(line));
    }
    prtVec(input);

    std::vector<int> input4WrapperTest {input};

    std::vector<int> sorted {input};
    std::sort(sorted.begin(), sorted.end());

    deterministic_quicksort(input, 0, (int)input.size()-1);
    if (sorted == input) {
        std::cout << "Match!\n";
    } else {
        std::cout << "Mismatch!\nStd: ";
    }
    prtVec(sorted);
    std::cout << "Dtm: ";
    prtVec(input);

    //Hein's tests

    std::cout << "..............Wrapper testers..............\n";
    
    std::vector<int> result {input4WrapperTest};
    deterministic_quicksort_benchmark(result);
    AnalysisResult analysisResult = deterministic_quicksort_analysis(input4WrapperTest);

    std::cout << "Input: ";
    prtVec(input4WrapperTest);

    std::cout << "DQS Benchmark Result: ";
    prtVec(result);

    std::cout << "DQS Analysis Result: ";
    prtVec(analysisResult.sortedArray);

    std::cout << "Same sorted result: " << (sorted == analysisResult.sortedArray) << '\n';

    std::cout << "Comparisons: " << analysisResult.metrics.comparisons << '\n';

    std::cout << "Swaps: " << analysisResult.metrics.swaps << '\n';

    std::cout << "Maximum recursion depth: " << analysisResult.metrics.maximumRecursionDepth << '\n';

    if (analysisResult.metrics.partitionCount > 0) {
        std::cout << "Mean partition imbalance: " << analysisResult.metrics.partitionImbalanceSum / analysisResult.metrics.partitionCount << '\n';
    }

    return {};
}