cmake_minimum_required(VERSION 3.13)
project(n_puzzle)

set(CMAKE_CXX_STANDARD 17)

add_executable(n_puzzle main.cpp
        Grid.hpp)