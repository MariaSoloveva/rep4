cmake_minimum_required(VERSION 3.15)
project(lzw_2017_version)

set(CMAKE_CXX_STANDARD 17)

add_executable(lzw_2017_version main.cpp BinaryFile.cpp BinaryFile.hpp PrefixTree.hpp PrefixTree.cpp)
