//
// Created by plouvel on 1/10/25.
//
#include "PPU.hxx"

#include <Utils.hxx>
#include <stdexcept>

PPU::PPU(Addressable& bus) : bus(bus) {}

uint8_t PPU::read(const uint16_t address)
{
    if (utils::address_in(address, MemoryMap::VIDEO_RAM))
    {
        return video_ram[address - 0x8000];
    }
    if (utils::address_in(address, MemoryMap::OAM))
    {
        return reinterpret_cast<uint8_t*>(oam_entries.data())[address - 0xFE00];
    }

    throw std::logic_error{"Invalid PPU Read"};
}

void PPU::write(const uint16_t address, const uint8_t value)
{
    if (utils::address_in(address, MemoryMap::VIDEO_RAM))
    {
        video_ram[address - 0x8000] = value;
    }
    if (utils::address_in(address, MemoryMap::OAM))
    {
        reinterpret_cast<uint8_t*>(oam_entries.data())[address - 0xFE00] = value;
    }

    throw std::logic_error{"Invalid PPU Write"};
}

inline void PPU::tick()
{
    /* There is 4 dot per single M-cycle */

    switch (mode)
    {
        case Mode::OAMScan:
            break;
        case Mode::Drawing:
            break;
        case Mode::HorizontalBlank:
            break;
        case Mode::VerticalBlank:
            break;
    }
}
