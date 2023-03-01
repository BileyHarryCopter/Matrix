#include <gtest/gtest.h>
#include "../Matrix_Lib/include/matrix.hpp"
#include "../Matrix_Lib/include/comparison.hpp"

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

TEST (DETERMINANT_TESTS, CALL_FOR_INTEGER)
{
    Matrix<int> A = {{0, 1}, 
                     {1, 0}};
    EXPECT_TRUE(A.determinant() == -1);
}

int main (int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}