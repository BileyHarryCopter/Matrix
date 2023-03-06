#ifndef DOUBLE_CMP
#define DOUBLE_CMP

namespace cmp
{

constexpr double epsilon = 1e-6;
constexpr double epsilon_for_det = 1e-3;

inline bool are_equal(const double a, const double b, const double accurancy = epsilon)
{
    auto diff = std::abs(a - b);
    auto max = std::min(std::abs(a), std::abs(b));

    if (max != 0.0) //  not a pure zero
        diff = diff / max;

    return diff < accurancy;
}

}

#endif