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

    addrToRegister = {
        {MemoryMap::IORegisters::LY, registers.LY},     {MemoryMap::IORegisters::LYC, registers.LYC},
        {MemoryMap::IORegisters::LCDC, registers.LCDC}, {MemoryMap::IORegisters::STAT, registers.STAT},
        {MemoryMap::IORegisters::SCX, registers.SCX},   {MemoryMap::IORegisters::SCY, registers.SCY},
        {MemoryMap::IORegisters::WX, registers.WX},     {MemoryMap::IORegisters::WY, registers.WY},
        {MemoryMap::IORegisters::BGP, registers.BGP},
    };
}

PPU::PixelFetcher::PixelFetcher(Addressable& bus, Registers& registers) : registers(registers), bus(bus) {}

void PPU::PixelFetcher::tick()
{
    constexpr auto isBackground{true};

    /* Just rendering background tiles. */
    switch (state)
    {
        case State::GetTile:
            /* Get tile number. */
            if (dots == 2)
            {
                size_t tileOffset{x};

                if (isBackground)
                {
                    constexpr uint16_t tileAddress{0x9800};
                    tileOffset += registers.SCX >> 3 & 0x1F;
                    tileOffset += 32 * ((registers.LY + registers.SCY & 0xFF) >> 3);

                    tileMapNbr = bus.read(tileAddress + tileOffset);
                }

                state = State::GetTileDataLow;
            }
            break;
        case State::GetTileDataLow:
            if (dots == 4)
            {
                if ((registers.LCDC & LCDControlFlags::BGAndWindowTileDataArea) == 0)
                {
                    tileDataAddress = 0x8800;
                }
                else
                {
                    tileDataAddress = 0x8000;
                }

                tileDataAddress = tileDataAddress + 2 * (registers.LY + registers.SCY & 0xF);
                tileDataLow     = bus.read(tileDataAddress);
                state           = State::GetTileDataHigh;
            }
            break;
        case State::GetTileDataHigh:
            if (dots == 6)
            {
                tileDataHigh = bus.read(tileDataAddress + 1);
                state        = State::Sleep;
            }
            break;
        case State::Sleep:
            if (dots == 8)
            {
                state = State::Push;
            }
            break;
        case State::Push:
            /* Attempted every dot until it succeed. */
            state = State::GetTile;
            dots  = 0;
            break;
    };
    dots += 1;
}

void PPU::PixelFetcher::start()
{
    while (!backgroundFIFO.empty())
    {
        backgroundFIFO.pop();
    }
}

uint8_t PPU::read(const uint16_t address) const
{
    if (utils::address_in(address, MemoryMap::VIDEO_RAM))
    {
        if ((registers.LCDC & LCDControlFlags::LCDAndPPUEnable) == 1)
        {
            return 0xFF;
        }

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

    if (const auto it = addrToRegister.find(address); it != addrToRegister.end())
    {
        return it->second;
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
    }

    if (const auto it = addrToRegister.find(address); it != addrToRegister.end())
    {
        it->second.get() = value;
    }

    throw std::logic_error{"Invalid PPU Write"};
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
                if (const uint8_t obj_height = (registers.LCDC & LCDControlFlags::ObjSize) != 0 ? 16 : 8;
                    registers.LY >= curr_oam_entry->y && registers.LY < curr_oam_entry->y + obj_height)
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
                pixelFetcher.start();
                transition(Mode::Drawing);
            }
            break;
        case Mode::Drawing:
            /* TODO */
            pixelFetcher.tick();

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
                registers.LY += 1;

                if (registers.LY == Displayable::HEIGHT)
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
                registers.LY += 1;

                if (registers.LY == Displayable::HEIGHT + 10 - 1)
                {
                    registers.LY = 0;
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
