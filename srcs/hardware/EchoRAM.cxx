#include "hardware/EchoRAM.hxx"

#include "Common.hxx"

EchoRAM::EchoRAM(Addressable& ram) : content(ram) {}

uint8_t EchoRAM::read(const uint16_t address)
{
    return content.read(address);
}

void EchoRAM::write(const uint16_t address, const uint8_t value)
{
    content.write(address, value);
}

Addressable::AddressableRange EchoRAM::get_addressable_range() const noexcept
{
    return {MemoryMap::ECHO_RAM};
}