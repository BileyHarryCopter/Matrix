#!/bin/bash

# colors for tracking 
green="\033[1;32m"
red="\033[1;31m"
default="\033[0m"

# parametrs of environment
test_dir="tests/end_to_end_tests/"
execution="./random_matrix"
result_dir="random_matrices/"

# aux variable
space=" "

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

if [ $# -ne 3 ]
then 
    echo "Perfomance testing requires 3 arguments"
else
    if [ $2 -le 0 ]
    then 
        echo "Number of tests should be more 0"
    else
        type=$1
        number_test=$2
        det_val=$3

        if [ "$type" = "integral" ]; then
            Mkdir "${result_dir}"

            # moving to the room directory
            cd ../../

            echo "Building n random matrices which determinant is ${det_val}..."
            echo -ne "\n"
            Build
            cd build
            echo -ne "\n"

            echo "Loading ${number} tests..."

            # moving to the test directory in the build directory
            cd ${test_dir}
            for ((i = 0; i < $number_test; i++))
            do
                ${execution}${space}${type}${space}${number_test}${space}${det_val} > ../../../${test_dir}${result_dir}result_${i}
                if diff -Z ../../../${test_dir}${result_dir}result_${i} ${det_val} > /dev/null
                    then
                        echo -e "${green}passed${default}"
                    else
                        echo -e "${red}failed${default}"
                fi
            done

            # moving to the room directory
            cd ../../../
            read -p "Do you wanna check directories for testing? (y / n) " answer
                case $answer in
                y) echo "Ok, check this out in \"tests/end_to_end_tests/random_matrices\" ";;
                *) echo "Delete testing derictories..."
                    rm -rf ${test_dir}${result_dir};;
                esac
        else 
            echo "This mode of testing is provided for integral type only"
        fi
    fi
fi
