#!/bin/bash

# colors for tracking 
green="\033[1;32m"
red="\033[1;31m"
default="\033[0m"

# parametrs of environment
test_dir="tests/end_to_end_tests/"
execution="./random_matrix"
result_dir="random_matrices/"
src_name="src/"

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

if [ $# -le 2 ]
then 
    echo "Perfomance testing requires more than 3 arguments"
else
    dump_state=0
    if [ $2 -le 0 ]
    then
        echo "Number of tests should be more 0"
    else
        number_test=$1
        matrix_size=$2
        det_val=$3
        dump_state=0

        if [ $# -eq 4 ]
        then 
            dump_state=$4
        fi

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
            ${execution}${space}${matrix_size}${space}${det_val}${space}${dump_state} > ../../../${test_dir}${result_dir}result_${i}
            read p < ../../../${test_dir}${result_dir}result_${i}
            if [ $p -eq $det_val ]
                then
                    echo -e "Test ${i}: ${green}passed${default}"
                else
                    echo -e "Test ${i}: ${red}failed${default}"
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
    fi
fi
