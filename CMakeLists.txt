cmake_minimum_required(VERSION 3.26)
project(Minesweeper)

set(CMAKE_CXX_STANDARD 17)

add_executable(Minesweeper main.cpp
        plain.h
        plain.cpp)
