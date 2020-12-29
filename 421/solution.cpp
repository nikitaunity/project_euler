#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>

uint64_t calculatePowerByModulo(uint64_t base, uint64_t power,
                               const uint64_t& modulo) {
    uint64_t exponent = 1;
    while (power) {
        if (power & 1) {
            exponent *= base;
            exponent %= modulo;
            --power;
        } else {
            base *= base;
            base %= modulo;
            power >>= 1;
        }
    }
    return exponent;
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


std::vector<uint64_t> returnDistinctPrimeFactors(
        uint64_t number,
        const std::vector<bool> &sieve,
        const std::vector<uint64_t>& prime_numbers) {

    std::vector<uint64_t> prime_factors;

    for (int current_prime_index = -1; number > 1; ++current_prime_index) {
        if (current_prime_index >= 0 && sieve[number >> 1]) {
            prime_factors.push_back(number);
            return prime_factors;
        } else {

            uint64_t prime_number = (current_prime_index >= 0)
                ? prime_numbers[current_prime_index]
                : 2;

            if (number % prime_number == 0) {
                while (number % prime_number == 0) {
                    number /= prime_number;
                }
                prime_factors.push_back(prime_number);
            }
        }
    }

    return prime_factors;
}

uint64_t calculateMinimalPrimitiveRootByPrimeModulo(
        const uint64_t& prime_number,
        const std::vector<bool> &sieve,
        const std::vector<uint64_t>& prime_numbers) {

    if (prime_number == 2) {
        return 1;
    }

    auto required_powers = returnDistinctPrimeFactors(prime_number - 1,
                                                      sieve, prime_numbers);

    for (size_t index = 0; index < required_powers.size(); ++index) {
        required_powers[index] = (prime_number - 1) / required_powers[index];
    }

    bool is_found = false;
    for (uint64_t candidate = 2; candidate < prime_number; ++candidate) {
        is_found = true;

        for (const auto& power: required_powers) {
            if (calculatePowerByModulo(candidate, power, prime_number) == 1) {
                is_found = false;
                break;
            }
        }

        if (is_found) {
            return candidate;
        }
    }

    return 0;
}


uint64_t calculateNumberOfDivisibleByPrime(
        const uint64_t& prime_number,
        const uint64_t& max_number,
        const std::vector<bool> &sieve,
        const std::vector<uint64_t>& prime_numbers) {

    uint64_t answer = prime_number * ((max_number + 1) / prime_number);

    if ((prime_number < 7) || (prime_number % 3 != 1 && prime_number % 5 != 1)) {
        return answer;
    }

    uint64_t primitive_root = calculateMinimalPrimitiveRootByPrimeModulo(
        prime_number, sieve, prime_numbers);

    uint64_t roots = 15;
    if (prime_number % 3 == 1) {
        if (prime_number % 5 == 1) {
            roots = 15;
        } else {
            roots = 3;
        }
    } else {
        roots = 5;
    }

    uint64_t required_power = (prime_number - 1) / roots;
    auto first_residual = calculatePowerByModulo(primitive_root,
                                                 required_power,
                                                 prime_number);

    auto current_residual = first_residual;
    for (uint64_t root_number = 0; root_number + 1 < roots; ++root_number) {
        answer += prime_number * ((max_number + current_residual) / prime_number);
        current_residual = (current_residual * first_residual) % prime_number;
    }

    return answer;
}


int main(int argc, char** argv) {
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    if (argc != 3) {
        std::cout << "Please provide 2 parameters N and M (limit for primes)...\n";
        return 1;
    }

    uint64_t max_prime = static_cast<uint64_t>(std::atoll(argv[2]));
    uint64_t max_number = static_cast<uint64_t>(std::atoll(argv[1]));

    auto sieve = generateSieve(static_cast<size_t>(max_prime));
    auto prime_numbers = generatePrimeNumbers(sieve);

    uint64_t answer = 0;
    for (const auto& prime_number: prime_numbers) {
        answer += calculateNumberOfDivisibleByPrime(
            prime_number, max_number, sieve, prime_numbers
        );
    }

    std::cout << answer << '\n';

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time difference = " <<
        static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()) / 1000.0 <<
        "[s]" << '\n';

    return 0;
}