//
// Created by plouvel on 3/13/25.
//

#include "hardware/WorkRAM.hxx"

#include "Common.hxx"

IAddressable::AddressableRange WorkRAM::getAddressableRange() const noexcept
{
    return {MemoryMap::WORK_RAM};
}

IAddressable::AddressableRange FakeRAM::getAddressableRange() const noexcept
{
    return {std::make_pair(0x0000, 0xFFFF)};
}
