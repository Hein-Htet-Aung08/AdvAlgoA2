#pragma once

#include "quicksort_metrics.hpp"

std::vector<int> randomized_quicksort(std::vector<int> input, unsigned int seed);
AnalysisResult randomized_quicksort_analysis(std::vector<int> input, unsigned int seed);
