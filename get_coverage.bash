#!/bin/bash

# Requirements :
# brew install gcovr

function run_tests {
    rm -rf build
    cmake -S . -B build -DCODE_COVERAGE=ON
    cmake --build build
    cd build && ctest
}

function main {
    local path=$(pwd)
    run_tests
    cd "${path}"
    gcovr --exclude tests --txt
}

main
