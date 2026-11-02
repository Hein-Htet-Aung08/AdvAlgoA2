#include <vector>
#include <iostream>
#include <random>

void prtVec(std::vector<int> v) {
    for (int i : v) {
        std::cout << i << ',';
    }
    std::cout << '\n';
}

std::vector<int> randomized_quicksort(std::vector<int> input) {
    input.push_back(4);
    return input;
}

void helperRDA(std::vector<int>& input, int A, int low, int high){
    if(low >= high) return;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distrib(low, high);   

    int pivotIndex = distrib(gen);

    
}

int main(int argc, char* argv[]) {
    std::vector<int> test {0,1,2};
    std::vector<int> output {randomized_quicksort(test)};
    std::cout << (test == output) << '\n';
    if (test != output) {
        prtVec(test);
        prtVec(output);
    }
    return {};
}