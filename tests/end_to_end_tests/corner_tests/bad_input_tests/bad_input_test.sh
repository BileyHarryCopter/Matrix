#!/bin/bash

# colors for tracking 
green="\033[1;32m"
red="\033[1;31m"
default="\033[0m"

# parametrs of environment
test_dir="tests/end_to_end_tests/"
execution="./integral_matrix"
result_dir="results/"
tests_dir="tests/"
bad_input_tests_dir="corner_tests/bad_input_tests/"

# number of basic tests
number_test=7

function Build
{
    cmake -B build
    cmake --build build
}

function Mkdir
{
    rm -rf $1
    mkdir $1
}

# moving to the room directory
cd ../../../../

pwd

echo "Building basic tests..."
echo -ne "\n"
Build
cd build
echo -ne "\n"

echo "Loading basic tests..."

# moving to the directory with 
cd ${test_dir}

for ((i = 0; i < $number_test; i++))
do
    ${execution} < ../../../${test_dir}${bad_input_tests_dir}${tests_dir}test${i}
    exit_code=$?
    if [ ${exit_code} -eq 1 ]
    then
        echo -e "Test ${i}: ${green}passed${default}"
    else
        echo -e "Test ${i}: ${red}failed${default}"
    fi
done

${execution} < ../../../${test_dir}${bad_input_tests_dir}${tests_dir}test7
exit_code=$?
if [ ${exit_code} -eq 0 ]
then
    echo -e "Test ${i}: ${green}passed${default}"
else
    echo -e "Test ${i}: ${red}failed${default}"
fi

# moving to the room directory
cd ../../../
rm -rf ${test_dir}${bad_input_tests_dir}${result_dir}
