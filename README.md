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
    cmake --build build
```
## How to launch tests
1) First of all, **driver** for the task is located in [tests/end_to_end_tests/src/](https://github.com/BileyHarryCopter/Matrix/tree/main/tests/end_to_end_tests/src). You can execute this and redirect input stream:
```
    ./integral_matrix << test
```
or:
```
    ./real_matrix << test
```
where test is can be your own file with test
2) Secondly, you can launch the for the random matrix: 
```
    ./random_matrix [S] [D] [-dump]
```
where **S** - matrix order; **D** - value of determinant; option **-dump** needs for calling **dump()** of the random matrix to output stream, it can be ommited and in this case there is no calling **dump()**
3) You also can launch generating N tests for directory [tests/end_to_end_tests](https://github.com/BileyHarryCopter/Matrix/tree/main/tests/end_to_end_tests) with bash script:
```
    ./test.sh [N] [S] [D] [-dump]
```
where **N** - number of tests and all of other parametrs are same above. But in this case **dump()** of the random matrices will be located in **tests/end_to_end_tests/random_matrices**