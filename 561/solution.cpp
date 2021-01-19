#include <iostream>
#include <cstdlib>
#include <chrono>


uint64_t calculateSumOfOrd(uint64_t number) {

    uint64_t answer = 0;
    uint64_t power = 1;

    while (number > 0) {
        answer += (number & 1) * (power - 1);
        power <<= 1;
        number >>= 1;
    }

    return answer;
}


int main(int argc, char** argv) {
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    if (argc != 3) {
        std::cout << "Please provide only 2 parameters (m and n)...\n";
        return 1;
    }

    uint64_t number_of_primes = static_cast<uint64_t>(std::atoll(argv[1]));
    uint64_t max_power_of_product = static_cast<uint64_t>(std::atoll(argv[2]));

    uint64_t k = max_power_of_product >> 2;
    uint64_t answer = (number_of_primes + 1) * (k + calculateSumOfOrd(k));

    if ((max_power_of_product & 3) == 3) {
        ++k;
        uint64_t ord_k = 0;
        while ((k & 1) == 0) {
            k /= 2;
            ++ord_k;
        }

        answer += number_of_primes * (ord_k + 1);
    }

    std::cout << answer << '\n';

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << std::fixed << "Time difference = " <<
        static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()) / 1000000.0 <<
        "[s]" << '\n';

    return 0;
}