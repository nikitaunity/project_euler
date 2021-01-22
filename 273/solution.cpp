#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <algorithm>


struct ComplexNumber {
    int64_t real;
    int64_t imaginary;

    ComplexNumber(const int64_t& real, const int64_t& imaginary):
        real(real), imaginary(imaginary) {}

    ComplexNumber& operator *= (const ComplexNumber& other) {
        auto real_copy = real;

        real = real * other.real - imaginary * other.imaginary;
        imaginary = real_copy * other.imaginary + imaginary * other.real;

        return *this;
    }

    ComplexNumber operator * (const ComplexNumber& other) const {
        return std::move(ComplexNumber(*this) *= other);
    }

    ComplexNumber operator~() const {
        return ComplexNumber(real, -imaginary);
    }

    int64_t returnSmallestByAbs() {
        return std::min(std::abs(real), std::abs(imaginary));
    }

};


std::vector<bool> generateSieve(size_t max_value) {
    size_t required_size = (max_value + 1) >> 1;
    std::vector<bool> sieve(required_size, true);
    sieve[0] = false;  // 1 is not prime

    size_t current_index = 1;
    while ((current_index << 1) * (current_index + 1) < required_size) {
        if (sieve[current_index]) {
            auto prime_number = (current_index << 1) +  1;

            for (auto prime_multiplicator = current_index + prime_number;
                prime_multiplicator < required_size;
                prime_multiplicator += prime_number) {
                    sieve[prime_multiplicator] = false;
                }

        }
        current_index += 1;
    }

    return sieve;
}


std::vector<uint64_t> generatePrimeNumbersOf1Mod4(const std::vector<bool>& sieve) {

    std::vector<uint64_t> prime_numbers;

    for (uint64_t index = 0; index < sieve.size(); index += 2) {
        if (sieve[index]) {
            prime_numbers.push_back((index << 1) + 1);
        }
    }

    return prime_numbers;
}


std::vector<ComplexNumber> findPrimitveSumsOfSquares(
    std::vector<uint64_t> prime_numbers) {

    std::vector<ComplexNumber> gaussian_primes;
    gaussian_primes.reserve(prime_numbers.size());

    for (auto p: prime_numbers) {
        for (uint64_t a = 1; a * a < p; ++a) {
            uint64_t b = sqrt(p - a * a);
            if (b * b + a * a == p) {
                gaussian_primes.emplace_back(ComplexNumber(a, b));
                break;
            }
        }
    }

    return gaussian_primes;
}



void traverseSquareFreeNumbers(
    const std::vector<ComplexNumber>& gaussian_primes,
    uint64_t pos, ComplexNumber acquired, uint64_t* answer, bool flipped, bool has_any_selected) {

    if (pos == gaussian_primes.size()) {
        *answer += acquired.returnSmallestByAbs();
        return;
    }

    traverseSquareFreeNumbers(gaussian_primes, pos + 1, acquired,
                              answer, flipped, has_any_selected);

    traverseSquareFreeNumbers(gaussian_primes, pos + 1, acquired * gaussian_primes[pos],
                              answer, flipped, true);

    if (!flipped && has_any_selected) {
        traverseSquareFreeNumbers(gaussian_primes, pos + 1, acquired * ~gaussian_primes[pos],
                                  answer, flipped, has_any_selected);
    }

    return;
}


int main(int argc, char* argv[]) {

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    if (argc != 2) {
        std::cout << "Please provide only 1 parameter (max prime)...\n";
        return 1;
    }

    int max_prime = std::atoi(argv[1]);
    auto sieve = generateSieve(static_cast<size_t>(max_prime));
    auto primes1mod4 = generatePrimeNumbersOf1Mod4(sieve);
    auto gaussian_primes = findPrimitveSumsOfSquares(primes1mod4);

    ComplexNumber one(1, 0);
    uint64_t answer = 0;
    traverseSquareFreeNumbers(gaussian_primes, 0, one, &answer, false, false);

    std::cout << answer << '\n';

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        std::cout << "Time difference = " <<
        static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()) / 1000.0 <<
        "[s]" << '\n';

    return 0;
}