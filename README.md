# HWMX: Attempt to write a class of matrix

## Annotation

This project is another task on course "Uses and applications of C++ language" by [K.I.Vladimirov](https://github.com/tilir) in which I try to create a class of matrix with base set of arithmetic operations and functions.

## Requirements

CMake of version 3.14 (or higher)

## How to install and build

### For linux:
1) Clone the repo and change the directory:
```
    git@github.com:BileyHarryCopter/Matrix.git
    cd Matrix
```
2) Create a directory **build** and build the project into this directory:
```
    cmake -B build
    cd build
    cmake --build .
```
3) In the directory **tests** you can explore unit_tests:
```
    cd test
    ./operator_tests
```