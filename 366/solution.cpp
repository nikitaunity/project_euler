#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>


std::vector<uint64_t> generateFibonacchiSequence(uint64_t max_value) {
    std::vector<uint64_t> fibonacchi_numbers = {1, 2};
    uint64_t last_two_sum = 3;
    size_t count = 2;

    while (last_two_sum <= max_value) {
        fibonacchi_numbers.push_back(last_two_sum);
        ++count;
        last_two_sum = fibonacchi_numbers[count - 2] + fibonacchi_numbers[count - 1];
    }
    fibonacchi_numbers.push_back(last_two_sum);
    return fibonacchi_numbers;
}


uint64_t combineProgressions(
    const std::vector<std::pair<uint64_t, uint64_t>>& progressions,
    uint64_t modulo, uint64_t required_number) {

    uint64_t count = 0;
    bool to_stop = false;
    uint64_t progressions_sum = 0;

    for (int index = static_cast<int>(progressions.size()) - 1; (index >= 0) && (!to_stop); --index) {

        uint64_t starting_number = progressions[index].first;
        uint64_t progression_length = progressions[index].second;
        if (progression_length + count >= required_number) {
            progression_length = required_number - count;
            to_stop = true;
        }

        progressions_sum += (progression_length % modulo) * (starting_number % modulo);
        progressions_sum %= modulo;

        if (progression_length > 1 && (progression_length & 1) == 0) {
            progressions_sum += ((progression_length >> 1) % modulo) * ((progression_length - 1) % modulo);
        } else {
            progressions_sum += (progression_length % modulo) * (((progression_length - 1) >> 1) % modulo);
        }

        progressions_sum %= modulo;

        count += progression_length;
    }

    return progressions_sum;
}


int main(int argc, char** argv) {
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    if (argc != 2) {
        std::cout << "Please provide only 1 parameter (max N)...\n";
        return 1;
    }

    uint64_t max_value = static_cast<uint64_t>(std::atoll(argv[1]));
    uint64_t answer = 0;
    constexpr uint64_t modulo = 100000000;

    std::vector<std::vector<std::pair<uint64_t, uint64_t>>> progressions(2);

    auto fibonacci_sequence = generateFibonacchiSequence(max_value);

    for (size_t index = 2; index + 1 < fibonacci_sequence.size(); ++index) {

        uint64_t max_stones_for_easy_win = (3 * fibonacci_sequence[index] - 1) >> 1;
        uint64_t required_from_progressions = 0;

        if (max_stones_for_easy_win > max_value) {
            max_stones_for_easy_win = max_value;
        } else {
            required_from_progressions = std::min(max_value + 1, fibonacci_sequence[index + 1])
                                         - (max_stones_for_easy_win + 1);
        }


        uint64_t k = max_stones_for_easy_win - fibonacci_sequence[index];
        if ((k & 1) == 0) {
            answer += ((k >> 1) % modulo) * ((k + 1) % modulo);
        } else {
            answer += (k  % modulo) * (((k + 1) >> 1)  % modulo);
        }
        answer %= modulo;

        answer += combineProgressions(progressions[index % 2], modulo, required_from_progressions);
        answer %= modulo;

        if (index + 3 < fibonacci_sequence.size()) {
            uint64_t next_max_stones_for_easy_win = (3 * fibonacci_sequence[index + 2] - 1) >> 1;
            uint64_t required_number_next_time = fibonacci_sequence[index + 3] - next_max_stones_for_easy_win - 1;

            uint64_t proogression_length = required_number_next_time - required_from_progressions;
            uint64_t progression_starting_number = k + 1 - proogression_length;

            progressions[index % 2].push_back(std::make_pair(progression_starting_number, proogression_length));
        }

    }

    std::cout << answer << '\n';

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << std::fixed << "Time difference = " <<
        static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()) / 1000000.0 <<
        "[s]" << '\n';

    return 0;
}