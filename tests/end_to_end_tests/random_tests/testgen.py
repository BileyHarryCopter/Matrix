#!/usr/bin/env python3

from io import TextIOWrapper
import sys, getopt              # for receiving and parcing command line arguments
import numpy                    # 
import random as RAND           # for creating random matrix by random value of determinant
import os                       # for system calls
from termcolor import colored   # for colored printing

# Variables of environment:
room_dir_from_testgen = "../../../"
build_dir             = "build/"
test_dir              = "tests/end_to_end_tests/"
random_test_dir       = "random_tests/"
real_test_exe         = "./real_matrix"
integral_test_exe     = "./integral_matrix"
random_test_exe       = "./random_matrix"
results_dir           = "results/"
answers_dir           = "answers/"
tests_dir             = "tests/"

# Constants:
epsilon      = 0.0001
gauss_mode   = "real"
bareiss_mode = "integral"

# Args:
#   1st - number of tests
#   2nd - size of matrices
#   3rd - mode of algorithm: gauss or bareiss 
#   4th - diapazon for determinant values
#   5th - [-dump] - flag for dump recieved tests

def mkdir(name_dir):
    os.system("mkdir " + name_dir)

def build_tests():

    print("Building tests...")
    os.chdir(str(os.getcwd()) + "/" + room_dir_from_testgen)
    os.system("cmake -B " + build_dir)
    os.system("cmake --build " + build_dir)
    os.chdir(str(os.getcwd()) + "/" + test_dir + random_test_dir)

def take_answer(file_name, mode):

    with open(file_name, "rb") as file:
        try:
            file.seek(-2, os.SEEK_END)
            while file.read(1) != b'\n':
                file.seek(-2, os.SEEK_CUR)
        except OSError:
            file.seek(0)
        last_line = file.readline()

        if mode == "integral":
            answer = int(last_line)
        elif mode == "real":
            answer = float(last_line)
        file.close()

        return answer


def generate_tests(mode_matrix, number_tests, size_matrices, det_value, dump_flag):

    mkdir(tests_dir)

    if dump_flag == "-dump":
        if mode_matrix == "gauss":
            for i in range(0, number_tests):
                os.system(room_dir_from_testgen + build_dir + test_dir + random_test_exe + " " + str(size_matrices) + " " + gauss_mode + " " + str(det_value) + " -dump " + " > " + tests_dir + "test_" + str(i))
        elif mode_matrix == "bareiss":
            for i in range(0, number_tests):
                os.system(room_dir_from_testgen + build_dir + test_dir + random_test_exe + " " + str(size_matrices) + " " + bareiss_mode + " " + str(det_value) + " -dump " + " > " + tests_dir + "test_" + str(i))
    else:
        if mode_matrix == "gauss":
            for i in range(0, number_tests):
                os.system(room_dir_from_testgen + build_dir + test_dir + random_test_exe + " " + str(size_matrices) + " " + gauss_mode + " " + str(det_value) + " > " + tests_dir + "test_" + str(i))
        elif mode_matrix == "bareiss":
            for i in range(0, number_tests):
                os.system(room_dir_from_testgen + build_dir + test_dir + random_test_exe + " " + str(size_matrices) + " " + bareiss_mode + " " + str(det_value) + " > " + tests_dir + "test_" + str(i))

    

def launch_tests(mode_matrix, dump_flag, number_tests):
    print("Launching tests...")

    mkdir(results_dir)

    if mode_matrix == "gauss":
        for i in range (0, number_tests):
            os.system(room_dir_from_testgen + build_dir + test_dir + real_test_exe + " < " + tests_dir + "test_" + str(i) + " > " + results_dir + "result_" + str(i))
            diff = take_answer(tests_dir + "test_" + str(i), gauss_mode) - take_answer(results_dir + "result_" + str(i), gauss_mode)
            if abs(diff) < epsilon:
                print("Test_" + str(i) + ": ", colored ('passed', 'green')) 
            else:
                print("Test_" + str(i) + ": ", colored('failed', 'red'))
            print("Error of calculating: " + str(diff))
    elif mode_matrix == "bareiss":
        for i in range (0, number_tests):
            os.system(room_dir_from_testgen + build_dir + test_dir + integral_test_exe + " < " + tests_dir + "test_" + str(i) + " > " + results_dir + "result_" + str(i))
            if take_answer(tests_dir + "test_" + str(i), bareiss_mode) == take_answer(results_dir + "result_" + str(i), bareiss_mode):
                print("Test_" + str(i) + ": ", colored('passed', 'green'))
            else:
                print("Test_" + str(i) + ": ", colored('failed', 'red'))

def delete_tests():
    os.system("rm -rf " + tests_dir + " " + results_dir)

def main():
    number_tests  = int(sys.argv[1])
    size_matrices = int(sys.argv[2])
    mode_matrix   = str(sys.argv[3])
    dump_flag = "-dump"

    det_value = 0
    if mode_matrix == "gauss":
        det_value = float(sys.argv[4])
    elif mode_matrix == "bareiss":
        det_value = int(sys.argv[4])
    else:
        print ("Mode should be gauss or bareiss!\n")
        return

    build_tests()
    generate_tests(mode_matrix, number_tests, size_matrices, det_value, dump_flag)
    launch_tests(mode_matrix, dump_flag, number_tests)

    print("Do you want check tests out? (y / n)\n")
    answer = str(input())
    if answer == "y":
        print("Ok, go to the \"tests/\" directory")
    else:
        print("Ok, delete tests...")
        delete_tests()

main()