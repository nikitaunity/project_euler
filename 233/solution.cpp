#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <cmath>
#include <algorithm>


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


std::vector<std::vector<uint64_t>> generateOddPrimeNumbers(
    const std::vector<bool>& sieve) {

    std::vector<std::vector<uint64_t> > prime_numbers(2);

    for (size_t index = 0; index < sieve.size(); ++index) {
        if (sieve[index]) {
            prime_numbers[index & 1].push_back((index << 1) + 1);
        }
    }

    return prime_numbers;
}

uint64_t calculatePower(uint64_t base, int power) {
    uint64_t exponent = 1;
    while (power) {
        if (power & 1) {
            exponent *= base;
            --power;
        } else {
            base *= base;
            power >>= 1;
        }
    }
    return exponent;
}


void collectBaseSolutions(const std::vector<uint64_t>& primes_mod_4_1,
                          const std::vector<int>& powers,
                          std::vector<uint64_t>* taken_numbers,
                          std::vector<uint64_t>* base_solutions,
                          uint64_t max_value,
                          uint64_t accured_product, int accured_count) {

    if (static_cast<size_t>(accured_count) == powers.size()) {
        (*base_solutions).push_back(accured_product);
        return;
    }

    for (auto prime: primes_mod_4_1) {
        bool is_taken = false;

        for (int i = 0; i < accured_count; ++i) {
            if ((*taken_numbers)[i] % prime == 0) {
                is_taken = true;
                break;
            }
        }

        if (!is_taken) {
            auto taken_number = calculatePower(prime, powers[accured_count]);
            if (accured_product * taken_number <= max_value) {
                (*taken_numbers)[accured_count] = taken_number;
                collectBaseSolutions(primes_mod_4_1, powers,
                                     taken_numbers, base_solutions,
                                     max_value,
                                     accured_product * taken_number,
                                     accured_count + 1);
            } else {
                break;
            }

        }

    }

    return;

}


std::vector<uint64_t> buildMultiples(uint64_t max_multiple,
                                     const std::vector<uint64_t>& primes_mod_4_3,
                                     const std::vector<bool>& sieve) {

    std::vector<uint64_t> multiples;

    for (uint64_t number = 1;  number <= max_multiple; number += 2) {

        if (number % 5 == 0 || number % 13 == 0) {
            continue;
        }

        bool is_multiple = true;
        uint64_t candidate = number;

        for (auto prime: primes_mod_4_3) {

            if (prime > candidate) {
                if (candidate > 1) {
                    is_multiple = false;
                }
                break;
            }

            if (sieve[candidate >> 1]) {
                if ((candidate & 3) == 1) {
                    is_multiple = false;
                }
                break;
            }

            if (candidate % prime == 0) {

                while (candidate % prime == 0) {
                    candidate /= prime;
                }

            }
        }

        if (is_multiple) {
            multiples.push_back(number);
        }

    }

    size_t odd_multiple_numbers_count = multiples.size();
    for (size_t index = 0; index < odd_multiple_numbers_count; ++index) {

        uint64_t multiple = multiples[index] << 1;
        while (multiple <= max_multiple) {
            multiples.push_back(multiple);
            multiple <<= 1;
        }
    }

    std::sort(multiples.begin(), multiples.end());

    return multiples;
}


std::vector<uint64_t> buildPartialSums(const std::vector<uint64_t>& array) {
    size_t array_size = array.size();
    std::vector<uint64_t> partial_sums(array_size + 1);

    partial_sums[0] = 0;
    for (size_t index = 0; index < array_size; ++index) {
        partial_sums[index + 1] = partial_sums[index] + array[index];
    }

    return partial_sums;
}


int main(int argc, char** argv) {
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    if (argc != 2) {
        std::cout << "Please only provide only 1 parameter (max N), number of solutions fixed (420)...\n";
        return 1;
    }

    uint64_t max_number = static_cast<uint64_t>(std::atoll(argv[1]));
    auto sieve = generateSieve(static_cast<size_t>(1e8));
    auto prime_numbers = generateOddPrimeNumbers(sieve);

    std::vector<std::vector<int> > powers = {{10, 2}, {7, 3}, {3, 2, 1}};
    // 420 = 4 * 105, 105 = 3 * 5 * 7 = 15 * 7 = 35 * 3 = 21 * 5

    std::vector<uint64_t> base_solutions;

    for (auto power: powers) {
        std::vector<uint64_t> taken_numbers(power.size());
        collectBaseSolutions(prime_numbers[0], power,
                            &taken_numbers, &base_solutions,
                            max_number, 1, 0);
    }

    uint64_t max_multiple = 0;
    for (auto base_solution: base_solutions) {
        max_multiple = std::max(max_multiple,
                                max_number / base_solution);
    }

    auto multiples = buildMultiples(max_multiple,
                                    prime_numbers[1],
                                    sieve);

    auto multiples_partial_sums = buildPartialSums(multiples);

    uint64_t answer = 0;
    for (auto base_solution: base_solutions) {
        uint64_t remainder = max_number / base_solution;
        size_t required_index = std::upper_bound(multiples.begin(),
                                                 multiples.end(),
                                                 remainder)
            - multiples.begin();

        answer += base_solution * multiples_partial_sums[required_index];
    }


    std::cout << answer << '\n';

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time difference = " <<
        static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()) / 1000.0 <<
        "[s]" << '\n';

    return 0;
}