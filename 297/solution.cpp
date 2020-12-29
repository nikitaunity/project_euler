#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>

std::vector<uint64_t> generateFibonacchiSequence(uint64_t max_value) {
    std::vector<uint64_t> fibonacchi_numbers = {1, 2};
    uint64_t last_two_sum = 3;
    size_t count = 2;

    while (last_two_sum < max_value) {
        fibonacchi_numbers.push_back(last_two_sum);
        ++count;
        last_two_sum = fibonacchi_numbers[count - 2] + fibonacchi_numbers[count - 1];
    }

    return fibonacchi_numbers;
}


std::vector<std::vector<uint64_t> > calculateNumberOfSequenceWithFixedFibonacciNumbers(
    size_t max_length) {

    std::vector<std::vector<uint64_t>> number_of_sequence(
        ((max_length + 1) >> 1) + 1, std::vector<uint64_t>(max_length, 0));

    for (size_t i = 0; i < max_length; ++i) {
        number_of_sequence[0][i] = 1;
    }

    for (size_t ones_number = 1; ones_number <= ((max_length + 1) >> 1); ++ones_number) {
        size_t min_length = (ones_number << 1) - 1;
        number_of_sequence[ones_number][min_length] = 1;

        for (size_t length = min_length + 1; length < max_length; ++length) {
            number_of_sequence[ones_number][length] =
                number_of_sequence[ones_number - 1][length - 2] + number_of_sequence[ones_number][length - 1];
        }
    }

    return number_of_sequence;
}


uint64_t calculateNumberOfOnesWithFixedLength(
    const std::vector<std::vector<uint64_t> >& number_of_sequence,
    const std::vector<uint64_t>& fibonacci_sequence,
    size_t length, size_t number_of_ones_defined) {

    uint64_t answer = 0;

    for (size_t ones_number = 0; ones_number <= ((length + 1) >> 1); ++ones_number) {
        answer += number_of_sequence[ones_number][length] * ones_number;
    }

    answer += number_of_ones_defined * fibonacci_sequence[length];
    return answer;
}


int main(int argc, char** argv) {
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    if (argc != 2) {
        std::cout << "Please provide only 1 parameter (max N)...\n";
        return 1;
    }

    uint64_t max_value = static_cast<uint64_t>(std::atoll(argv[1]));

    auto fibonacci_sequence = generateFibonacchiSequence(max_value);
    auto number_of_sequences = calculateNumberOfSequenceWithFixedFibonacciNumbers(fibonacci_sequence.size());

    uint64_t pos = static_cast<uint64_t>(fibonacci_sequence.size()) - 1;
    uint64_t current_value = max_value;
    uint64_t number_of_ones = 0;

    uint64_t answer = 0;

    while (current_value > 0) {
        if (fibonacci_sequence[pos] <= current_value) {

            answer += calculateNumberOfOnesWithFixedLength(
                number_of_sequences, fibonacci_sequence, pos, number_of_ones);

            current_value -= fibonacci_sequence[pos];
            ++number_of_ones;
            pos -= 2;

        } else {
            --pos;
        }
    }

    std::cout << answer << '\n';


    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << std::fixed << "Time difference = " <<
        static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()) / 1000000.0 <<
        "[s]" << '\n';

    return 0;
}