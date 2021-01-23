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


int main(int argc, char** argv) {
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    if (argc != 3) {
        std::cout << "Please provide only 2 parameters (k for N = 10^k) and (number of primes)...\n";
        return 1;
    }

    uint64_t power = static_cast<uint64_t>(std::atoll(argv[1]));
    uint64_t number_of_primes = static_cast<uint64_t>(std::atoll(argv[2]));

    uint64_t primes_acquired = 0;
    uint64_t index = 3;
    uint64_t answer = 0;

    auto sieve = generateSieve(static_cast<size_t>(1e6));
    auto prime_numbers = generatePrimeNumbers(sieve);

    while (primes_acquired < number_of_primes && index < prime_numbers.size()) {

        uint64_t ord2 = 0;
        uint64_t ord5 = 0;

        uint64_t prime = prime_numbers[index];
        uint64_t p = prime - 1;

        while (p % 2 == 0) {
            p /= 2;
            ++ord2;
        }

        while (p % 5 == 0) {
            p /= 5;
            ++ord5;
        }

        if (ord2 > power) {
            ord2 = power;
        }

        if (ord5 > power) {
            ord5 = power;
        }


        uint64_t base_remainder = 10;

        for (uint64_t x = 0; x <= ord5; ++x) {

            uint64_t remainder = base_remainder;

            for (uint64_t y = 0; y <= ord2 && remainder > 1; ++y) {
                remainder = calculatePowerByModulo(remainder, 2, prime);
            }

            if (remainder == 1) {
                ++primes_acquired;
                answer += prime;
                break;
            }

            base_remainder =  calculatePowerByModulo(base_remainder, 5, prime);
        }

        ++index;
    }

    if (primes_acquired < number_of_primes) {
        std::cout << "oops, I need to generate more prime numbers...\n";
        return 1;
    }

    std::cout << answer << '\n';

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << std::fixed << "Time difference = " <<
        static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()) / 1000000.0 <<
        "[s]" << '\n';

    return 0;
}