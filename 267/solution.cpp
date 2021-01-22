#include <iostream>
#include <cmath>
#include <cstdlib>
#include <chrono>
#include <vector>
#include <algorithm>
#include <iomanip>

constexpr double EPS = 1e-7;


inline double evaluateFormula(const double& point) {
    return 2 * (1 - point) * std::log(1 - point) +
        (1 + 2 * point) * std::log(1 + 2 * point);
}


double findExtremum(const double& target_value) {

    double lower_bound = 0.0;
    double upper_bound = 1.0;

    while (upper_bound - lower_bound >= EPS) {
        auto middle_point = (lower_bound + upper_bound) / 2;
        auto value_at_middle = evaluateFormula(middle_point);

        if (value_at_middle > target_value + EPS / 2) {
            upper_bound = middle_point;
        } else if (value_at_middle < target_value - EPS / 2) {
            lower_bound = middle_point;
        } else {
            return middle_point;
        }
    }

    return (upper_bound + lower_bound) / 2;
}



std::vector<std::vector<double>> calculateProbabilities(uint64_t number_of_runs) {
    std::vector<std::vector<double>> probabilities(number_of_runs);

    probabilities[0] = {0.5, 0.5};

    for (uint64_t run = 1; run < number_of_runs; ++run) {
        probabilities[run].reserve(run + 2);

        probabilities[run].emplace_back(probabilities[run - 1][0] / 2);

        for (uint64_t success_number = 1; success_number <= run; ++success_number) {
            probabilities[run].emplace_back(
                (probabilities[run - 1][success_number - 1]
                    + probabilities[run - 1][success_number]) / 2
            );
        }

        probabilities[run].emplace_back(probabilities[run - 1][run] / 2);
    }

    return probabilities;
}


double calculateProbability(uint64_t number_of_runs, uint64_t min_number_of_success) {

    auto binomial_probabilities = calculateProbabilities(number_of_runs);
    auto last_row = binomial_probabilities[number_of_runs - 1];

    uint64_t x = (number_of_runs + 1) / 2;
    double p = 0.5;
    if (number_of_runs % 2 == 0) {
        p = (1 + last_row[x]) / 2;
    }

    while (x != min_number_of_success) {
        if (min_number_of_success < x) {
            p += last_row[--x];
        } else {
            p -= last_row[x++];
        }
    }

    return p;
}


int main(int argc, char* argv[]) {

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    if (argc != 3) {
        std::cout << "Please provide 2 parameters (desired sum and number of turns)...\n";
        return 1;
    }

    uint64_t target_sum = std::atoll(argv[1]);
    uint64_t number_of_turns = std::atoll(argv[2]);


    double target_logarithm = std::log(target_sum);
    if (target_logarithm > std::log(2) * number_of_turns + EPS) {
        std::cout << "Booooy, you want to much... \n";
        return 1;
    }


    auto extremum_point = findExtremum(3 * target_logarithm / number_of_turns);
    uint64_t min_number_of_heads = static_cast<uint64_t>(
        std::ceil(
            (target_logarithm - number_of_turns * std::log(1 - extremum_point)) /
            (std::log(1 + 2 * extremum_point) - std::log(1 - extremum_point))
        )
    );

    std::cout << std::setprecision(12)
        << calculateProbability(number_of_turns, min_number_of_heads) << '\n';

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        std::cout << "Time difference = " <<
        static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()) / 1000.0 <<
        "[s]" << '\n';

    return 0;
}