#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <cmath>
#include <algorithm>


uint64_t mulmod(uint64_t a, uint64_t b, uint64_t m) {
    uint64_t res = 0;
    uint64_t temp_b;

    /* Only needed if b may be >= m */
    if (b >= m) {
        if (m > UINT64_MAX / 2u)
            b -= m;
        else
            b %= m;
    }

    while (a != 0) {
        if (a & 1) {
            /* Add b to res, modulo m, without overflow */
            if (b >= m - res) /* Equiv to if (res + b >= m), without overflow */
                res -= m;
            res += b;
        }
        a >>= 1;

        /* Double b, modulo m */
        temp_b = b;
        if (b >= m - b)       /* Equiv to if (2 * b >= m), without overflow */
            temp_b -= m;
        b += temp_b;
    }
    return res;
}


uint64_t calculatePowerByModulo(uint64_t base, uint64_t power,
                               const uint64_t& modulo) {
    uint64_t exponent = 1;
    while (power) {
        if (power & 1) {
            exponent = mulmod(exponent, base, modulo);
            --power;
        } else {
            base =  mulmod(base, base, modulo);
            power >>= 1;
        }
    }
    return exponent;
}


uint64_t calculateCubicNonTrivialRoot(const uint64_t& prime_number) {

    for (uint64_t candidate = 2; candidate < prime_number; ++candidate) {

        uint64_t root = calculatePowerByModulo(candidate, (prime_number - 1) / 3, prime_number);
        if (root != 1) {
            return root;
        }

    }

    return 0;
}


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
        std::cout << "Please only provide only 1 parameter (max k)...\n";
        return 1;
    }

    uint64_t max_number = static_cast<uint64_t>(std::atoll(argv[1]));
    std::vector<std::vector<uint64_t> > factorization_table(max_number + 1);

    for (uint64_t index = 0; index <= max_number; ++index) {
        factorization_table[index].reserve(12);
    }

    auto sieve = generateSieve(max_number + 1);
    std::vector<uint64_t> prime_numbers = generatePrimeNumbers(sieve);

    for (auto prime: prime_numbers) {

        for (auto cubic_root = prime - 1; cubic_root <= max_number; cubic_root += prime) {
            factorization_table[cubic_root].emplace_back(prime);
        }

        if (prime % 3 == 1) {
            uint64_t non_trivial_solution = calculateCubicNonTrivialRoot(prime);
            uint64_t first_solution = prime - non_trivial_solution;
            uint64_t second_solution = prime - calculatePowerByModulo(non_trivial_solution, 2, prime);

            for (auto solution: {first_solution, second_solution}) {
                for (auto cubic_root = solution; cubic_root <= max_number; cubic_root += prime) {
                    factorization_table[cubic_root].emplace_back(prime);
                }
            }
        }
    }

    uint64_t answer = 0;

    for (uint64_t number = 1; number <= max_number; ++number) {

        uint64_t cube = number * number * number + 1;

        for (auto factor: factorization_table[number]) {
            while (cube % factor == 0) {
                cube /= factor;
            }
        }

        auto number_of_factors =  factorization_table[number].size();

        uint64_t biggest_factor = factorization_table[number][number_of_factors - 1];

        answer += std::max(biggest_factor, cube) - 1;

    }

    std::cout << answer << '\n';

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time difference = " <<
        static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()) / 1000.0 <<
        "[s]" << '\n';

    return 0;
}