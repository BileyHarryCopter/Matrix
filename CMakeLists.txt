cmake_minimum_required(VERSION 3.14)

project(Matrix VERSION 1.0)

find_package(Threads REQUIRED)

find_package(GTest   REQUIRED)
enable_testing()

set (CMAKE_CXX_STANDARD             20)
set (CMAKE_CXX_STANDARD_REQUIRED  True)

include_directories(./include)

add_subdirectory(tests)
