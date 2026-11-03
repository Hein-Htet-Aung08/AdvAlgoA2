#pragma once

#include "quicksort_metrics.hpp"

void randomized_quicksort(std::vector<int>& input, const unsigned int& seed);
AnalysisResult randomized_quicksort_analysis(std::vector<int> input, const unsigned int& seed);