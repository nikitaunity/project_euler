#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <queue>
#include <functional>


std::vector<bool> generateSieve(const size_t& max_value) {
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
        std::cout << "Please provide only 1 parameter (N -- the min value of remainder)...\n";
        return 1;
    }

    uint64_t min_remainder = static_cast<uint64_t>(std::atoll(argv[1]));

    auto sieve = generateSieve(static_cast<size_t>(1e7));
    std::vector<uint64_t> prime_numbers = generatePrimeNumbers(sieve);

    bool is_found = false;
    size_t index = 1;
    uint64_t answer = 0;

    while (!is_found && index < prime_numbers.size()) {
        uint64_t prime = prime_numbers[index - 1];
        uint64_t remainder = ((static_cast<uint64_t>(2) * static_cast<uint64_t>(index)) % prime) * prime;

        if (remainder > min_remainder) {
            is_found = true;
            answer = index;
        }

        index += 2;
    }

    if (answer == 0) {
        std::cout << "increase the size of sieve\n";
    } else {
        std::cout << answer << '\n';
    }

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << std::fixed << "Time difference = " <<
        static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()) / 1000000.0 <<
        "[s]" << '\n';

    return 0;
}