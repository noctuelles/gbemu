//
// Created by plouvel on 12/10/24.
//

#include "Bus.hxx"

#include <algorithm>

Bus::Bus()  = default;
Bus::~Bus() = default;

uint8_t Bus::read(const uint16_t addr) const
{
    return this->ram[addr];
}

void Bus::write(const uint16_t addr, const uint8_t data)
{
    this->ram[addr] = data;
}

void Bus::write(uint16_t addr, std::initializer_list<uint8_t> data)
{
    for (const auto& byte : data)
    {
        this->write(addr++, byte);
    }
}
