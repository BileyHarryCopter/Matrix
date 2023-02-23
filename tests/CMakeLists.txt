include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.11.0
)
FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

add_executable(operator_tests operator_tests.cpp)

target_link_libraries(operator_tests
    PRIVATE
    GTest::GTest
)

target_link_directories(operator_tests
                        PRIVATE ../Matrix_Lib/include
)

add_test(operator_gtests operator_tests)

add_executable(determinant_tests determinant_tests.cpp)

target_link_libraries(determinant_tests
    PRIVATE
    GTest::GTest
)

target_link_directories(determinant_tests
                        PRIVATE ../Matrix_Lib/include
)

add_test(determinant_gtests determinant_tests)
