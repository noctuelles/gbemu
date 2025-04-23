//
// Created by plouvel on 1/7/25.
//

#include "hardware/Cartridge.hxx"

#include <iostream>

#include "gtest/gtest.h"
#include "hardware/core/SM83.hxx"

TEST(Cartridge, Test)
{
    const std::filesystem::path path{std::string{ROMS_PATH} + "/blargg/cpu_instrs/01-special.gb"};

    Cartridge          cart{path};
    SM83::Disassembler disassembler{static_cast<std::span<uint8_t>>(cart)};

    std::cout << cart.get_title() << std::endl;
    std::cout << cart.get_licensee() << std::endl;
}
