# HWMX: Attempt to write a class of matrix

## Annotation

This project is another task on course "Uses and applications of C++ language" by [K.I.Vladimirov](https://github.com/tilir) in which I try to create a class of matrix with base set of arithmetic operations and functions.

## Requirements

CMake with version 3.14 (or higher)

Python3 with version 3.10.6 (or higher)

Installed library of [termcolor] (https://pypi.org/project/termcolor/)

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

3) You can launch generating N tests from directory [tests/end_to_end_tests/random_tests/](https://github.com/BileyHarryCopter/Matrix/tree/main/tests/end_to_end_tests/random_tests) with python script:
```
    ./testgen.py [N] [S] [M] [D]
```
where **N** - number of tests and all of other parametrs are same above; and **M** - mode of testing which can be **gauss** (*floating_point*) and **bareiss** (*integral*). But in this case **dump()** of the random matrices will be located in **tests/end_to_end_tests/random_matrices**

4) And you can also launch some basic tests with corner cases from directory [tests/end_to_end_tests/corner_tests](https://github.com/BileyHarryCopter/Matrix/tree/main/tests/end_to_end_tests/corner_tests):
```
    ./corner_test.sh
```