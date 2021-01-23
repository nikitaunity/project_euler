#include <iostream>
#include <cstdlib>
#include <chrono>


int main(int argc, char** argv) {
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    if (argc != 2) {
        std::cout << "Please provide only 1 parameter (number of triangles)...\n";
        return 1;
    }

    uint64_t answer = 0;
    int number_of_triangles = std::atoi(argv[1]);

    uint64_t x = 2;
    uint64_t L = 1;

    for (int t = 0; t < number_of_triangles; ++t) {
        uint64_t new_x = 9 * x + 20 * L;
        uint64_t new_L = 4 * x + 9 * L;

        answer += new_L;

        x = new_x;
        L = new_L;
    }

    std::cout << answer << '\n';

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << std::fixed << "Time difference = " <<
        static_cast<int>(std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()) <<
        "[μs]" << '\n';

    return 0;
}