#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <queue>

struct Remainder {
    uint64_t dividend;
    int value;

    Remainder(const uint64_t& dividend, const int& value):
        dividend(dividend), value(value) {}
};


uint64_t findMinimalSmallDigitNumber(uint64_t number) {

    if (number < 3) {
        return number;
    }

    std::vector<bool> is_found(number, false);
    std::queue<Remainder> remainders;

    for (int init = 1; init <= 2; ++init) {
        remainders.push(Remainder(static_cast<uint64_t>(init), init));
        is_found[init] = true;
    }

    uint64_t answer = 0;

    while (!is_found[0]) {
        auto current_remainder = remainders.front();
        remainders.pop();

        uint64_t new_divident_base = current_remainder.dividend * static_cast<uint64_t>(10);

        for (int it = 0; it <= 2; ++it) {
            uint64_t new_divident = new_divident_base + it;
            int new_remainder = (current_remainder.value * 10 + it) % number;

            if (new_remainder == 0) {
                answer = new_divident;
                is_found[0] = true;
                break;
            }

            if (!is_found[new_remainder]) {
                remainders.push(Remainder(new_divident, new_remainder));
                is_found[new_remainder] = true;
            }
        }
    }

    return answer;
}

int main(int argc, char** argv) {

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    if (argc != 2) {
        std::cout << "Please provide only 1 parameter...\n";
        return 1;
    }

    uint64_t max_number = static_cast<uint64_t>(std::atoi(argv[1]));

    uint64_t answer = 0;
    for (uint64_t number = 1; number <= max_number; ++number) {
        answer += findMinimalSmallDigitNumber(number) / number;
    }

    std::cout << answer  << '\n';

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        std::cout << "Time difference = " <<
        static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()) / 1000.0 <<
        "[s]" << '\n';

    return 0;
}