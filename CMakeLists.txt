cmake_minimum_required(VERSION 3.18)

project(papyrus)

set(CMAKE_CXX_STANDARD 17)

add_executable(papy main.cpp
        diagnostic.h
        lexer.h
        lexer.cpp
        token.h)
