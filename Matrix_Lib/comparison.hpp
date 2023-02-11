#ifndef DOUBLE_CMP
#define DOUBLE_CMP

namespace cmp
{

constexpr double epsilon = 1e-6;

inline bool are_equal(const double a, const double b, const double accurancy = epsilon)
{
    auto diff = std::abs(a - b);
    
    return diff < accurancy;
}

}

#endif