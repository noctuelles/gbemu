//
// Created by plouvel on 12/10/24.
//

#include "Bus.hxx"

#include <utility>

void Bus::write(uint16_t address, uint8_t value)
{
    ram[address] = std::byte{value};
}

uint8_t Bus::read(uint16_t address)
{
    return std::to_underlying(ram[address]);
}