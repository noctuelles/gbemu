#include "hardware/EchoRAM.hxx"

#include "Common.hxx"

EchoRAM::EchoRAM(Addressable& ram) : content(ram) {}

uint8_t EchoRAM::read(const uint16_t address) const
{
    return content.read(address - MemoryMap::ECHO_RAM.first);
}

void EchoRAM::write(const uint16_t address, const uint8_t value)
{
    content.write(address - MemoryMap::ECHO_RAM.second, value);
}

Addressable::AddressableRange EchoRAM::getAddressableRange() const noexcept
{
    return {MemoryMap::ECHO_RAM};
}