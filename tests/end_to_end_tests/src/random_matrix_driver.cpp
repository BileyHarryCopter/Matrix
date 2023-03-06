#include <iostream>
#include <string>
#include "../../../include/matrix.hpp"

using namespace Matrix_Algebra;

int main (int argv, char** argc)
{
    size_t size = 0;
    int det = 0;
    try
    {
        std::string size_s {argc[1]};
        std::string det_s {argc[2]};
        size = std::stoll (size_s, &size, 10);
        det = std::stoi (det_s, nullptr, 10);
    }
    catch (std::logic_error)
    {
        std::cout << "Please, enter 2 arguments: size det\n";
        return 0;
    }

    Matrix<int> tmp = Matrix<int>::random(size, det);

    std::cout << tmp.determinant() << std::endl << std::endl;

    if (argv == 4)
    {
        std::string is_dump_s {argc[3]};
        if (is_dump_s == "-dump")
            std::cout << tmp;
    }

    return 0;
}