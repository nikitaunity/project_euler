#include <iostream>
#include <cstdlib>
#include <chrono>


int main(int argc, char** argv) {
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    if (argc != 3) {
        std::cout << "Please provide only 2 parameters (numerator and denominator)...\n";
        return 1;
    }

    uint64_t numerator = static_cast<uint64_t>(std::atoll(argv[1]));
    uint64_t denominator = static_cast<uint64_t>(std::atoll(argv[2]));

    if (numerator > denominator) {
       std::cout << "2\n";
       return 0;
    } else if (numerator == denominator) {
       std::cout << "6\n";
       return 0;
    }

    uint64_t number_of_perfect_solutions = 1;
    uint64_t max_number_of_solutions_given = 2;

    while (number_of_perfect_solutions * denominator >=
        numerator * max_number_of_solutions_given) {

        ++number_of_perfect_solutions;
        ++max_number_of_solutions_given;
        max_number_of_solutions_given <<= 1;

    }

    uint64_t min_number_of_solutions = (number_of_perfect_solutions * denominator + 1) / numerator;
    min_number_of_solutions <<= 1;
    ++min_number_of_solutions;

    uint64_t discriminant = min_number_of_solutions * min_number_of_solutions;
    uint64_t answer = (discriminant - 1) >> 2;

    std::cout << answer << '\n';

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << std::fixed << "Time difference = " <<
        static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()) / 1000000.0 <<
        "[s]" << '\n';

    return 0;
}