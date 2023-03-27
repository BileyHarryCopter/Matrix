#include <gtest/gtest.h>
#include <vector>
#include "array.hpp"

using namespace My_Array;

TEST(ARRAY_TESTS, ACCESS_TEST)
{
    std::vector<double> v = {1.2, 2.0, 2.0};
    Array<double> arr {10, v.begin(), v.end()};

    EXPECT_EQ (arr.size(), 3);
    EXPECT_EQ (arr.capacity(), 10);
    EXPECT_EQ (arr[2], 2.0);

    EXPECT_NO_THROW ({ auto non = arr[2]; });
    EXPECT_THROW    ({ auto non = arr[3]; }, Custom_Exceptions::Access_Except);
    EXPECT_THROW    ({ auto non = arr[200];},Custom_Exceptions::Access_Except);

    std::cout << v.capacity() << std::endl;
    std::cout << v[4]         << std::endl;
    std::cout << v.capacity() << std::endl;
}
