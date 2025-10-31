//
// Created by plouvel on 4/23/25.
//

#include "tests/hardware/FakeRAM.hxx"

uint8_t FakeRAM::read(const uint16_t address) override
{
    return content[address];
}

void FakeRAM::write(const uint16_t address, const uint8_t value) override
{
    content[address] = value;
}

[[nodiscard]] Addressable::AddressableRange FakeRAM::getAddressableRange() const noexcept override
{
    return {std::make_pair(0x0000, 0xFFFF)};
}
