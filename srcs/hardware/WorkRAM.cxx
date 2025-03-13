//
// Created by plouvel on 3/13/25.
//

#include "hardware/WorkRAM.hxx"

#include "Common.hxx"

uint8_t WorkRAM::read(const uint16_t address)
{
    return content[address & 0x1FFF];
}

void WorkRAM::write(const uint16_t address, const uint8_t value)
{
    content[address & 0x1FFF] = value;
}

Addressable::AddressableRange WorkRAM::get_addressable_range() const
{
    return {MemoryMap::WORK_RAM};
}