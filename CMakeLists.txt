cmake_minimum_required(VERSION 3.18)

project(papyrus)

set(CMAKE_CXX_STANDARD 20)

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

add_executable(papy main.cpp
        # TODO: Add this conditionally based on an option.
        PdfBackend.cpp
        PdfBackend.h
        FileWatcher.cpp
        FileWatcher.h)
target_link_libraries(papy PUBLIC papylib)
target_compile_options(papy PUBLIC -Wall -Wextra)

# PDF Backend
add_subdirectory(vendor/podofo EXCLUDE_FROM_ALL)
target_link_libraries(papy PRIVATE podofo::podofo)
target_include_directories(papy PRIVATE ${CMAKE_BINARY_DIR}/vendor/podofo/src/podofo/podofo/auxiliary)

# LSP
set(LSP_USE_SANITIZERS OFF)
add_subdirectory(vendor/lsp-framework EXCLUDE_FROM_ALL)
add_executable(papy-lsp-server Lsp.cpp)
target_link_libraries(papy-lsp-server PRIVATE lsp papylib)
target_compile_options(papy-lsp-server PRIVATE -Wall -Wextra)
