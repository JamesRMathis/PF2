#!/bin/bash
set -e
g++ -Wall -Werror -g main.cpp config.cpp charmatrix.cpp -o lex
./lex $*