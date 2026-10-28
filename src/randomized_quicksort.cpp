#include <vector>
#include <iostream>

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