//
// Created by plouvel on 1/7/25.
//

#include "Cartridge.hxx"

#include "gtest/gtest.h"

TEST(Cartridge, Test)
{
    std::filesystem::path path{std::string{ROMS_PATH} + "/blargg/cpu_instrs/01-special.gb"};
    Cartridge cart{path};

    std::cout << cart.get_title() << std::endl;
    std::cout << cart.get_licensee() << std::endl;
    std::cout << cart.get_size()<< std::endl;
}
