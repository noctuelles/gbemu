cmake_minimum_required(VERSION 3.20)

project(gbemu VERSION 1.0)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_STANDARD 23)
set(BUILD_TESTING OFF)

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

FetchContent_Declare(faker
        GIT_REPOSITORY https://github.com/cieslarmichal/faker-cxx.git
        GIT_TAG main
)

FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui
        GIT_TAG 95e047244eb6ee1a27e1b891ffe4ad80b3567aad
)

FetchContent_Declare(
        imgui_club
        GIT_REPOSITORY https://github.com/ocornut/imgui_club
        GIT_TAG 17510b03602c8b6c7ed48fd8fa29d9a75eb10dc6
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest sdl2 imgui imgui_club faker)

set(IMGUI_FILES
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer2.cpp
)

add_library(imgui STATIC ${IMGUI_FILES})
target_include_directories(imgui PRIVATE ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)
target_link_libraries(imgui SDL2)

include_directories(${imgui_SOURCE_DIR})
include_directories(${imgui_SOURCE_DIR}/backends)
include_directories(${imgui_club_SOURCE_DIR})

add_compile_options(-Wall -Werror -Wextra -Wpedantic -g3)

add_executable(gbemu
        srcs/hardware/core/SM83.cxx
        srcs/hardware/core/Opcode.cxx
        srcs/hardware/core/Disassembler.cxx
        srcs/hardware/Timer.cxx
        srcs/hardware/PPU.cxx
        srcs/main.cxx
        srcs/hardware/Bus.cxx
        srcs/hardware/Cartridge.cxx
        srcs/Utils.cxx
        includes/hardware/Cartridge.hxx
        includes/tests/hardware/core/SM83.hxx
        includes/Common.hxx
        includes/hardware/PPU.hxx
        includes/hardware/Timer.hxx
        srcs/hardware/WorkRAM.cxx
        includes/hardware/WorkRAM.hxx
        srcs/hardware/EchoRAM.cxx
        includes/hardware/EchoRAM.hxx
        srcs/hardware/Joypad.cxx
        includes/hardware/Displayable.hxx
        includes/hardware/LCD.hxx
        srcs/hardware/Displayable.cxx
        srcs/hardware/LCD.cxx
        includes/ui/ForkAwesomeFont.hxx
        srcs/ui/Debugger.cxx
        includes/ui/Debugger.hxx
        includes/ui/AddressSpaceMemoryEditor.hxx
        srcs/ui/AddressSpaceMemoryEditor.cxx
        includes/ui/AddressSpaceMemoryEditor.hxx
)

target_link_libraries(
        gbemu
        imgui
        SDL2::SDL2
)

add_executable(gbemu_test
        srcs/hardware/core/SM83.cxx
        srcs/hardware/core/Disassembler.cxx
        srcs/hardware/core/Opcode.cxx
        srcs/hardware/Timer.cxx
        srcs/hardware/PPU.cxx
        srcs/hardware/Bus.cxx
        srcs/hardware/Cartridge.cxx
        srcs/Utils.cxx
        srcs/tests/hardware/core/BlarggInstructions.cxx
        srcs/tests/hardware/Cartridge.cxx
        srcs/tests/hardware/core/MooneyeAcceptance.cxx
        srcs/tests/hardware/core/Disassembler.cxx
        srcs/tests/hardware/FakeRAM.cxx
        srcs/tests/hardware/core/SM83.cxx
        includes/hardware/Displayable.hxx)

target_compile_definitions(gbemu_test PUBLIC ROMS_PATH="${CMAKE_SOURCE_DIR}/roms")

target_link_libraries(
        gbemu_test
        GTest::gtest_main
        faker-cxx
)

include(GoogleTest)
gtest_discover_tests(gbemu_test)