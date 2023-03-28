#include <gtest/gtest.h>
#include <vector>
#include "array.hpp"

using namespace My_Array;

TEST(ARRAY_TESTS, COPY_CTOR)
{
    std::vector<double> v = {1.0, 2.0, 3.0};
    Array<double> A{10, v.begin(), v.end()};
    Array<double> B {A};

    EXPECT_TRUE (A.size() == B.size() && A.capacity() == B.capacity() &&
                 A[0] == B[0] && A[1] == B[1] && A[2] == B[2]);
}

TEST(ARRAY_TESTS, COPY_ASIGN)
{
    std::vector<double> v = {1.0, 2.0, 3.0};
    Array<double> A {10, v.begin(), v.end()};
    Array<double> B (10);

    B = A;

    EXPECT_TRUE (A.size() == B.size() && A.capacity() == B.capacity() &&
                 A[0] == B[0] && A[1] == B[1] && A[2] == B[2]);   
}

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
}

TEST(ARRAY_TEST, PUSH_BACK)
{
    std::vector<double> v = {1.2, 2.0, 2.0};
    Array<double> arr {10, v.begin(), v.end()};

    EXPECT_TRUE (arr.size() == 3 && arr.capacity() == 10 &&
                 arr[0] == 1.2 && arr[1] == 2.0 && arr[2] == 2.0);

    arr.push_back(5.6);
    EXPECT_TRUE (arr.size() == 4 && arr.capacity() == 10 &&
                 arr[0] == 1.2 && arr[1] == 2.0 && arr[2] == 2.0 && arr[3] == 5.6);

    arr.fill(1.0);
    arr.push_back(2.0);

    EXPECT_TRUE (arr.size() == 11 && arr.capacity() == 21 && arr[10] == 2.0);
}

TEST(ARRAY_TEST, FILL)
{
    std::vector<double> v = {1.2, 2.0, 2.0};
    Array<double> arr {10, v.begin(), v.end()};

    arr.fill(1.0);
    EXPECT_TRUE (arr[0] == 1.0 && arr[1] == 1.0 && arr[2] == 1.0);

    arr.fill(0, 3, 2.0);
    EXPECT_TRUE (arr[0] == 2.0 && arr[1] == 2.0 && arr[2] == 2.0);

    EXPECT_THROW({arr.fill(9, 11, 2.4);},  Custom_Exceptions::Range_Except);
    EXPECT_THROW({arr.fill(11, 12, 2.4);}, Custom_Exceptions::Range_Except);
    EXPECT_THROW({arr.fill(5, 4, 2.4);},   Custom_Exceptions::Range_Except);
}

TEST(ARRAY_TEST, POP_BACK)
{
    std::vector<double> v = {1.2, 2.0};
    Array<double> arr {10, v.begin(), v.end()};

    EXPECT_TRUE (arr.size() == 2 && arr.capacity() == 10 &&
                 arr[0] == 1.2 && arr[1] == 2.0);

    arr.pop_back();

    EXPECT_TRUE (arr.size() == 1 && arr.capacity() == 10 &&
                 arr[0] == 1.2);

    arr.pop_back();

    EXPECT_TRUE (arr.size() == 0 && arr.capacity() == 10);

    EXPECT_THROW({ arr.pop_back(); }, Custom_Exceptions::Pop_Except);
}

TEST(ARRAY_TEST, SWAP_SUBARRAY)
{
    std::vector<double> v = {1.0, 2.0, 3.0, 4.0, 5.0};
    Array<double> arr {5, v.begin(), v.end()};

    arr.swap_subarray(0, 2, 1);
    EXPECT_TRUE(arr.size() == 5 && arr.capacity() == 5 &&
                arr[0] == 2.0 && arr[1] == 3.0 && arr[2] == 1.0 && arr[3] == 4.0 && arr[4] == 5.0);

    arr.swap_subarray(0, 1, 4);
    EXPECT_TRUE (arr.size() == 5 && arr.capacity() == 5 &&
                 arr[0] == 5.0 && arr[1] == 3.0 && arr[2] == 1.0 && arr[3] == 4.0 && arr[4] == 2.0);

    EXPECT_THROW({ arr.swap_subarray(0, 1, 5); }, Custom_Exceptions::Border_Except);
}
