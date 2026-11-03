#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>

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
    int boundary = lowerLimit - 1;
    for (int i = lowerLimit; i <= upperLimit - 1; i++) {
        if (input[i] < pivot) {
            boundary++;
            std::swap(input[boundary], input[i]);
        }
    }
    std::swap(input[boundary + 1], input[upperLimit]);
    boundary++;

    //lower
    deterministic_quicksort(input, lowerLimit, boundary - 1);
    
    //upper
    deterministic_quicksort(input, boundary + 1, upperLimit);
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
    return {};
}