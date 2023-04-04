#include <iostream>
#include <iterator>
#include "matrix.hpp"

using namespace Matrix_Algebra;

int main (int argv, char** argc)
{
    size_t order = 0;
    try 
    {
        std::string order_s {};
        std::cin >> order_s;
        auto order_i = std::stof(order_s, nullptr);
        if (order_i != static_cast<int>(order_i) || order_i <= 0)
        {
            std::cerr << "Input error: enter the order of unsigned integral type\n";
            return -1;
        }
        order = static_cast<int>(order_i);
    }
    catch (const std::logic_error&)
    {
        std::cerr << "Input error: enter the order of unsigned integral type\n";
        return -1;
    }

    std::vector<long long int> data_vec (order * order);
    try
    {
        std::string x_s{};
        for (auto &x : data_vec)
        {
            std::cin >> x_s;
            auto x_v = std::stold(x_s, nullptr);
            if (x_v != static_cast<long long int>(x_v))
            {
                std::cerr << "Input error: enter the massive of integral numbers after the order of matrix\n";
                return -1;
            }
            x = static_cast<long long int>(x_v);
            x_s.clear();
        }
    }
    catch (const std::logic_error&)
    {
        std::cerr << "Input error: enter the massive of integral numbers after the order of matrix\n";
        return -1;
    }

    Matrix<long long int> M {order, order, data_vec.cbegin(), data_vec.cend()};
    std::cout << M.determinant() << std::endl;

    return 0;
}