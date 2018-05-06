cmake_minimum_required(VERSION 2.8)

project(BIGINT)

#SET (CMAKE_CXX_COMPILER "/usr/local/bin/g++-7")

include_directories(${BIGINT_SOURCE_DIR})
include_directories(vector)

add_executable(big_integer_testing
               big_integer_testing.cpp
               big_integer.h
               big_integer.cpp
               vector/storage.h
               vector/storage.cpp
               gtest/gtest-all.cc
               gtest/gtest.h
               gtest/gtest_main.cc)

#add_executable(vector_tests
#               vector_tests.cpp
#        vector/storage.cpp
#        vector/storage.h)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++14")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined -fno-omit-frame-pointer -D_GLIBCXX_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

target_link_libraries(big_integer_testing -lpthread)
