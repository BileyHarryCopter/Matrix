#include <iostream>
#include <iterator>
#include <string>
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
            return 1;
        }
        order = static_cast<int>(order_i);
    }
    catch (const std::logic_error&)
    {
        std::cerr << "Input error: enter the order of unsigned integral type\n";
        return 1;
    }

    std::vector<long double> data_vec (order * order);
    try
    {
        std::string x_s{};
        for (auto &x : data_vec)
        {
            std::cin >> x_s;
            x = std::stold(x_s, nullptr);
            x_s.clear();
        }
    }
    catch (const std::logic_error&)
    {
        std::cerr << "Input error: enter the massive of floating point numbers after an order of matrix\n";
        return 1;
    }

    Matrix<long double> M {order, order, data_vec.cbegin(), data_vec.cend()};
    std::cout << M.determinant() << std::endl;

    return 0;
}