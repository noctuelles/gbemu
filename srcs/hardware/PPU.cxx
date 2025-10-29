//
// Created by plouvel on 1/10/25.
//
#include "../../includes/hardware/PPU.hxx"

#include <Utils.hxx>
#include <cassert>
#include <stdexcept>

#include "Utils.hxx"

PPU::PPU(Addressable& bus, Displayable& display) : bus(bus), display(display)
{
    obj_to_draw.reserve(10);
    transition(Mode::OAMScan);
}

uint8_t PPU::read(const uint16_t address) const
{
    if (utils::address_in(address, MemoryMap::VIDEO_RAM))
    {
        return video_ram[address - 0x8000];
    }
    if (utils::address_in(address, MemoryMap::OAM))
    {
        if (mode == Mode::OAMScan)
        {
            return 0xFF;
        }

        return reinterpret_cast<const uint8_t*>(oam_entries.data())[address - MemoryMap::OAM.first];
    }

    throw std::logic_error{"Invalid PPU Read"};
}

void PPU::write(const uint16_t address, const uint8_t value)
{
    if (utils::address_in(address, MemoryMap::VIDEO_RAM))
    {
        video_ram[address - 0x8000] = value;
    }
    else if (utils::address_in(address, MemoryMap::OAM))
    {
        if (mode == Mode::OAMScan)
        {
            return;
        }

        reinterpret_cast<uint8_t*>(oam_entries.data())[address - MemoryMap::OAM.first] = value;
    } else if (address == 0xFF41) {
        LY = 0;
    }
    else
    {
        throw std::logic_error{"Invalid PPU Write"};
    }
}

void PPU::tick()
{
    using namespace utils;

    switch (mode)
    {
        case Mode::OAMScan:
            if (dots % 2 == 0 && dots != 0)
            {
                /**
                 * During each scanline’s OAM scan, the PPU compares LY (using LCDC bit 2 to determine their size) to
                 * each object’s Y position to select up to 10 objects to be drawn on that line. The PPU scans OAM
                 * sequentially (from $FE00 to $FE9F), selecting the first (up to) 10 suitably-positioned objects. Since
                 * the PPU only checks the Y coordinate to select objects, even off-screen objects count towards the
                 * 10-objects-per-scanline limit. Merely setting an object’s X coordinate to X = 0 or X ≥ 168 (160 + 8)
                 * will hide it, but it will still count towards the limit, possibly causing another object later in OAM
                 * not to be drawn.
                 */
                if (const uint8_t obj_height = (LCDC & LCDControlFlags::ObjSize) != 0 ? 16 : 8;
                    LY >= curr_oam_entry->y && LY < curr_oam_entry->y + obj_height)
                {
                    if (obj_to_draw.size() < 10)
                    {
                        obj_to_draw.push_back(curr_oam_entry);
                    }
                }
                curr_oam_entry += 1;
            }

            if (dots == 80)
            {
                transition(Mode::Drawing);
            }
            break;
        case Mode::Drawing:
            /* TODO */
            x += 1;
            if (x == 160)
            {
                transition(Mode::HorizontalBlank);
            }
            break;
        case Mode::HorizontalBlank:
            /* Idle... */
            if (dots == 456)
            {
                dots = 0;
                LY += 1;

                if (LY == Displayable::HEIGHT)
                {
                    transition(Mode::VerticalBlank);
                }
                else
                {
                    transition(Mode::OAMScan);
                }
            }
            break;
        case Mode::VerticalBlank:
            /* Idle... */

            if (dots == 456)
            {
                dots = 0;
                LY += 1;

                if (LY == Displayable::HEIGHT + 10 - 1)
                {
                    LY = 0;
                    transition(Mode::OAMScan);
                }
            }
            break;
    }

    dots += 1;
}
void PPU::transition(const Mode transition_to)
{
    if (this->mode != Mode::OAMScan && transition_to == Mode::OAMScan)
    {
        obj_to_draw.clear();
        curr_oam_entry = oam_entries.cbegin();
    }

    this->mode = transition_to;
}
