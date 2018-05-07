cmake_minimum_required(VERSION 3.9)
project(huffman)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -D_GLIBCXX_DEBUG")
#-fsanitize=address,undefined

include_directories(${CMAKE_SOURCE_DIR}/lib/huffman-lib/include)
include_directories(${CMAKE_SOURCE_DIR}/lib)

add_subdirectory(lib/huffman-lib)

add_executable(huffman main.cpp)
target_link_libraries(huffman huffman_lib)

add_executable(huffman_lib_testing
        test/huffman_lib_testing.cpp
        lib/gtest/gtest-all.cc
        lib/gtest/gtest.h
        lib/gtest/gtest_main.cc)

target_link_libraries(huffman_lib_testing huffman_lib)