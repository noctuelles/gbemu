cmake_minimum_required(VERSION 3.20)

project(gbemu VERSION 1.0)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_STANDARD 20)

include_directories(includes)
include_directories(/usr/include/c++/14)
include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/b514bdc898e2951020cbdca1304b75f5950d1f59.zip
)

FetchContent_Declare(
        sdl2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL
        GIT_TAG 9c821dc21ccbd69b2bda421fdb35cb4ae2da8f5e
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest sdl2)

add_executable(gbemu srcs/CPU.cxx
        srcs/main.cxx
        srcs/Bus.cxx
        srcs/Cartridge.cxx
        includes/Cartridge.hxx)

target_link_libraries(
        gbemu
        SDL2::SDL2
)

add_executable(gbemu_test
        srcs/CPU.cxx
        srcs/Bus.cxx
        srcs/tests/CPU.cxx
        srcs/tests/Integration.cxx)

target_link_libraries(
        gbemu_test
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(gbemu_test)