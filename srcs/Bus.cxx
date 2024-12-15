//
// Created by plouvel on 12/10/24.
//

#include "Bus.hxx"

Bus::Bus()
    : memory()
{}

Bus::~Bus() = default;

uint8_t Bus::read(const uint16_t addr) const {
    return this->memory.at(addr);
}

void Bus::write(uint16_t addr, uint8_t data)
{
    this->memory.at(addr) = data;
}

void Bus::write(uint16_t addr, std::initializer_list<uint8_t> data)
{
    for (const auto &byte : data)
    {
        this->write(addr++, byte);
    }
}
