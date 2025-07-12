cmake_minimum_required(VERSION 3.18)

project(papyrus)

set(CMAKE_CXX_STANDARD 17)

add_library(papylib STATIC
        diagnostic.h
        lexer.h
        lexer.cpp
        token.h
        ast.cpp
        ast.h
        Parser.cpp
        Parser.h)

add_executable(papy main.cpp)
target_link_libraries(papy PUBLIC papylib)
target_compile_options(papy PUBLIC -Wall -Wextra)
