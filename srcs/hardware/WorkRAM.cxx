//
// Created by plouvel on 3/13/25.
//

#include "hardware/WorkRAM.hxx"

#include "Common.hxx"

Addressable::AddressableRange WorkRAM::get_addressable_range() const noexcept
{
    return {MemoryMap::WORK_RAM};
}

Addressable::AddressableRange FakeRAM::get_addressable_range() const noexcept
{
    return {std::make_pair(0x0000, 0xFFFF)};
}
