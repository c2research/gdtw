#!/bin/bash
export CXX=/usr/bin/clang++
mkdir -p build
cd build
cmake ..
make
cd ..
