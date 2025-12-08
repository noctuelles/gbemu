cmake_minimum_required(VERSION 3.20)

project(gbemu VERSION 1.0)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Boost REQUIRED)

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

        srcs/ui/MainWindow.cxx
        includes/ui/MainWindow.hxx
        srcs/ui/MainWindow.ui
        srcs/Emulator.cxx
        includes/Emulator.hxx
        srcs/main.cxx
        srcs/ui/Preference.cxx
        includes/ui/Preference.hxx
        srcs/ui/Preference.ui
        includes/ui/Settings.hxx
        srcs/ui/Settings.cxx
        srcs/ui/Debugger.cxx
        includes/ui/Debugger.hxx
        srcs/ui/Debugger.ui
        includes/tests/roms/BlarggInstructions.hxx
        includes/tests/roms/MooneyeAcceptance.hxx
        includes/QtRenderer.hxx
        includes/IRenderer.hxx
        srcs/QtRenderer.cxx
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