#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>

std::vector<bool> generateSieve(size_t max_value) {
    size_t required_size = (max_value + 1) >> 1;
    std::vector<bool> sieve(required_size, true);
    sieve[0] = false;  // 1 is not prime

    size_t current_index = 1;
    while ((current_index << 1) * (current_index + 1) < required_size) {
        if (sieve[current_index]) {
            auto prime_number = (current_index << 1) +  1;

            for (auto prime_multiplicator = current_index + prime_number;
                prime_multiplicator < required_size;
                prime_multiplicator += prime_number) {
                    sieve[prime_multiplicator] = false;
                }

        }
        current_index += 1;
    }

    return sieve;
}

std::vector<uint64_t> generatePrimeNumbers(const std::vector<bool>& sieve) {

    std::vector<uint64_t> prime_numbers = {2};

    for (uint64_t index = 0; index < sieve.size(); ++index) {
        if (sieve[index]) {
            prime_numbers.push_back((index << 1) + 1);
        }
    }

    return prime_numbers;
}

int main(int argc, char** argv) {
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    if (argc != 2) {
        std::cout << "Please provide only 1 parameter (max N)...\n";
        return 1;
    }

    uint64_t max_value = static_cast<uint64_t>(std::atoll(argv[1]));

    if (max_value < 4) {
        std::cout << "0\n";
        return 0;
    }

    auto sieve = generateSieve(max_value / 2);
    std::vector<uint64_t> prime_numbers = generatePrimeNumbers(sieve);

    size_t opposite_index = prime_numbers.size() - 1;
    uint64_t answer = 0;

    for (size_t index = 0;
        prime_numbers[index] * prime_numbers[index] < max_value; ++index) {

        while (prime_numbers[opposite_index] * prime_numbers[index] >= max_value) {
            --opposite_index;
        }

        answer += (opposite_index - index + 1);
    }

    std::cout << answer << '\n';

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << std::fixed << "Time difference = " <<
        static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()) / 1000000.0 <<
        "[s]" << '\n';

    return 0;
}