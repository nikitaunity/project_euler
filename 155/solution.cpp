#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <unordered_set>


int calculateGCD(const int& first, const int& second) {
    if (second == 0) {
        return first;
    }

    return calculateGCD(second, first % second);
}


struct Fraction {
    int numerator;
    int denominator;

    Fraction(const int& numerator, const int& denominator):
        numerator(numerator), denominator(denominator) {}

    Fraction& simplify() {
        auto gcd = calculateGCD(numerator, denominator);
        numerator /= gcd;
        denominator /= gcd;
        return *this;
    }

    Fraction operator+(const Fraction& other) {
        return Fraction(numerator * other.denominator + denominator * other.numerator,
                        denominator * other.denominator).simplify();
    }

    Fraction operator~() {
        return Fraction(denominator, numerator);
    }

    void print() {
        std::cout << numerator << '/' << denominator << ' ';
        return;
    }
};

bool operator==(const Fraction& first, const Fraction& second) {
    return first.numerator == second.numerator
        && first.denominator == second.denominator;
}


struct FractionHash {
    size_t operator()(Fraction const& fraction) const noexcept {
        return (static_cast<size_t>(fraction.numerator) << 32)
            | static_cast<size_t>(fraction.denominator);
    }
};


size_t calculateNumberOfCapacitanceValues(int max_number) {

    const Fraction one = Fraction(1, 1);

    std::vector<std::vector<Fraction>> constructions(max_number,
                                                     std::vector<Fraction>());

    constructions[0] = {one};
    std::unordered_set<Fraction, FractionHash> fraction_hash_set;
    fraction_hash_set.insert(one);

    for (int current_level = 1; current_level < max_number; ++current_level) {
        for (int sub_level = 0; sub_level < (current_level + 1) >> 1; ++sub_level) {

            for (Fraction first: constructions[sub_level]) {
                for (Fraction second: constructions[current_level - sub_level - 1]) {

                    Fraction consecuitive = first + second;
                    Fraction parallel = ~(~first + ~second);

                    if (fraction_hash_set.find(consecuitive) == fraction_hash_set.end()) {
                        fraction_hash_set.insert(consecuitive);
                        constructions[current_level].push_back(consecuitive);
                    }

                    if (fraction_hash_set.find(parallel) == fraction_hash_set.end()) {
                        fraction_hash_set.insert(parallel);
                        constructions[current_level].push_back(parallel);
                    }

                }
            }

        }
    }

    return fraction_hash_set.size();
}

int main(int argc, char** argv) {

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    if (argc != 2) {
        std::cout << "Please provide only 1 parameter (max number of capacitors)...\n";
        return 1;
    }

    int max_capacitors = std::atoi(argv[1]);

    std::cout << calculateNumberOfCapacitanceValues(max_capacitors) << '\n';

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        std::cout << "Time difference = " <<
        static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()) / 1000.0 <<
        "[s]" << '\n';

    return 0;
}