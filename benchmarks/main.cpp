// Primary author: Hein Htet Aung
// Secondary author: Felix Herrmann

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
#include <utility>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <map>
#include <tuple>
#include <cmath>
#include "../include/randomized_quicksort.hpp"
#include "../include/deterministic_quicksort.hpp"

//Stucts for this
struct BenchmarkConfig {
    std::vector<std::size_t> arraySize {100, 1000, 5000, 10000, 15000};
    // std::vector<unsigned int> randomizedSeeds {23, 48, 123321,2923};
    std::vector<unsigned int> randomizedSeeds {4020006181, 3572205801, 693963546, 1533451385, 1727316755};
    std::vector<unsigned int> pivotSeeds {4393318, 3257601471, 469755461, 804904386, 3853312017};
    std::vector<std::string> inputTypes {"random", "sorted", "reverse", "nearly_sorted", "many_duplicates", "all_equal"};
    int warmupRepetitions {5};
    int repetitions {5};
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

struct TimingStats{
    std::size_t sampleCount = 0;
    
    double mean = 0.0;
    double median = 0.0;
    double standardDeviation = 0.0;
    double minimum = 0.0;
    double maximum = 0.0;
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
    int repetitionNumber,
    const std::string& measurementMode
){
    BenchmarkResult result;

    result.inputType = inputType;
    result.algorithmName = algorithmName;
    result.arraySize = originalArray.size();
    result.inputSeed = inputSeed;
    result.pivotSeed = pivotSeed;
    result.repetitionNumber = repetitionNumber;
    result.measurementMode = measurementMode;

    std::vector<int> workingArray = originalArray;

    std::vector<int> expectedArray = originalArray;
    std::sort(expectedArray.begin(), expectedArray.end());

    if(measurementMode == "performance"){
        auto start = std::chrono::steady_clock::now();
        auto end = std::chrono::steady_clock::now();

        if(algorithmName == "randomized"){

            if(!pivotSeed.has_value()) throw std::invalid_argument("Randomized Quicksort requires a pivot seed.");

            start = std::chrono::steady_clock::now();
            randomized_quicksort(workingArray, pivotSeed.value());
            end = std::chrono::steady_clock::now();
        }else if(algorithmName == "deterministic"){
            start = std::chrono::steady_clock::now();
            deterministic_quicksort_benchmark(workingArray);
            end = std::chrono::steady_clock::now();
        }else{
            throw std::invalid_argument("Unknown algorithm: " + algorithmName);
        }

        std::chrono::duration<double, std::milli> elapsed = end - start;

        result.executionTimeMilliseconds = elapsed.count();

    }else if(measurementMode == "analysis"){
        AnalysisResult analysisResult;

        if(algorithmName == "randomized"){
            if(!pivotSeed.has_value()){
                throw std::invalid_argument("Randomized Quicksort requires a pivot seed.");
            }

            analysisResult = randomized_quicksort_analysis(originalArray, pivotSeed.value());
        }else if(algorithmName == "deterministic"){
            analysisResult = deterministic_quicksort_analysis(originalArray);
        }else{
            throw std::invalid_argument("Unknown algorithm: " + algorithmName);
        }

        if(analysisResult.sortedArray == expectedArray){
            result.comparisons = analysisResult.metrics.comparisons;
            result.swaps = analysisResult.metrics.swaps;
            result.maximumRecursionDepth = analysisResult.metrics.maximumRecursionDepth;

            if(analysisResult.metrics.partitionCount > 0){
                result.meanPartitionImbalance = analysisResult.metrics.partitionImbalanceSum / analysisResult.metrics.partitionCount;
            }
        }

        workingArray = std::move(analysisResult.sortedArray);

    }else{
        throw std::invalid_argument("Unknown measurement mode: " + measurementMode);
    }

    result.correctnessPassed = (workingArray == expectedArray);
    result.runStatus = result.correctnessPassed ? "PASS" : "FAIL";
    return result;
}

std::vector<BenchmarkResult> runAllBenchmarks(const BenchmarkConfig& config){
    std::vector<BenchmarkResult> results;
    std::mt19937 scheduleGen(41052);

    for(const std::string& inputType: config.inputTypes){
        for(std::size_t size: config.arraySize){

            bool requiresSeed = inputType == "random" || inputType == "nearly_sorted" || inputType=="many_duplicates";

            std::vector<std::optional<unsigned int>> inputSeeds;

            if(requiresSeed){
                for(unsigned int seed: config.randomizedSeeds){
                    inputSeeds.push_back(seed);
                }
            }else{
                inputSeeds.push_back(std::nullopt);
            }

            for(std::optional<unsigned int> inputSeed: inputSeeds){

                std::vector<int> originalArray = generateInput(inputType, size, inputSeed.value_or(0));

                std::vector<std::optional<unsigned int>> timingSchedule {std::nullopt};
                for(unsigned int pivotSeed: config.pivotSeeds){
                    timingSchedule.push_back(pivotSeed);
                }

                for(int warmup = 0; warmup < config.warmupRepetitions; ++warmup){
                    std::shuffle(timingSchedule.begin(), timingSchedule.end(), scheduleGen);

                    for(std::optional<unsigned int> pivotSeed: timingSchedule){
                        std::string algorithmName = pivotSeed.has_value() ? "randomized" : "deterministic";

                        BenchmarkResult warmupResult = runSingleBenchmark(originalArray, inputType, algorithmName, inputSeed, pivotSeed, 0, "performance");

                        if(!warmupResult.correctnessPassed){
                            throw std::runtime_error("Correctness failure during benchmark warm-up.");
                        }
                    }
                }

                for(int repetition = 1; repetition <= config.repetitions; ++repetition){
                    std::shuffle(timingSchedule.begin(), timingSchedule.end(), scheduleGen);

                    for(std::optional<unsigned int> pivotSeed: timingSchedule){
                        std::string algorithmName = pivotSeed.has_value() ? "randomized" : "deterministic";

                        BenchmarkResult performance = runSingleBenchmark(originalArray, inputType, algorithmName, inputSeed, pivotSeed, repetition, "performance");

                        results.push_back(performance);
                    }
                }

                BenchmarkResult analysis = runSingleBenchmark(originalArray, inputType, "deterministic", inputSeed, std::nullopt, 1, "analysis");
                results.push_back(analysis);

                for(unsigned int pivotSeed : config.pivotSeeds){

                    BenchmarkResult analysis = runSingleBenchmark(originalArray, inputType, "randomized", inputSeed, pivotSeed, 1, "analysis");
                    results.push_back(analysis);

                }
            }
        }
    }

    return results;
}

//Write functions
void writeDetailedResults(const std::vector<BenchmarkResult>& results, const BenchmarkConfig& config){

    std::filesystem::create_directories(config.outputDirectory);

    for(const std::string& inputType: config.inputTypes){
        std::string filePath = config.outputDirectory + inputType + ".csv";

        std::ofstream file(filePath);

        if(!file.is_open()){
            throw std::runtime_error("Cannot create CSV file: " + filePath);
        }

        file << std::setprecision(15);

        file << "input_type,algorithm,array_size,input_seed,pivot_seed,repetition,measurement_mode,execution_time_ms,comparisons,swaps,max_recursion_depth,mean_partition_imbalance,correctness_passed,run_status\n";

        for(const BenchmarkResult& result: results){
            if(result.inputType != inputType){
                continue;
            }

            file << result.inputType << ',' << result.algorithmName << ',' << result.arraySize << ',';

            if(result.inputSeed.has_value()){
                file << result.inputSeed.value();
            }
            file << ',';

            if(result.pivotSeed.has_value()){
                file << result.pivotSeed.value();
            }
            file << ',';

            file << result.repetitionNumber << ',' << result.measurementMode << ',';

            if(result.executionTimeMilliseconds.has_value()){
                file << result.executionTimeMilliseconds.value();
            }
            file << ',';

            if(result.comparisons.has_value()){
                file << result.comparisons.value();
            }
            file << ',';

            if(result.swaps.has_value()){
                file << result.swaps.value();
            }
            file << ',';

            if(result.maximumRecursionDepth.has_value()){
                file << result.maximumRecursionDepth.value();
            }
            file << ',';

            if(result.meanPartitionImbalance.has_value()){
                file << result.meanPartitionImbalance.value();
            }
            file << ',';

            file << (result.correctnessPassed ? "true" : "false") << ',' << result.runStatus << '\n';
        }

        std::cout << "Created: " << filePath << '\n';
    }

}

TimingStats calculateTimingStats(std::vector<double> times){
    TimingStats stats;

    if(times.empty()) return stats;

    std::sort(times.begin(), times.end());

    stats.sampleCount = times.size();
    stats.minimum = times.front();
    stats.maximum = times.back();

    double sum = std::accumulate(times.begin(), times.end(), 0.0);
    stats.mean = sum / times.size();

    std::size_t middle = times.size()/2;

    if(times.size()%2 == 0){
        stats.median = (times[middle - 1] + times[middle]) / 2.0;
    }else{
        stats.median = times[middle];
    }

    double squaredDifferenceSum = 0.0;

    for(double time: times){
        double difference = time - stats.mean;
        squaredDifferenceSum += difference * difference;
    }

    if(times.size() > 1){
        stats.standardDeviation = std::sqrt(squaredDifferenceSum / (times.size() - 1));
    }

    return stats;
}

void writeDashboard(const std::vector<BenchmarkResult>& results, const BenchmarkConfig& config){
    using GroupKey = std::tuple<std::string, std::string, std::size_t>;

    std::map<GroupKey, std::vector<double>> timings;
    std::map<GroupKey, std::vector<const BenchmarkResult*>> analysisGroups;
    std::map<GroupKey, std::size_t> attemptedRuns;
    std::map<GroupKey, std::size_t> failedRuns;

    for(const BenchmarkResult& result: results){
        GroupKey key {result.inputType, result.algorithmName, result.arraySize};

        ++attemptedRuns[key];

        if(!result.correctnessPassed){
            ++failedRuns[key];
            continue;
        }

        if(result.measurementMode == "performance" && result.executionTimeMilliseconds.has_value()){
            timings[key].push_back(result.executionTimeMilliseconds.value());
        }

        if(result.measurementMode == "analysis"){
            analysisGroups[key].push_back(&result);
        }
    }

    std::filesystem::create_directories(config.outputDirectory);

    std::filesystem::path filePath = std::filesystem::path(config.outputDirectory) / "dashboard.csv";

    std::ofstream file(filePath);

    if(!file.is_open()){
        throw std::runtime_error("Cannot create dashboard.csv");
    }

    file << std::setprecision(15);
    file << "input_type,algorithm,array_size,sample_count,mean_time_ms,median_time_ms,std_dev_ms,min_time_ms,max_time_ms,analysis_samples,mean_comparisons,mean_swaps,mean_max_recursion_depth,mean_partition_imbalance,attempted_runs,passed_runs,failed_runs\n";

    for(const auto& [key, attempted]: attemptedRuns){
        std::vector<double> times;

        auto timingIt = timings.find(key);

        if(timingIt != timings.end()) times = timingIt->second;

        TimingStats stats = calculateTimingStats(times);

        const auto& [inputType, algorithmName, arraySize] = key;

        double totalComparisons = 0.0;
        double totalSwaps = 0.0;
        double totalDepth = 0.0;
        double totalImbalance = 0.0;

        std::size_t analysisCount = 0;
        std::size_t imbalanceCount = 0;

        for(const BenchmarkResult* analysis: analysisGroups[key]){
            if(analysis->comparisons.has_value() && analysis->swaps.has_value() && analysis->maximumRecursionDepth.has_value()){
                totalComparisons += analysis->comparisons.value();
                totalSwaps += analysis->swaps.value();
                totalDepth += analysis->maximumRecursionDepth.value();

                ++analysisCount;
            }

            if(analysis->meanPartitionImbalance.has_value()){
                totalImbalance += analysis->meanPartitionImbalance.value();
                ++imbalanceCount;
            }
        }

        double meanComparisons = (analysisCount > 0) ? totalComparisons / analysisCount : 0.0;
        double meanSwaps = (analysisCount > 0) ? totalSwaps / analysisCount : 0.0;
        double meanDepth = (analysisCount > 0) ? totalDepth / analysisCount : 0.0;
        double meanImbalance = (imbalanceCount > 0) ? totalImbalance / imbalanceCount : 0.0;

        file << inputType << ',' << algorithmName << ',' << arraySize << ',' << stats.sampleCount << ',';

        if(stats.sampleCount > 0){
            file << stats.mean;
        }
        file << ',';

        if(stats.sampleCount > 0){
            file << stats.median;
        }
        file << ',';
        
        if(stats.sampleCount > 1){
            file << stats.standardDeviation;
        }
        file << ',';

        if(stats.sampleCount > 0){
            file << stats.minimum;
        }
        file << ',';

        if(stats.sampleCount > 0){
            file << stats.maximum;
        }
        file << ',';

        file << analysisCount << ',';

        if(analysisCount > 0){
            file << meanComparisons;
        }
        file << ',';

        if(analysisCount > 0){
            file << meanSwaps;
        }
        file << ',';

        if(analysisCount > 0){
            file << meanDepth;
        }
        file << ',';

        if(imbalanceCount > 0){
            file << meanImbalance;
        }
        file << ',';

        std::size_t failed = failedRuns[key];
        std::size_t passed = attempted - failed;

        file << attempted << ',' << passed << ',' << failed << '\n';
    }

    std::cout << "Created: " << filePath << '\n';
}

//Main benchmark algorithm
int runBenchmarkProgram(){
    try {
        BenchmarkConfig config;

        std::cout << "----------------- Quicksort Benchmark -----------------\n";
        std::cout << "Starting benchmark...\n";

        std::vector<BenchmarkResult> results = runAllBenchmarks(config);

        std::size_t passed = 0;
        std::size_t failed = 0;

        for(const BenchmarkResult& result: results){
            if(result.correctnessPassed){
                ++passed;
            }else{
                ++failed;
            }
        }

        writeDetailedResults(results, config);
        writeDashboard(results, config);

        std::cout << "\n----------------- Benchmark Summary -----------------\n";
        std::cout << "Total runs: " << results.size() << "\n";
        std::cout << "Passed: " << passed << "\n";
        std::cout << "Failed: " << failed << "\n";

        std::cout << "Results directory: " << config.outputDirectory << "\n";
        std::cout << "CSV files generated: " << config.inputTypes.size() + 1 << '\n';

        if(failed > 0){
            std::cout << "Status: COMPLETED WITH FAILURES\n";
            return 1;
        }

        std::cout << "Status: SUCCESS\n";
        return 0;
    }catch(const std::exception& e){
        std::cerr << "Benchmark failed: " << e.what() << '\n';
        return 1;
    }
}

//main
int main(){
    return runBenchmarkProgram();
}