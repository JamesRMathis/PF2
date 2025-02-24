#!/bin/bash
set -e
g++ -Wall -Werror -std=c++17 -g main.cpp config.cpp charmatrix.cpp -o main
./main $*