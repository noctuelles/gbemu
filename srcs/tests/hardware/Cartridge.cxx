//
// Created by plouvel on 1/7/25.
//

#include "hardware/Cartridge.hxx"

#include <iostream>

#include "faker-cxx/faker.h"
#include "gtest/gtest.h"
#include "hardware/core/SM83.hxx"

TEST(Cartridge, InvalidFile)
{
    const std::filesystem::path path{faker::system::filePath()};

    ASSERT_THROW(Cartridge{path}, std::filesystem::filesystem_error);
}

TEST(Cardtridge, InvalidFileSize)
{
    const std::filesystem::path path{std::format("{}/test/invalid_size.gb", ROMS_PATH)};

    ASSERT_THROW(Cartridge{path}, std::runtime_error);
}

TEST(Cartridge, Test)
{
    const std::filesystem::path path{std::string{ROMS_PATH} + "/blargg/cpu_instrs/01-special.gb"};

    Cartridge          cart{path};
    SM83::Disassembler disassembler{static_cast<std::span<uint8_t>>(cart)};

    std::cout << cart.get_title() << std::endl;
    std::cout << cart.get_licensee() << std::endl;
}
