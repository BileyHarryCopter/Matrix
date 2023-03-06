cmake_minimum_required(VERSION 3.14)

project(Matrix VERSION 1.0)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED True)

enable_testing()

add_subdirectory(tests)
include_directories(Matrix include)
