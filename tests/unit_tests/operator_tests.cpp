#include <gtest/gtest.h>
#include <vector>
#include "../../include/matrix.hpp"

using namespace Matrix_Algebra;

//  TESTS FOR OPERATOR WITH MATRICES  //

TEST (MATRIX_OPERATORS_TESTS, EQUALITY)
{
    Matrix<double> A = {{1, 0}, {2, 3}, {3, 3}};
    std::vector<double> v = {1, 0, 2, 3, 3, 3};
    Matrix<double> B = {2, 3, v.begin(), v.end()};

    EXPECT_TRUE (A == B);
}

TEST (MATRIX_OPERATORS_TESTS, NON_EQUALITY)
{
    Matrix<double> A = {{1, 1}, {2, 3}, {3, 3}};
    std::vector<double> v = {1, 0, 2, 3, 3, 3};
    Matrix<double> B = {2, 3, v.begin(), v.end()};

    EXPECT_TRUE (A != B);
}

TEST (MATRIX_OPERATORS_TESTS, ADDITION)
{
    Matrix<double> A = {{1, 0}, {2, 3}, {3, 3}};
    Matrix<double> B = {{2, 0.1}, {2, 3.3}, {-2.4, 0.0}};
    Matrix<double> C = {{3, 0.1}, {4, 6.3}, {0.6, 3}};
    Matrix<double> D = {{2, 0.1}, {2, 3.3}};

    EXPECT_TRUE ((A + B) == C);
    EXPECT_THROW({ Matrix<double> E = A + D; }, Addition_Except);
}

TEST (MATRIX_OPERATORS_TESTS, MULTIPLY_ON_NUMB)
{
    Matrix<double> A = {{1, 0}, {2, 3}, {3, 3}};
    Matrix<double> B = {{2, 0}, {4, 6}, {6, 6}};

    EXPECT_TRUE(B == 2.0 * A);
    EXPECT_TRUE(B == A * 2.0);
}

TEST (MATRIX_OPERATORS_TESTS, DIVISION_ON_NUMB)
{
    Matrix<double> A = {{1, 0}, {2, 3}, {3, 3}};
    Matrix<double> B = {{2, 0}, {4, 6}, {6, 6}};

    EXPECT_TRUE(B == A / 0.5);
    EXPECT_THROW({ B / 0.0; }, Division_Overflow_Except);
}

TEST (MATRIX_OPERATORS_TESTS, CTOR_EYE)
{
    Matrix<double> A = {{1.0, 0.0, 0.0, 0.0},
                        {0.0, 1.0, 0.0, 0.0},
                        {0.0, 0.0, 1.0, 0.0},
                        {0.0, 0.0, 0.0, 1.0}};
    Matrix<double> B = Matrix<double>::eye(4);

    EXPECT_TRUE(B == A);
}

TEST (MATRIX_OPERATORS_TESTS, MATRIX_PRODUCT)
{
    Matrix<double> A = {{1.0, 0.0, 0.0},
                        {0.0, 1.0, 0.0},
                        {0.0, 0.0, 1.0},
                        {0.0, 0.0, 0.0}};
    Matrix<double> B = {{1.0, 0.0, 0.0, 0.0},
                        {0.0, 1.0, 0.0, 0.0},
                        {0.0, 0.0, 1.0, 0.0}};
    Matrix<double> C = {{1.0, 0.0, 0.0, 0.0},
                        {0.0, 1.0, 0.0, 0.0},
                        {0.0, 0.0, 1.0, 0.0},
                        {0.0, 0.0, 0.0, 0.0}};
    Matrix<double> D = Matrix<double>::eye(2);

    EXPECT_TRUE(C == matrix_product(A, B));
    EXPECT_THROW({ C == matrix_product(A, D); }, Product_Except);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}