cmake_minimum_required(VERSION 3.20)

project(ryn)

set(CMAKE_CXX_STANDARD 20)

add_executable(ryn src/main.cpp
        src/tokenization.h
        src/parser.h
)