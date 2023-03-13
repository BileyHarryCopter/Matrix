#ifndef DOUBLE_CMP
#define DOUBLE_CMP

namespace cmp
{

constexpr double epsilon = 10e-10;
constexpr double big_number = 10e6;

inline bool are_equal(const double a, const double b, const double accurancy = epsilon)
{
    auto diff = std::abs(a - b);
    auto min = std::min(std::abs(a), std::abs(b));

    if (min > big_number)
        diff = diff / min;

    return diff < accurancy;
}

}

#endif
