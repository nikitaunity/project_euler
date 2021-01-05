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

    if ((number & 1) == 1 && sieve[number >> 1]) {
        prime_factors.push_back(number);
        return prime_factors;
    }

    for (int current_prime_index = 0; number > 1; ++current_prime_index) {
        uint64_t prime_number = prime_numbers[current_prime_index];

        if (number % prime_number == 0) {
            while (number % prime_number == 0) {
                number /= prime_number;
            }
            prime_factors.push_back(prime_number);
        }
    }

    return prime_factors;
}


bool isPrimitiveRootByModulo(
    uint64_t candidate,
    uint64_t modulo,
    uint64_t prime,
    const std::vector<bool>& sieve,
    const std::vector<uint64_t>& prime_numbers) {

    uint64_t phi = modulo / prime * (prime - 1);

    auto required_powers = returnDistinctPrimeFactors(phi,
                                                      sieve, prime_numbers);

    for (size_t index = 0; index < required_powers.size(); ++index) {
        required_powers[index] = phi / required_powers[index];
    }

    bool is_primitive_root = true;


    for (const auto& power: required_powers) {
        if (calculatePowerByModulo(candidate, power, modulo) == 1) {
            is_primitive_root = false;
            break;
        }
    }

    return is_primitive_root;
}



int main(int argc, char** argv) {

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    if (argc != 2) {
        std::cout << "Please only provide only 1 parameter (N > 100)...\n";
        return 1;
    }

    uint64_t min_number = static_cast<uint64_t>(std::atoll(argv[1]));
    constexpr uint64_t base = 10;

    auto sieve = generateSieve(2 * min_number);
    auto prime_numbers = generatePrimeNumbers(sieve);

    uint64_t answer = min_number - 1;
    bool is_found = false;

    while (!is_found) {

        if (answer > min_number && answer % 10 == 3) {
            answer += 4;
        } else {
            answer += 2;
        }

        auto prime_factors = returnDistinctPrimeFactors(answer, sieve, prime_numbers);
        int distinct_prime_factors_count = static_cast<int>(prime_factors.size());

        if (distinct_prime_factors_count == 1 ||
           (distinct_prime_factors_count == 2 && prime_factors[0] == 3 && prime_factors[1] % 3 == 2)) {

            uint64_t modulo = answer;
            while (modulo % 3 == 0) {
                modulo /= 3;
            }

            if (isPrimitiveRootByModulo(base, modulo,
                                        prime_factors[distinct_prime_factors_count - 1],
                                        sieve, prime_numbers)) {

                is_found = true;
            }

        }

    }

    std::cout << answer << '\n';

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time difference = " <<
        static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()) / 1000.0 <<
        "[s]" << '\n';

    return 0;
}