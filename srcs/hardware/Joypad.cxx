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

    _selectButtons    = value & 0x20;
    _selectDirections = value & 0x10;
}

Addressable::AddressableRange Joypad::getAddressableRange() const noexcept
{
    return {MemoryMap::IORegisters::JOYPAD};
}

void Joypad::press(const Button button)
{
    _state &= ~(1 << std::to_underlying(button));
}

void Joypad::release(const Button button)
{
    _state |= (1 << std::to_underlying(button));
}

uint8_t Joypad::read(const uint16_t address) const
{
    if (address != MemoryMap::IORegisters::JOYPAD) [[unlikely]]
    {
        throw std::logic_error{"Invalid joypad read"};
    }

    uint8_t keyValue{};

    if (_selectButtons && _selectDirections)
    {
        keyValue = 0xF;
    }
    else if (!_selectButtons && !_selectDirections)
    {
        keyValue = ((_state & 0xF0) >> 4) & (_state & 0xF);
    }
    else if (!_selectButtons && _selectDirections)
    {
        keyValue = _state & 0x0F;
    }
    else if (_selectButtons && !_selectDirections)
    {
        keyValue = ((_state & 0xF0) >> 4);
    }

    return 0xC0 | (_selectButtons | _selectDirections) | keyValue;
}
