#include <iostream>
#include <string>
#include "matrix.hpp"

using namespace Matrix_Algebra;
enum modes {INTEGRAL, REAL};

// 1st arg - size of random matrix
// 2nd arg - mode: integral of real
// 3rd arg - determinant's value
// 4th arg - optional -dump for output the matrix

int main (int argv, char** argc)
{
    size_t size = 0;
    modes mode;
    try
    {
        std::string size_s {argc[1]};
        std::string mode_s {argc[2]};
        size = std::stoll (size_s, &size, 10);
        if (mode_s == "integral")
            mode = INTEGRAL;
        else if (mode_s == "real")
            mode = REAL;

        std::string det_s {argc[3]};
        if (mode == INTEGRAL)
        {
            long int det = std::stol (det_s, nullptr, 10);
            Matrix<long long int> tmp = Matrix<long long int>::random(size, det);

            if (argv == 5 && (std::string{argc[4]} == "-dump"))
            {
                std::cout << size << std::endl << std::endl;
                std::cout << tmp << std::endl;
            }
            std::cout << tmp.determinant();
        }
        else if (mode == REAL)
        {
            double det = std::stof(det_s, nullptr);
            Matrix<long double> tmp = Matrix<long double>::random(size, det);
            
            if (argv == 5 && (std::string{argc[4]} == "-dump"))
            {
                std::cout << size << std::endl << std::endl;
                std::cout << tmp << std::endl;
            }
            std::cout << tmp.determinant();
        }
    }
    catch (const std::logic_error&)
    {
        std::cerr << "Please, enter 3 (or 4) arguments: size mode det (-dump opt.)\n";
        return 1;
    }

    return 0;
}