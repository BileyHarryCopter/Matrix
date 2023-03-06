#include <iostream>
#include <iterator>
#include "../../../include/matrix.hpp"

using namespace Matrix_Algebra;

int main (int argv, char** argc)
{
    size_t order = 0;
    std::cin >> order;

    std::vector<float> data_vec (order * order);
    for (auto& x : data_vec)
        std::cin >> x;

    Matrix<float> M {order, order, data_vec.cbegin(), data_vec.cend()};
    std::cout << M.determinant() << std::endl;

    return 0;
}