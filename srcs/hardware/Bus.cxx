//
// Created by plouvel on 12/10/24.
//

#include "hardware/Bus.hxx"

#include <format>
#include <utility>

#include "Common.hxx"
#include "Utils.hxx"

Bus::Bus(const EmulationState& emulationState) : _emulationState(emulationState) {}

void Bus::loadBootRom(const std::array<uint8_t, 256>& bootRom) noexcept
{
    _bootRom = bootRom;
}

IAddressable::AddressableRange Bus::getAddressableRange() const noexcept
{
    return {std::make_pair(0x0000, 0xFFFF)};
}

void Bus::setPostBootRomRegisters()
{
    _bootRomMapped = 1;
}

std::array<uint8_t, 0x10000> Bus::getAddressSpace() const noexcept
{
    std::array<uint8_t, 0x10000> snapshot{};

    for (auto i = 0ULL; i < _memoryMap.size(); i++)
    {
        snapshot[i] = read(i);
    }

    return snapshot;
}

/**
 * @brief Attaches an Addressable object to the bus memory map.
 *
 * This method maps the addressable object's address or address range
 * to the bus' internal memory map. If an address within the specified
 * range is already mapped, it will not be overwritten.
 *
 * @param addressable A reference to an Addressable object, which contains
 *        addressable ranges that need to be mapped to the bus.
 */
void Bus::attach(IAddressable& addressable)
{
    for (const auto& range : addressable.getAddressableRange())
    {
        if (const auto memoryLocation{std::get_if<uint16_t>(&range)}; memoryLocation)
        {
            if (_memoryMap[*memoryLocation] == nullptr)
            {
                _memoryMap[*memoryLocation] = &addressable;
            }
        }
        else if (const auto memoryRange{std::get_if<AddressRange>(&range)}; memoryRange)
        {
            for (std::size_t address = memoryRange->first; address <= memoryRange->second; ++address)
            {
                if (_memoryMap[address] == nullptr)
                {
                    _memoryMap[address] = &addressable;
                }
            }
        }
    }
}

void Bus::write(const uint16_t address, const uint8_t value)
{
    if (_emulationState.isInOamDma)
    {
        if (!Utils::addressIn(address, MemoryMap::HIGH_RAM))
        {
            return;
        }
    }

    if (Utils::addressIn(address, MemoryMap::BOOT_ROM) && !_bootRomMapped)
    {
        _bootRom[address] = value;
        return;
    }

    if (address == MemoryMap::IORegisters::BOOTM)
    {
        _bootRomMapped = value;
        return;
    }

    if (_memoryMap[address] == nullptr)
    {
        throw std::logic_error{std::format("Cannot perform bus write at {:#04x}.", address)};
    }

    _memoryMap[address]->write(address, value);
}

uint8_t Bus::read(const uint16_t address) const
{
    if (_emulationState.isInOamDma)
    {
        if (!Utils::addressIn(address, MemoryMap::HIGH_RAM))
        {
            return 0xFF;
        }
    }

    if (Utils::addressIn(address, MemoryMap::BOOT_ROM) && !_bootRomMapped)
    {
        return _bootRom[address];
    }

    if (address == MemoryMap::IORegisters::BOOTM)
    {
        return _bootRomMapped;
    }

    if (_memoryMap[address] == nullptr)
    {
        throw std::logic_error{std::format("Cannot perform bus read at {:#04x}.", address)};
    }

    return _memoryMap[address]->read(address);
}