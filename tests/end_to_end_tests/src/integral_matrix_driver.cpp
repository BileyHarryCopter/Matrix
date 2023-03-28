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
        auto order_i = std::stoi(order_s, nullptr, 10);
        if (order < 0)
        {
            std::cout << "Input error: enter the order of unsigned integral type\n";
            return 0;
        }
        order = order_i;
    }
    catch (std::logic_error)
    {
        std::cout << "Input error: enter the order of unsigned integral type\n";
        return 0;
    }

    std::vector<long long int> data_vec (order * order);
    try
    {
        std::string x_s{};
        for (auto &x : data_vec)
        {
            std::cin >> x_s;
            x = std::stoll(x_s, nullptr);
            x_s.clear();
        }
    }
    catch (std::logic_error)
    {
        std::cout << "Input error: enter the massive of floating point number after an order of matrix\n";
        return 0;
    }

    Matrix<long long int> M {order, order, data_vec.cbegin(), data_vec.cend()};
    std::cout << M.determinant() << std::endl;

    return 0;
}