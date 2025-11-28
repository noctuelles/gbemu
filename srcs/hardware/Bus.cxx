//
// Created by plouvel on 12/10/24.
//

#include "hardware/Bus.hxx"

#include <format>
#include <utility>

#include "Common.hxx"
#include "Utils.hxx"

Bus::Bus(const EmulationState& emulationState) : emulationState(emulationState) {}

IAddressable::AddressableRange Bus::getAddressableRange() const noexcept
{
    return {std::make_pair(0x0000, 0xFFFF)};
}

std::array<uint8_t, 0x10000> Bus::getAddressSpace() const noexcept
{
    std::array<uint8_t, 0x10000> snapshot{};

    for (auto i = 0ULL; i < memoryMap.size(); i++)
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
            if (memoryMap[*memoryLocation] == nullptr)
            {
                memoryMap[*memoryLocation] = &addressable;
            }
        }
        else if (const auto memoryRange{std::get_if<AddressRange>(&range)}; memoryRange)
        {
            for (std::size_t address = memoryRange->first; address <= memoryRange->second; ++address)
            {
                if (memoryMap[address] == nullptr)
                {
                    memoryMap[address] = &addressable;
                }
            }
        }
    }
}

void Bus::write(const uint16_t address, const uint8_t value)
{
    if (emulationState.isInOamDma)
    {
        if (!Utils::addressIn(address, MemoryMap::HIGH_RAM))
        {
            return;
        }
    }

    if (memoryMap[address] == nullptr)
    {
        throw std::logic_error{std::format("Cannot perform bus write at {:#04x}.", address)};
    }

    memoryMap[address]->write(address, value);
}

uint8_t Bus::read(const uint16_t address) const
{
    if (emulationState.isInOamDma)
    {
        if (!Utils::addressIn(address, MemoryMap::HIGH_RAM))
        {
            return 0xFF;
        }
    }

    if (memoryMap[address] == nullptr)
    {
        throw std::logic_error{std::format("Cannot perform bus read at {:#04x}.", address)};
    }

    return memoryMap[address]->read(address);
}