cmake_minimum_required(VERSION 3.14)

project(Matrix VERSION 1.0)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(Matrix main.cpp)
add_subdirectory(Matrix_Lib)

target_link_libraries(Matrix PUBLIC Matrix_Lib)
target_include_directories(Matrix PUBLIC
                        "${PROJECT_BINARY_DIR}"
                        "${PROJECT_SOURCE_DIR}/Matrix_Lib"
                        )                                   
