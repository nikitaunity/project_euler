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
        std::cout << "Please provide only 1 parameter (N -- the value of power)...\n";
        return 1;
    }

    uint64_t required_power = static_cast<uint64_t>(std::atoll(argv[1]));

    auto sieve = generateSieve(static_cast<size_t>(1e7));
    std::vector<uint64_t> prime_numbers = generatePrimeNumbers(sieve);
    constexpr uint64_t modulo = 500500507;

    uint64_t answer = 1;
    size_t current_index = 0;

    std::priority_queue<uint64_t, std::vector<uint64_t>, std::greater<uint64_t> > candidates;

    for (uint64_t power = 0; power < required_power; ++power) {

        if (candidates.empty() || candidates.top() > prime_numbers[current_index]) {

            answer *= prime_numbers[current_index];
            candidates.push(prime_numbers[current_index] * prime_numbers[current_index]);
            answer %= modulo;
            ++current_index;

        } else {

            uint64_t min_value = candidates.top();
            candidates.pop();
            candidates.push(min_value * min_value);
            answer *= min_value;
            answer %= modulo;

        }
    }

    std::cout << answer << '\n';

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << std::fixed << "Time difference = " <<
        static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()) / 1000000.0 <<
        "[s]" << '\n';

    return 0;
}