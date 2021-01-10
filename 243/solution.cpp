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

    if (argc != 3) {
        std::cout << "Please provide only 2 parameters...\n";
        return 1;
    }

    uint64_t numerator = static_cast<uint64_t>(std::atoll(argv[1]));
    uint64_t denominator = static_cast<uint64_t>(std::atoll(argv[2]));

    if (numerator > denominator) {
        std::cout << "1\n";
        return 0;
    }

    constexpr int N = 100;
    auto sieve = generateSieve(N);
    std::vector<uint64_t> prime_numbers = generatePrimeNumbers(sieve);

    uint64_t current_numerator = 1;
    uint64_t current_denominator = 1;
    uint64_t index = 0;

    while (current_numerator * denominator >= (current_denominator - 1) * numerator) {
        current_numerator *= (prime_numbers[index] - 1);
        current_denominator *= prime_numbers[index];
        ++index;
    }

    --index;
    current_denominator /= prime_numbers[index];
    current_numerator /= (prime_numbers[index] - 1);

    uint64_t answer = 0;

    for (uint64_t multiplier = 2; multiplier <= prime_numbers[index]; ++multiplier) {
        uint64_t phi_value = current_numerator * multiplier;
        answer = current_denominator * multiplier;

        if (phi_value * denominator < (answer - 1) * numerator) {
            break;
        }
    }

    std::cout << answer << '\n';

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << std::fixed << "Time difference = " <<
        static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()) / 1000000.0 <<
        "[s]" << '\n';

    return 0;
}