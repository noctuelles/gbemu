cmake_minimum_required(VERSION 3.20)

project(gbemu VERSION 1.0)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

include(FetchContent)
include(ExternalProject)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

find_package(Qt6 REQUIRED COMPONENTS Widgets)

include_directories(includes)

qt_standard_project_setup()

qt_add_executable(gbemu
        srcs/hardware/core/Disassembler.cxx
        srcs/hardware/core/Opcode.cxx
        srcs/hardware/core/SM83.cxx
        srcs/hardware/Bus.cxx
        srcs/hardware/Joypad.cxx
        srcs/hardware/Cartridge.cxx
        srcs/hardware/EchoRAM.cxx
        srcs/hardware/Joypad.cxx
        srcs/hardware/PPU.cxx
        srcs/hardware/Timer.cxx
        srcs/hardware/WorkRAM.cxx

        includes/graphics/Tile.hxx
        includes/hardware/core/SM83.hxx
        includes/hardware/Bus.hxx
        includes/hardware/Cartridge.hxx
        includes/hardware/EchoRAM.hxx
        includes/hardware/Joypad.hxx
        includes/hardware/PPU.hxx
        includes/hardware/Timer.hxx
        includes/hardware/WorkRAM.hxx

        includes/ui/MainWindow.hxx
        includes/ui/Preference.hxx
        includes/ui/Settings.hxx
        includes/ui/debugger/Debugger.hxx
        includes/ui/debugger/RegisterModel.hxx
        includes/ui/debugger/InstructionModel.hxx
        includes/ui/debugger/BreakpointDelegate.hxx
        includes/ui/debugger/InstructionListView.hxx

        includes/Emulator.hxx
        includes/IRenderer.hxx
        includes/QtRenderer.hxx

        srcs/ui/MainWindow.ui
        srcs/ui/Preference.ui
        srcs/ui/debugger/Debugger.ui

        srcs/ui/MainWindow.cxx
        srcs/ui/Preference.cxx
        srcs/ui/Settings.cxx
        srcs/ui/debugger/Debugger.cxx
        srcs/ui/debugger/RegisterModel.cxx
        srcs/ui/debugger/InstructionModel.cxx
        srcs/ui/debugger/BreakpointDelegate.cxx
        srcs/ui/debugger/InstructionListView.cxx

        srcs/QtRenderer.cxx
        srcs/Debugger.cxx
        srcs/Emulator.cxx
        srcs/main.cxx
)

add_executable(gbemu_test
        srcs/hardware/core/Disassembler.cxx
        srcs/hardware/core/Opcode.cxx
        srcs/hardware/core/SM83.cxx
        srcs/hardware/Bus.cxx
        srcs/hardware/Joypad.cxx
        srcs/hardware/Cartridge.cxx
        srcs/hardware/EchoRAM.cxx
        srcs/hardware/Joypad.cxx
        srcs/hardware/PPU.cxx
        srcs/hardware/Timer.cxx
        srcs/hardware/WorkRAM.cxx

        includes/graphics/Tile.hxx
        includes/hardware/core/SM83.hxx
        includes/hardware/Bus.hxx
        includes/hardware/Cartridge.hxx
        includes/hardware/EchoRAM.hxx
        includes/hardware/Joypad.hxx
        includes/hardware/PPU.hxx
        includes/hardware/Timer.hxx
        includes/hardware/WorkRAM.hxx

        srcs/tests/roms/BlarggInstructions.cxx
        srcs/tests/DummyComponent.cxx
        includes/tests/DummyComponent.hxx
        srcs/tests/roms/TestRom.cxx
        includes/tests/roms/TestRom.hxx
        srcs/tests/roms/MooneyeAcceptance.cxx
        srcs/tests/Utils.cxx
)

add_compile_options(-Wall -Wextra -Wpedantic -g)

target_compile_definitions(gbemu_test PUBLIC ROMS_PATH="${CMAKE_SOURCE_DIR}/roms")

target_link_libraries(gbemu PRIVATE
        Qt6::Widgets
)

target_link_libraries(gbemu_test PRIVATE
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(gbemu_test)