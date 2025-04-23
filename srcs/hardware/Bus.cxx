//
// Created by plouvel on 12/10/24.
//

#include "hardware/Bus.hxx"

#include <utility>

Addressable::AddressableRange Bus::get_addressable_range() const noexcept
{
    return {std::make_pair(0x0000, 0xFFFF)};
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
void Bus::attach(Addressable& addressable)
{
    for (const auto& range : addressable.get_addressable_range())
    {
        if (const auto memoryLocation{std::get_if<uint16_t>(&range)}; memoryLocation)
        {
            if (memory_map[*memoryLocation] == nullptr)
            {
                memory_map[*memoryLocation] = &addressable;
            }
        }
        else if (const auto memoryRange{std::get_if<AddressRange>(&range)}; memoryRange)
        {
            for (std::size_t address = memoryRange->first; address <= memoryRange->second; ++address)
            {
                if (memory_map[address] == nullptr)
                {
                    memory_map[address] = &addressable;
                }
            }
        }
    }
}

void Bus::write(const uint16_t address, const uint8_t value)
{
    if (memory_map[address] == nullptr)
    {
        throw std::logic_error{std::format("Cannot perform bus write at {:#04x}.", address)};
    }

    memory_map[address]->write(address, value);
}

uint8_t Bus::read(const uint16_t address)
{
    if (memory_map[address] == nullptr)
    {
        throw std::logic_error{std::format("Cannot perform bus read at {:#04x}.", address)};
    }
    return memory_map[address]->read(address);
}