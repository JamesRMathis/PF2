#!/bin/bash
set -e
g++ -Wall -Werror -g main.cpp config.cpp lexicon.cpp -o lex
./lex $*