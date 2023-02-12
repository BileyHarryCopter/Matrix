#include <gtest/gtest.h>
#include <vector>
#include "../Matrix_Lib/include/matrix.hpp"

using namespace Matrix_Algebra;

TEST (OPERATOR_TESTS, EQUALITY)
{
    Matrix<double> A = {{1, 0}, {2, 3}, {3, 3}};
    std::vector<double> v = {1, 0, 2, 3, 3, 3};
    Matrix<double> B {2, 3, v.begin(), v.end()};

    EXPECT_TRUE (A == B);
}

TEST (OPERATOR__TESTS, NON_EQUALITY)
{
    Matrix<double> A = {{1, 1}, {2, 3}, {3, 3}};
    std::vector<double> v = {1, 0, 2, 3, 3, 3};
    Matrix<double> B {2, 3, v.begin(), v.end()};

    EXPECT_TRUE (A != B);
}

TEST (OPERATOR_TESTS, ADDITION)
{
    Matrix<double> A = {{1, 0}, {2, 3}, {3, 3}};
    Matrix<double> B = {{2, 0.1}, {2, 3.3}, {-2.4, 0.0}};
    Matrix<double> C = {{3, 0.1}, {4, 6.3}, {0.6, 3}};
    Matrix<double> D = {{2, 0.1}, {2, 3.3}};

    EXPECT_TRUE ((A + B) == C);
    EXPECT_THROW({ Matrix<double> E = A + D; }, std::logic_error);
}

TEST (OPERATOR_TESTS, MULTIPLY_ON_NUMB)
{
    Matrix<double> A = {{1, 0}, {2, 3}, {3, 3}};
    Matrix<double> B = {{2, 0}, {4, 6}, {6, 6}};

    EXPECT_TRUE(B == 2.0 * A);
    EXPECT_TRUE(B == A * 2.0);
}

TEST (OPERATOR_TEST, DIVISION_ON_NUMB)
{
    Matrix<double> A = {{1, 0}, {2, 3}, {3, 3}};
    Matrix<double> B = {{2, 0}, {4, 6}, {6, 6}};

    EXPECT_TRUE(B == A / 0.5);
}

TEST (OPERATOR_TEST, CTOR_EYE)
{
    Matrix<double> A = {{1.0, 0.0, 0.0, 0.0},
                        {0.0, 1.0, 0.0, 0.0},
                        {0.0, 0.0, 1.0, 0.0},
                        {0.0, 0.0, 0.0, 1.0}};
    Matrix<double> B = Matrix<double>::eye(4);

    EXPECT_TRUE(B == A);
}

TEST (OPERATOR__TESTS, MATRIX_PRODUCT)
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
    EXPECT_THROW({ C == matrix_product(A, D); }, std::logic_error);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}