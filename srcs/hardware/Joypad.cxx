//
// Created by plouvel on 4/23/25.
//

#include "hardware/Joypad.hxx"

#include <stdexcept>

#include "Common.hxx"

void Joypad::write(const uint16_t address, const uint8_t value)
{
    if (address != MemoryMap::IORegisters::JOYPAD) [[unlikely]]
    {
        throw std::logic_error{"Invalid joypad write"};
    }

    [[unlikely]] (void) address;

    /* The lower nibble is read-only. */

    state = value & 0xF0;
}

Addressable::AddressableRange Joypad::get_addressable_range() const
{
    return {MemoryMap::IORegisters::JOYPAD};
}

void Joypad::press(const Button button)
{
    /* Rather unconventionally for the Game Boy, a button being pressed is seen as the corresponding bit being 0, not 1.
     */
    state &= ~std::to_underlying(button);
}

void Joypad::release(const Button button)
{
    state |= ~std::to_underlying(button);
}

uint8_t Joypad::read(const uint16_t address)
{
    if (address != MemoryMap::IORegisters::JOYPAD) [[unlikely]]
    {
        throw std::logic_error{"Invalid joypad read"};
    }

    /**
     * If neither buttons nor d-pad is selected ($30 was written), then the low nibble reads $F (all buttons released).
     */
    if ((state & 0x30) == 0x30)
    {
        return state | 0x0F;
    }

    return state;
}
