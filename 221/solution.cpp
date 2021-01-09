#include <iostream>
#include <cstdlib>
#include <climits>
#include <chrono>
#include <algorithm>
#include <queue>


void addAlexandrians(uint64_t r, uint64_t required_index,
                    std::priority_queue<uint64_t>* alexandrians,
                    uint64_t* size) {

    uint64_t s = r * r + 1;
    uint64_t max_uint64_t = 0 - 1;

    uint64_t divisor = s / (r << 1);

    while (divisor > 0) {

        if (divisor % 3 == 0 || (divisor & 3) == 0 || (divisor & 3) == 3 || divisor % 7 == 0) {
            --divisor;
            continue;
        }

        if (s % divisor == 0) {
            uint64_t a = r * (s / divisor - r);

            if (a < max_uint64_t / (r - divisor)) {
                uint64_t alexandrian = a * (r - divisor);

                if (*size < required_index) {
                    (*alexandrians).push(alexandrian);
                    ++(*size);
                } else if ((*alexandrians).top() > alexandrian) {
                    (*alexandrians).pop();
                    (*alexandrians).push(alexandrian);
                } else {
                    break;
                }

            } else {
                break;
            }
        }

        --divisor;
    }
}



int main(int argc, char** argv) {
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    if (argc != 2) {
        std::cout << "Please only provide only 1 parameter (the required index)...\n";
        return 1;
    }

    uint64_t required_index = static_cast<uint64_t>(std::atoll(argv[1]));

    std::priority_queue<uint64_t> alexandrians;

    uint64_t r = 2;
    uint64_t size = 0;

    while (r < std::max(100ull, static_cast<uint64_t>(required_index * 1.1))) {
        addAlexandrians(r++, required_index, &alexandrians, &size);
    }

    std::cout << alexandrians.top() << '\n';

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time difference = " <<
        static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()) / 1000.0 <<
        "[s]" << '\n';

    return 0;
}