#include "bigint.h"
#include <algorithm>


void BigInteger::removeLeadingZeroes(BigInteger* number) {
    while ((*number).digits.size() > 1 && (*number).digits.back() == 0) {
        (*number).digits.pop_back();
    }
}

BigInteger::BigInteger()
    : digits(1, 0) {}

BigInteger::BigInteger(uint64_t number)
    : digits((size_64u + size_32u - 1 ) / size_32u)
{

    for (size_t digit = 0; digit < digits.size(); ++digit, number >>= (8 * size_32u)) {
        digits[digit] = static_cast<uint32_t>(number);
    }

    removeLeadingZeroes(this);
}

BigInteger::BigInteger(const std::vector<uint32_t>& _digits)
    : digits(_digits)
{
    removeLeadingZeroes(this);
}

BigInteger::BigInteger(std::vector<uint32_t>&& _digits) noexcept
    : digits(std::move(_digits))
{
    removeLeadingZeroes(this);
}

BigInteger::BigInteger(const BigInteger& other)
    : digits(other.digits) {}

BigInteger::BigInteger(BigInteger&& other) noexcept
    : digits(std::move(other.digits)) {}


BigInteger& BigInteger::operator = (const BigInteger& other) {
    if (this == &other)
        return *this;
    digits = other.digits;

    return *this;
}

BigInteger& BigInteger::operator = (BigInteger&& other) noexcept {
    if (this == &other)
        return *this;
    digits = std::move(other.digits);

    return *this;
}

const BigInteger BigInteger::operator + (const BigInteger& other)const {
    return std::move(BigInteger(*this) += other);
}

BigInteger& BigInteger::operator += (const BigInteger& other) {
    digits.resize(std::max(digits.size(), other.digits.size()));

    uint64_t sum = 0;
    for (size_t digit = 0; digit < other.digits.size(); ++digit) {
        sum = (sum >> (8 * size_32u)) + digits[digit] + other.digits[digit];
        digits[digit] = sum;
    }

    for (size_t digit = other.digits.size(); digit < digits.size(); ++digit) {
        sum = (sum >> (8 * size_32u)) + digits[digit];
        digits[digit] = sum;
    }

    sum >>= (8 * size_32u);
    if (sum > 0) {
        digits.emplace_back(sum);
    }

    return *this;
}

BigInteger& BigInteger::operator ++() {

    for (auto& digit : digits) {
        ++digit;
        if (digit != 0)
            return *this;
    }

    digits.emplace_back(1);
    return *this;
}

uint64_t BigInteger::get64() const noexcept {

    uint64_t result = 0;
    for(size_t i = std::min(digits.size(), size_64u / size_32u) - 1; i < size_64u; --i)
        result = (result << (8 * size_32u)) | digits[i];
    return result;

}

const BigInteger BigInteger::operator * (const uint32_t& small_number) const {
    return std::move(BigInteger(*this) *= small_number);
}

BigInteger& BigInteger::operator *= (const uint32_t& small_number) {
    if (small_number == 0) {
        digits.resize(1);
        digits.front() = 0;
        return *this;
    }

    uint64_t product = 0;
    for (auto& digit : digits) {
        digit = product += static_cast<uint64_t>(digit) * small_number;
        product >>= (8 * size_32u);
    }

    if (product > 0)
        digits.emplace_back(product);

    return *this;
}

const std::vector<uint32_t> BigInteger::returnDigits() const noexcept {
    return digits;
}

const BigInteger BigInteger::operator / (const uint32_t& small_number) const {
    return std::move(BigInteger(*this) /= small_number);
}

BigInteger& BigInteger::operator /= (const uint32_t& small_number) {

    if (small_number == 0) {
        throw std::logic_error("Division by zero!");
    }

    uint64_t result = 0;
    for (size_t digit = digits.size() - 1; digit < digits.size(); --digit) {
        result = (result % small_number << (8 * size_32u)) + digits[digit];
        digits[digit] = result / small_number;
    }

    removeLeadingZeroes(this);
    return *this;
}

BigInteger& BigInteger::operator %= (const uint32_t& small_number)  {
    digits.resize(1);
    digits[0] = BigInteger(*this) % small_number;
    return *this;
}

uint32_t BigInteger::operator % (const uint32_t& small_number) const {

    if (small_number == 0) {
        throw std::logic_error("Division by zero!");
    }

    uint64_t result = 0;
    for (size_t digit = digits.size() - 1; digit < digits.size(); --digit) {
        result <<= (8 * size_32u);
        result += digits[digit];
        result %= small_number;
    }

    return static_cast<uint32_t>(result);
}