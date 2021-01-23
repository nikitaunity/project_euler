#include <iostream>
#include <cstdlib>
#include <chrono>
#include <algorithm>


uint64_t calculateGCD(const uint64_t& first, const uint64_t& second) {
    if (second == 0) {
        return first;
    }

    return calculateGCD(second, first % second);
}


inline uint64_t calculateLCM(const uint64_t& first, const uint64_t& second) {
    return first / calculateGCD(first, second) * second;
}



int main(int argc, char* argv[]) {

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    if (argc != 3) {
        std::cout << "Please provide 2 parameters " <<
            "(max desired strak length and base)...\n";
        return 1;
    }

    uint64_t max_streak_length = std::atoll(argv[1]);
    uint64_t base = std::atoll(argv[2]);

    uint64_t current_lcm = 1;
    uint64_t next_lcm = 1;
    uint64_t max_number = 1;
    uint64_t answer = 0;

    for (uint64_t streak_length = 1; streak_length <= max_streak_length; ++streak_length) {

        current_lcm = next_lcm;
        next_lcm = calculateLCM(next_lcm, streak_length + 1);
        max_number *= base;

        uint64_t base_solutions_count = next_lcm / current_lcm - 1;
        //std::cout << base_solutions_count << '\n';

        uint64_t quotient = (max_number - 1) / next_lcm;
        uint64_t remainder = (max_number - 1) % next_lcm;

        answer += quotient * base_solutions_count;
        answer += (std::max(1ull, remainder) - 1) / current_lcm;

    }

    std::cout << answer << '\n';


    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        std::cout << "Time difference = " <<
        static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()) <<
        "[μs]" << '\n';

    return 0;
}