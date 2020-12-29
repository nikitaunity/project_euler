#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <algorithm>

constexpr int PRIME = 9973;

int calculateNaivelyGCD(const int& first, const int& second) {

    if (second == 0) {
        return first;
    }

    return calculateNaivelyGCD(second, first % second);
}


int calculateWithMemorizationGCD(const int& first, const int& second,
                                std::vector<int>* gcds) {

    if (second == 0) {
        return first;
    }

    if (first >= PRIME) {
        return calculateWithMemorizationGCD(second, first % second,
                                            gcds);
    }

    const int key = first * PRIME + second;
    if ((*gcds)[key] != 0) {
        return (*gcds)[key];
    }

    (*gcds)[key] = calculateWithMemorizationGCD(second, first % second,
                                                gcds);

    return (*gcds)[key];
}


std::vector<int> initializeGCDTable(int prime_number) {
    std::vector<int> gcds(prime_number * prime_number, 0);

    for (auto i = 0; i < prime_number; ++i) {
        gcds[i] = gcds[i * prime_number] = i;
        gcds[i * prime_number + 1] = gcds[prime_number + i] = 1;
        gcds[i * prime_number + 2] = gcds[(prime_number << 1) + i] = 2 - (i & 1);
        gcds[i * prime_number + 3] = gcds[3 * prime_number + i]
            = 1 + ((i % 3 == 0) << 1);
    }

    return gcds;
}


int calculateNumberOfGoodTriangles(int max_perimeter, std::vector<int>* gcds) {

    int answer = 1;  // let's take 2 2 2 into answer

    for (int smallest_side = 2; 3 * smallest_side <= max_perimeter; ++smallest_side) {

        int maximum_remainder = std::min(smallest_side,
                                        ((max_perimeter - 3 * smallest_side) >> 1) + 1);

        for (int remainder = 0; remainder < maximum_remainder; ++remainder) {
            const int gcd = calculateWithMemorizationGCD(smallest_side, remainder,
                                                         gcds);

            if (gcd  > 2) {

                int q = ((smallest_side << 1) + remainder) / gcd;
                int q_increment = smallest_side / gcd;

                for (int middle_side = smallest_side + remainder;
                     middle_side <= ((max_perimeter - smallest_side) >> 1);
                     middle_side += smallest_side, q += q_increment) {

                    int min_multiplier = (middle_side + q - 1) / q;
                    int sum_of_two_sides = smallest_side + middle_side;
                    int max_multiplier = std::min(sum_of_two_sides - 1,
                                                  max_perimeter - sum_of_two_sides) / q;

                    if (min_multiplier <= max_multiplier) {
                        answer += max_multiplier - min_multiplier + 1;
                    } else {
                        break;
                    }

                }
            }
        }

    }

    return answer;
}


int main(int argc, char** argv) {

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    if (argc != 2) {
        std::cout << "Please provide only 1 parameter (max perimeter)...\n";
        return 1;
    }

    auto gcds = initializeGCDTable(PRIME);

    int max_perimeter = std::atoi(argv[1]);

    std::cout << calculateNumberOfGoodTriangles(max_perimeter, &gcds) << '\n';

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time difference = " <<
        static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()) / 1000.0 <<
        "[s]" << '\n';

    return 0;
}