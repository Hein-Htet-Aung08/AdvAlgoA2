#pragma once

#include <vector>
#include <cstdint>

struct SortMetrics{
    std::uint64_t comparisons = 0;
    std::uint64_t swaps = 0;
    std::uint64_t maximumRecursionDepth = 0;

    double partitionImbalanceSum = 0.0;
    std::uint64_t partitionCount = 0;
};

struct AnalysisResult {
    std::vector<int> sortedArray;
    SortMetrics metrics;
};

std::vector<int> deterministic_quicksort_benchmark(std::vector<int> input);
AnalysisResult deterministic_quicksort_analysis(std::vector<int> input);