#include <iostream>
#include <cstdlib>
#include <chrono>
#include <climits>
#include <vector>
#include <algorithm>


uint64_t sumOfDigits(uint64_t number) {

    uint64_t answer = 0;

    while (number > 0) {
        answer += number % 10;
        number /= 10;
    }

    return answer;
}


int main(int argc, char* argv[]) {

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    if (argc != 2) {
        std::cout << "Please provide only 1 parameter " <<
            "(the nth term)...\n";
        return 1;
    }

    uint64_t term_number = std::atoll(argv[1]);
    std::vector<uint64_t> interesting_numbers;

    constexpr uint64_t MAX_DIGIT_SUM = 163;

    for (uint64_t digit_sum = 2; digit_sum <= MAX_DIGIT_SUM; ++digit_sum) {

        uint64_t candidate = digit_sum;
        while (candidate <= UINT64_MAX / digit_sum) {

            candidate *= digit_sum;

            if (candidate < 10) {
                continue;
            }

            if (sumOfDigits(candidate) == digit_sum) {
                interesting_numbers.push_back(candidate);
            }

        }

    }

    std::sort(interesting_numbers.begin(), interesting_numbers.end());

    if (term_number > interesting_numbers.size()) {
        std::cout << "oops, the answer is at least 2^64, it doesn't compute\n";
        return 1;
    }

    std::cout << interesting_numbers[term_number - 1] << '\n';

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        std::cout << "Time difference = " <<
        static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()) <<
        "[μs]" << '\n';

    return 0;
}