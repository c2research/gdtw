#!/bin/bash
export CXX=/usr/bin/clang++
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make && make test
cd ..
