cmake_minimum_required(VERSION 3.12)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

project(lled C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
add_compile_options(-ggdb -D_DEBUG)

add_subdirectory(external)
add_subdirectory(src)
add_compile_options(-Wall
        -O3
        -Wextra
)
if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wno-maybe-uninitialized)
endif()
