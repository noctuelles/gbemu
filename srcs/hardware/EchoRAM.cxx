#include "hardware/EchoRAM.hxx"

#include "Common.hxx"
#include "hardware/WorkRAM.hxx"

EchoRAM::EchoRAM(IAddressable& workRam) : _workRam(workRam) {}

uint8_t EchoRAM::read(const uint16_t address) const
{
    return _workRam.read(getRealAddress(address));
}

void EchoRAM::write(const uint16_t address, const uint8_t value)
{
    _workRam.write(getRealAddress(address), value);
}

IAddressable::AddressableRange EchoRAM::getAddressableRange() const noexcept
{
    return {MemoryMap::ECHO_RAM};
}

uint16_t EchoRAM::getRealAddress(const uint16_t address)
{
    return MemoryMap::WORK_RAM.first + static_cast<uint16_t>(address & 0x1FFF);
}