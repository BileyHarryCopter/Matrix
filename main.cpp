#include <iostream>
#include <vector>
#include "matrix.hpp"

using namespace Matrix_Algebra;

int main(int argc, char *argv[])
{
    std::vector<int> v3 = {12,	3,	14,	1,
                            3,	4,	53,	2,
                            765,	22,	1,	1023,
                            34,	57,	1,  1};

    Matrix<double> A = {{1}, {2, 3}, {3, 3}};
    Matrix<double> B = -A;
    Matrix<double> C (2, 2, 3.2);
    Matrix<int> D = {{1, 0}, {1, 0}};
    Matrix<int> E = {{1, 0}};

    std::cout << A << B << C << D << E;
    std::cout << (D == E) << "\n";

    return 0;
}