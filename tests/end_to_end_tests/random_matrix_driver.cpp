#include <iostream>
#include <string>
#include "../../include/matrix.hpp"

using namespace Matrix_Algebra;

int main (int argv, char** argc)
{
    std::string type = std::string{argc[1]};

    std::string size_s = std::string{argc[2]};
    size_t size = std::stoll(size_s, &size, 10);

    if (type == "integral")
    {
        std::string det_s = std::string{argc[3]};
        int det = std::stoi (size_s, nullptr, 10);

        Matrix<int> tmp = Matrix<int>::random(size, det);

        std::cout << tmp.determinant() << std::endl << std::endl;
        std::cout << tmp;

        return det;
    }
    else
        throw std::runtime_error ("Type of matrix is undefined");

    return 0;
}