#include "../bigint/bigint.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <string>
#include <algorithm>

std::string convertFromBase10(uint64_t number, uint64_t base) {
    std::string result = "";

    for (; number > 0; number /= base) {
        auto remainder = number % base;
        if (remainder < 10) {
            result += '0' + remainder;
        } else {
            result += 'a' + (remainder - 10);
        }
    }

    auto N = result.size();

    for (int digit = 0; digit * 2ull < N; ++digit) {
        std::swap(result[digit], result[N - 1 - digit]);
    }

    return result;
}


int main(int argc, char** argv) {

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    if (argc != 2) {
        std::cout << "Please provide only 1 parameter (max number of digits)...\n";
        return 1;
    }

    uint64_t max_number_digits = std::atoi(argv[1]);
    constexpr uint32_t base = 14;

    std::vector<std::vector<int>> digits(2, std::vector<int>(max_number_digits, 0));
    digits[0][max_number_digits - 1] = 8;
    digits[1][max_number_digits - 1] = 7;

    auto power2 = BigInteger(1);
    auto power7 = BigInteger(1);
    auto x0 = BigInteger(4);
    auto y0 = BigInteger(1);
    auto x1 = BigInteger(3);
    auto y1 = BigInteger(1);

    for (size_t digit_index = max_number_digits - 2; digit_index < max_number_digits; --digit_index) {
        digits[0][digit_index] = ((x0 % base) * (y0 % base) * (base - 1)) % base;
        digits[1][digit_index] = ((x1 % base) * (y1 % base)) % base;

        power2 *= 2;
        power7 *= 7;
        x0 = (x0 + power7 * digits[0][digit_index]) / 2;
        y0 = (y0 + power2 * digits[0][digit_index]) / 7;
        x1 = (x1 + power7 * digits[1][digit_index]) / 2;
        y1 = (y1 + power2 * digits[1][digit_index]) / 7;
    }

    uint64_t answer_base10 = 1;
    for (auto& solution_class: digits) {
        uint64_t zeros_count = 0;
        uint64_t total_count = 0;
        for (auto& digit: solution_class) {
            if (digit == 0) {
                ++zeros_count;
            }
            ++total_count;
            answer_base10 += digit * (total_count - zeros_count);
        }
    }

    std::string answer = convertFromBase10(answer_base10,
                                           static_cast<uint64_t>(base));

    std::cout << answer << '\n';

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time difference = " <<
        static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()) / 1000.0 <<
        "[s]" << '\n';

    return 0;
}