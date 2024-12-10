cmake_minimum_required(VERSION 3.20)
project(gbemu VERSION 1.0)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_STANDARD 14)
include_directories(includes)

include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/b514bdc898e2951020cbdca1304b75f5950d1f59.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(gbemu srcs/CPU.cxx
        includes/Memory.hxx
        srcs/Memory.cxx
        includes/Bus.hxx
        srcs/main.cxx
        srcs/utils.cxx
        includes/utils.hxx)