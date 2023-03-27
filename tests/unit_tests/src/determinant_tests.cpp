#include <gtest/gtest.h>
#include "matrix.hpp"
#include "comparison.hpp"

using namespace cmp;
using namespace Matrix_Algebra;

TEST (DETERMINANT_TESTS, FINDING_PIVOT)
{
    Matrix<float> A = {{0.0, 1.0},
                       {1.0, 0.0}};
    EXPECT_TRUE(are_equal(A.determinant(), -1.0, epsilon));
}

TEST (DETERMINANT_TESTS, ZERO_DET)
{
    Matrix<float> A = {{0.0, 1.0, 2.0},
                       {0.0, 3.0, 4.0},
                       {0.0, 5.0, 6.0}};
    EXPECT_TRUE(are_equal(A.determinant(), 0.0, epsilon));
}

TEST (DETERMINANT_TESTS, CALL_FOR_INTEGRAL)
{
    Matrix<int> A = {{0, 1}, 
                     {1, 0}};
    EXPECT_TRUE(A.determinant() == -1);
}

TEST (DETERMINANT_TESTS, RANDOM_MATRIX)
{
    Matrix<long long int> A = Matrix<long long int>::random (10, 4);
    Matrix<long double> B = Matrix<long double>::random(100, 11.200000);

    EXPECT_TRUE (A.determinant() == 4);
    EXPECT_TRUE(are_equal(B.determinant(), 11.200000, epsilon));
}
