cmake_minimum_required(VERSION 3.9)
project(huffman_lib)

set(CMAKE_CXX_STANDARD 14)

add_library(huffman_lib frequency.cpp include/frequency.h decoder.cpp include/decoder.h huffman.cpp include/huffman.h huffman_code.cpp include/huffman_code.h tree.cpp include/tree.h encoder.cpp include/encoder.h)