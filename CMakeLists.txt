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
        Parser.h
        Backend.h)

add_subdirectory(vendor/podofo)

add_executable(papy main.cpp
        # TODO: Add this conditionally based on an option.
        PdfBackend.cpp
        PdfBackend.h)
target_link_libraries(papy PUBLIC papylib)
target_compile_options(papy PUBLIC -Wall -Wextra)

# PDF Backend
target_link_libraries(papy PUBLIC podofo::podofo)
target_include_directories(papy PRIVATE ${CMAKE_BINARY_DIR}/vendor/podofo/src/podofo/podofo/auxiliary)