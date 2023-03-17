#include <gtest/gtest.h>
#include "../../include/comparison.hpp"

using namespace cmp;

TEST (COMPARISON_TEST, SMALL_NUMBER)
{
    auto first = 1.342e-10;
    auto second = 1.341e-10;

    EXPECT_TRUE(are_equal(first, second, epsilon));
}

TEST (COMPARISON_TEST, BIG_NUMBER)
{
    auto first = 13400000002;
    auto second = 13400000001;

    auto third = 134000002;
    auto fourth = 134000001;

    EXPECT_TRUE(are_equal(first, second, epsilon));
    EXPECT_FALSE(are_equal(third, fourth, epsilon));
}

int main (int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}