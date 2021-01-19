#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <vector>

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


int main(int argc, char** argv) {
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    if (argc != 3) {
        std::cout << "Please provide only 2 parameters k and N, k should divide N...\n";
        return 1;
    }

    uint64_t window_size = static_cast<uint64_t>(std::atoll(argv[1]));
    uint64_t table_size = static_cast<uint64_t>(std::atoll(argv[2]));
    constexpr uint64_t MODULO = static_cast<uint64_t>(1e9 + 7);

    if (table_size % window_size != 0) {
        std::cout << "k should divide N...\n";
        return 1;
    }

    uint64_t ratio = table_size / window_size;

    uint64_t multiplier_of_power_2 = calculatePowerByModulo(2, ratio << 1, MODULO);

    uint64_t number_of_parts = (window_size + 2) >> 1;
    std::vector<uint64_t> powers_of_2(number_of_parts);
    powers_of_2[0] = ((window_size & 1) == 1) ? calculatePowerByModulo(2, ratio, MODULO) : 1;

    for (size_t index = 1; index < number_of_parts; ++index) {
        powers_of_2[index] = (powers_of_2[index - 1] * multiplier_of_power_2) % MODULO;
    }

    std::vector<uint64_t> residuals(window_size + 1);
    residuals[1] = 1;
    for (uint64_t n = 2; n <= window_size; ++n) {
        residuals[n] = (MODULO / n) * residuals[MODULO % n] % MODULO;
    }

    uint64_t answer = powers_of_2[number_of_parts - 1];
    uint64_t current_number = 1;
    uint64_t k = window_size;

    for (size_t index = 1; index < number_of_parts; ++index) {

        current_number *= k;
        current_number %= MODULO;

        current_number *= k - 1;
        current_number %= MODULO;

        k -= 2;

        current_number *= (residuals[index] * residuals[index]) % MODULO;
        current_number %= MODULO;

        answer += (current_number * powers_of_2[number_of_parts - index - 1]) % MODULO;
        answer %= MODULO;

    }

    std::cout << answer << '\n';

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << std::fixed << "Time difference = " <<
        static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()) / 1000000.0 <<
        "[s]" << '\n';

    return 0;
}