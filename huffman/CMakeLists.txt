cmake_minimum_required(VERSION 3.5)
project(huffman)

#SET (CMAKE_CXX_COMPILER "/usr/local/bin/g++-7")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -D_GLIBCXX_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

#-fsanitize=address,undefined

include_directories(${CMAKE_SOURCE_DIR}/lib/huffman-lib/include)
include_directories(${CMAKE_SOURCE_DIR}/lib)

add_subdirectory(lib/huffman-lib)

add_executable(huffman src/main.cpp src/huffman_encoder.cpp src/huffman_encoder.h src/buffered_reader.cpp src/buffered_reader.h src/buffered_writer.cpp src/buffered_writer.h src/huffman_decoder.cpp src/huffman_decoder.h lib/huffman-lib/include/consts.h src/impl_consts.h)
target_link_libraries(huffman huffman_lib)

add_executable(huffman_lib_testing
        test/huffman_lib_testing.cpp
        lib/gtest/gtest-all.cc
        lib/gtest/gtest.h
        lib/gtest/gtest_main.cc)

target_link_libraries(huffman_lib_testing huffman_lib)