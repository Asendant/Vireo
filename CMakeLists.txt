cmake_minimum_required(VERSION 3.31)
project(vireo)

set(CMAKE_CXX_STANDARD 20)

add_executable(vireo main.cpp
        src/lexer/lexer.cpp
        src/lexer/lexer.h
        src/lexer/token.h
        src/lexer/tokentype.h)
