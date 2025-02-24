#!/bin/bash
set -e
g++ -Wall main.cpp -o count
./count $*