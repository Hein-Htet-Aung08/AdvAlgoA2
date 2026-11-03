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

std::vector<int> deterministic_quicksort(std::vector<int> &input, int lowerLimit, int upperLimit) {
    if (upperLimit - lowerLimit <= 1) {
        return input;
    }
    int pivot {input[lowerLimit]};
    int boundary {lowerLimit};
    for (int i {lowerLimit + 1} ; i < upperLimit ; i++) {
        if (input[i] < pivot) {
            std::swap(input[i], input[boundary]);
            boundary++;
        }
    }

    //lower
    deterministic_quicksort(input, lowerLimit, boundary);

    //upper
    deterministic_quicksort(input, boundary + 1, upperLimit);
    
    return input;
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

    std::vector<int> output {deterministic_quicksort(input, 0, (int)input.size())};
    if (sorted == output) {
        std::cout << "Match!\n";
    } else {
        std::cout << "Mismatch!\nStd: ";
        prtVec(sorted);
        std::cout << "Dtm: ";
        prtVec(output);
    }
    return {};
}