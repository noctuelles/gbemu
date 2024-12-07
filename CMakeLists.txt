cmake_minimum_required(VERSION 3.10)
project(GbEmu VERSION 1.0)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_STANDARD 11)
include_directories(includes)
add_executable(gbemu srcs/CPU.cxx
        includes/Memory.hxx
        srcs/Memory.cxx
        includes/Bus.hxx)