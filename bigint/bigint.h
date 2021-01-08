#include <stdexcept>
#include <string>
#include <vector>

constexpr size_t size_64u = sizeof(uint64_t);
constexpr size_t size_32u = sizeof(uint32_t);

class BigInteger {
  public:
    BigInteger();
    BigInteger(uint64_t number);
    BigInteger(const std::vector<uint32_t>&);
    BigInteger(std::vector<uint32_t>&&) noexcept;
    BigInteger(const BigInteger&);
    BigInteger(BigInteger&&) noexcept;

    BigInteger& operator = (const BigInteger&);
    BigInteger& operator = (BigInteger&&) noexcept;

    const BigInteger operator + (const BigInteger&) const;
    BigInteger& operator += (const BigInteger&);
    BigInteger& operator ++();

    const BigInteger operator * (const uint32_t&) const;
    BigInteger& operator *= (const uint32_t&);

    const BigInteger operator / (const uint32_t&) const;
    BigInteger& operator /= (const uint32_t&);

    uint32_t operator % (const uint32_t&) const;
    BigInteger& operator %= (const uint32_t&);

    void removeLeadingZeroes(BigInteger* number);

    uint64_t get64() const noexcept;
    const std::vector<uint32_t> returnDigits() const noexcept;

  private:
    std::vector<uint32_t> digits;
};
