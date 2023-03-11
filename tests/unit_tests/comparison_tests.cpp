#include <gtest/gtest.h>
#include "../../include/comparison.hpp"

using namespace cmp;

TEST (COMPARISON_TEST, SMALL_NUMBER)
{
    auto first = 1.3400000002e-10;
    auto second = 1.3400000001e-10;

    auto third = 1.340000002e-10;
    auto fourth = 1.340000001e-10;

    EXPECT_TRUE(are_equal(first, second, epsilon));
    EXPECT_FALSE(are_equal(third, fourth, epsilon));
}

TEST (COMPARISON_TEST, BIG_NUMBER)
{
    auto first = 13400000002;
    auto second = 13400000001;

    auto third = 1340000002;
    auto fourth = 1340000001;

    EXPECT_TRUE(are_equal(first, second, epsilon));
    EXPECT_FALSE(are_equal(third, fourth, epsilon));
}

int main (int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}