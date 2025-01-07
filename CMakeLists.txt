cmake_minimum_required(VERSION 3.20)

project(gbemu VERSION 1.0)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_STANDARD 23)

include_directories(includes)
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

FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui
        GIT_TAG 993fa347495860ed44b83574254ef2a317d0c14f
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest sdl2 imgui)

set(IMGUI_FILES
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

add_library(imgui STATIC ${IMGUI_FILES})
target_include_directories(imgui PRIVATE ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)
target_link_libraries(imgui SDL2)

include_directories(${imgui_SOURCE_DIR})
include_directories(${imgui_SOURCE_DIR}/backends)

add_compile_options(-Wall -Werror -Wextra -Wpedantic -g3)

add_executable(gbemu
        srcs/core/SM83.cxx
        srcs/core/Opcode.cxx
        srcs/main.cxx
        srcs/Bus.cxx
        srcs/Cartridge.cxx
        includes/Cartridge.hxx
        includes/tests/CPU.hxx
        srcs/Utils.cxx)

target_link_libraries(
        gbemu
        imgui
        SDL2
)

add_executable(gbemu_test
        srcs/core/SM83.cxx
        srcs/core/Opcode.cxx
        srcs/Bus.cxx
        srcs/Cartridge.cxx
        srcs/Utils.cxx
        srcs/tests/cpu/BlarggInstructions.cxx
        srcs/tests/Cartridge.cxx)

target_compile_definitions(gbemu_test PUBLIC ROMS_PATH="${CMAKE_SOURCE_DIR}/srcs/tests/rom")

target_link_libraries(
        gbemu_test
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(gbemu_test)