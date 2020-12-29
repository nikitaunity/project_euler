#include <iostream>
#include <unordered_set>
#include <chrono>

constexpr int MAX_LIMIT = 28123;

bool isAbundantNumber(const int& number) {

    if (number > 6 && number % 6 == 0) {
        return true;
    }

    int divisor_sum = 1;
    for (int current_divisor = 2; current_divisor * current_divisor <= number; ++current_divisor) {
        if (number % current_divisor == 0) {

            int other_divisor = number / current_divisor;
            divisor_sum += current_divisor;

            if (other_divisor != current_divisor) {
                divisor_sum += other_divisor;
            }

            if (divisor_sum > number) {
                return true;
            }
        }
    }

    return false;
}


int calculateSumOfAllAbundantNumbers() {

    std::unordered_set<int> abundant_numbers;
    int answer = 0;

    for (int number = 1; number <= MAX_LIMIT; ++number) {

        bool is_found = false;

        for (const auto& abundant_number: abundant_numbers) {
            if (abundant_numbers.find(number - abundant_number) != abundant_numbers.end()) {
                is_found = true;
                break;
            }
        }

        if (!is_found) {
            answer += number;
        }

        if (isAbundantNumber(number)) {
            abundant_numbers.insert(number);
        }
    }

    return answer;
}

int main() {

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    std::cout << calculateSumOfAllAbundantNumbers() << '\n';

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        std::cout << "Time difference = " <<
        static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()) / 1000.0 <<
        "[s]" << '\n';

    return 0;
}