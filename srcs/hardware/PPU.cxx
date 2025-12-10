//
// Created by plouvel on 1/10/25.
//
#include "hardware/PPU.hxx"

#include <Utils.hxx>
#include <algorithm>
#include <cassert>
#include <stdexcept>

#include "graphics/Tile.hxx"
#include "hardware/core/SM83.hxx"

PPU::PPU(IAddressable& bus, IRenderer& renderer) : _bus(bus), _renderer(renderer)
{
    _oamEntriesToDraw.reserve(10);
}

uint8_t PPU::read(const uint16_t address) const
{
    if (Utils::addressIn(address, MemoryMap::VIDEO_RAM))
    {
        return _videoRam[address & 0x7FFF];
    }
    if (Utils::addressIn(address, MemoryMap::OAM))
    {
        return reinterpret_cast<const uint8_t*>(_oamEntries.data())[address - MemoryMap::OAM.first];
    }
    if (address == MemoryMap::IORegisters::LY)
    {
        return _registers.LY;
    }
    if (address == MemoryMap::IORegisters::LYC)
    {
        return _registers.LYC;
    }
    if (address == MemoryMap::IORegisters::SCY)
    {
        return _registers.SCY;
    }
    if (address == MemoryMap::IORegisters::SCX)
    {
        return _registers.SCX;
    }
    if (address == MemoryMap::IORegisters::WX)
    {
        return _registers.WX;
    }
    if (address == MemoryMap::IORegisters::WY)
    {
        return _registers.WY;
    }
    if (address == MemoryMap::IORegisters::BGP)
    {
        return _registers.BGP;
    }
    if (address == MemoryMap::IORegisters::STAT)
    {
        return _registers.STAT;
    }
    if (address == MemoryMap::IORegisters::LCDC)
    {
        return _registers.LCDC;
    }
    if (address == MemoryMap::IORegisters::OBP0)
    {
        return _registers.OBP0;
    }
    if (address == MemoryMap::IORegisters::OBP1)
    {
        return _registers.OBP1;
    }

    throw std::logic_error{"Invalid PPU Read"};
}

void PPU::write(const uint16_t address, uint8_t value)
{
    if (Utils::addressIn(address, MemoryMap::VIDEO_RAM))
    {
        _videoRam[address & 0x7FFF] = value;
    }
    else if (Utils::addressIn(address, MemoryMap::OAM))
    {
        reinterpret_cast<uint8_t*>(_oamEntries.data())[address - MemoryMap::OAM.first] = value;
    }
    else if (address == MemoryMap::IORegisters::LY)
    {
    }
    else if (address == MemoryMap::IORegisters::LYC)
    {
        _registers.LYC = value;
    }
    else if (address == MemoryMap::IORegisters::SCY)
    {
        _registers.SCY = value;
    }
    else if (address == MemoryMap::IORegisters::SCX)
    {
        _registers.SCX = value;
    }
    else if (address == MemoryMap::IORegisters::WX)
    {
        _registers.WX = value;
    }
    else if (address == MemoryMap::IORegisters::WY)
    {
        _registers.WY = value;
    }
    else if (address == MemoryMap::IORegisters::BGP)
    {
        _registers.BGP = value;
    }
    else if (address == MemoryMap::IORegisters::STAT)
    {
        _registers.STAT = value & 0b01111000;
    }
    else if (address == MemoryMap::IORegisters::LCDC)
    {
        _registers.LCDC = value;

        if (value & LCDControlFlags::LCDAndPPUEnable)
        {
            _transition(Mode::OAMScan);
        }
        else
        {
            _transition(Mode::Disabled);
        }
    }
    else if (address == MemoryMap::IORegisters::OBP0)
    {
        _registers.OBP0 = value;
    }
    else if (address == MemoryMap::IORegisters::OBP1)
    {
        _registers.OBP1 = value;
    }
    else
    {
        throw std::logic_error{"Invalid PPU Write"};
    }
}

void PPU::tick(const size_t machineCycle)
{
    using namespace Utils;

    for (size_t i{0}; i < machineCycle * 4; ++i)
    {
        _dots += 1;

        switch (_mode)
        {
            case Mode::OAMScan:
                _videoRamAccessible = true;
                _oamAccessible      = false;

                if (_dots == 80)
                {
                    for (auto oamEntry{_oamEntries.cbegin()}; oamEntry != _oamEntries.cend(); oamEntry++)
                    {
                        if (const auto objSize{_registers.LCDC & LCDControlFlags::ObjSize ? 16 : 8};
                            _registers.LY + 16 >= oamEntry->y && _registers.LY + 16 < oamEntry->y + objSize)
                        {
                            if (_oamEntriesToDraw.size() < 10)
                            {
                                _oamEntriesToDraw.emplace_back(oamEntry);
                            }
                        }
                    }

                    /*
                     * In Non-CGB mode, the smaller the X coordinate, the higher the priority. When X coordinates are
                     * identical, the object located first in OAM has higher priority.
                     * A stable sort preserves the original order of the OAM if two OAM entries X position are equal.
                     */
                    std::ranges::stable_sort(_oamEntriesToDraw, {}, &OAMEntry::x);

                    _transition(Mode::Drawing);
                }
                break;
            case Mode::Drawing:
                _videoRamAccessible = false;
                _oamAccessible      = false;

                /* Minimum length : 172 dots. */
                if (_dots == 252)
                {
                    _drawLine();
                    _transition(Mode::HorizontalBlank);
                }
                break;
            case Mode::HorizontalBlank:
                _videoRamAccessible = true;
                _oamAccessible      = true;

                if (_dots == 456)
                {
                    _dots = 0;

                    _registers.LY += 1;

                    if (_registers.LY == _registers.LYC)
                    {
                        if (_registers.STAT & Status::LYC)
                        {
                            _bus.write(MemoryMap::IORegisters::IF,
                                       _bus.read(MemoryMap::IORegisters::IF) | (1 << Interrupts::LCD));
                        }

                        _registers.STAT |= Status::LYCCompare;
                    }
                    else
                    {
                        _registers.STAT &= ~Status::LYCCompare;
                    }

                    if (_registers.LY == 144)
                    {
                        _transition(Mode::VerticalBlank);
                    }
                    else
                    {
                        _transition(Mode::OAMScan);
                    }
                }
                break;
            case Mode::VerticalBlank:
                _videoRamAccessible = true;
                _oamAccessible      = true;

                if (_dots == 456)
                {
                    _dots = 0;
                    _registers.LY += 1;

                    if (_registers.LY == 154)
                    {
                        _transition(Mode::OAMScan);
                    }
                }
                break;
            case Mode::Disabled:
                return;
        }
    }
}

void PPU::setPostBootRomRegisters()
{
    _registers.LCDC = 0x91;
    _registers.STAT = 0x85;
}

IAddressable::AddressableRange PPU::getAddressableRange() const noexcept
{
    return {MemoryMap::VIDEO_RAM,         MemoryMap::OAM,
            MemoryMap::IORegisters::SCX,  MemoryMap::IORegisters::SCY,
            MemoryMap::IORegisters::WX,   MemoryMap::IORegisters::WY,
            MemoryMap::IORegisters::BGP,  MemoryMap::IORegisters::LY,
            MemoryMap::IORegisters::LYC,  MemoryMap::IORegisters::STAT,
            MemoryMap::IORegisters::LCDC, MemoryMap::IORegisters::OBP0,
            MemoryMap::IORegisters::OBP1};
}

void PPU::_drawLine()
{
    bool hasWndPixel{};

    for (uint8_t x{0}; x < 160; ++x)
    {
        ObjPixel objPixel{_oamEntries.cend(), 0};
        BgPixel  bgPixel{false, 0};

        if (_registers.LCDC & LCDControlFlags::ObjEnable)
        {
            objPixel = _spriteFetch(x);
        }

        if (_registers.LCDC & LCDControlFlags::BGWindowEnableOrPriority)
        {
            bgPixel = _bgFetch(x);
        }

        if (bgPixel.first)
        {
            hasWndPixel = true;
        }

        _renderer.setPixel(x, _registers.LY, _pixelMixing(objPixel, bgPixel));
    }

    if (hasWndPixel)
    {
        _windowLineCounter += 1;
    }
}

PPU::ObjPixel PPU::_spriteFetch(const uint8_t x) const
{
    auto    objFetched{_oamEntries.cend()};
    uint8_t objPixel{};
    uint8_t objColOffset{};

    for (const auto oamEntryToDraw : _oamEntriesToDraw)
    {
        if (x + 8 >= oamEntryToDraw->x && x + 8 < oamEntryToDraw->x + 8)
        {
            uint8_t  objHeight{};
            uint8_t  rowOffset{};
            uint16_t tileDataAddress{};
            uint8_t  tileIndex{};
            uint8_t  objTileDataLow{};
            uint8_t  objTileDataHigh{};

            objFetched = oamEntryToDraw;

            if (_registers.LCDC & LCDControlFlags::ObjSize)
            {
                objHeight = 16;
                tileIndex = objFetched->tileIndex & 0xFE;
            }
            else
            {
                objHeight = 8;
                tileIndex = objFetched->tileIndex;
            }

            if (objFetched->yFlip)
            {
                /* Instead of fetching from the first line, fetch starting from the last line and advance
                 * backward. */

                rowOffset = ((objHeight * 2) - 2) - 2 * ((_registers.LY + 16 - objFetched->y) % objHeight);
            }
            else
            {
                rowOffset = 2 * ((_registers.LY + 16 - objFetched->y) % objHeight);
            }

            if (objFetched->xFlip)
            {
                /* Same logic as yFlip, but instead of fetching from the leftmost pixel, start from the
                 * rightmost, and advance backward. */

                objColOffset = (x + 8 - objFetched->x) % Graphics::TILE_SIZE;
            }
            else
            {
                objColOffset = 7 - (x + 8 - objFetched->x) % Graphics::TILE_SIZE;
            }

            tileDataAddress = tileIndex * 16 + rowOffset;
            objTileDataLow  = _videoRam[tileDataAddress];
            objTileDataHigh = _videoRam[tileDataAddress + 1];

            objPixel = (((objTileDataHigh >> objColOffset) & 1) << 1) | ((objTileDataLow >> objColOffset) & 1);

            /* Stop at the first (highest priority) object found for this pixel. */

            break;
        }
    }

    return {objFetched, objPixel};
}

PPU::BgPixel PPU::_bgFetch(uint8_t x) const
{
    uint8_t bgPixel{};
    bool    isWnd{};

    uint8_t tileNumber{};
    uint8_t bgTileDataLow{};
    uint8_t bgTileDataHigh{};
    uint8_t bgColOffset{};

    if (_registers.LCDC & LCDControlFlags::WindowEnable && _registers.WX != 0 && _registers.WY != 0 &&
        _registers.LY >= _registers.WY && x + 7 >= _registers.WX)
    {
        isWnd = true;

        {
            /* Get the tile number off the background tile map. */

            uint16_t windowTileMapAreaOffset{};
            uint8_t  colTileMap{};
            uint8_t  rowTileMap{};
            uint8_t  rowOffset{};
            uint8_t  colOffset{};

            if (_registers.LCDC & LCDControlFlags::WindowTileMapSelect)
            {
                windowTileMapAreaOffset = 0x1C00;
            }
            else
            {
                windowTileMapAreaOffset = 0x1800;
            }

            colOffset  = x - (_registers.WX - 7);
            rowOffset  = _windowLineCounter;
            colTileMap = colOffset / Graphics::TILE_SIZE;
            rowTileMap = rowOffset / Graphics::TILE_SIZE;

            tileNumber = _videoRam[windowTileMapAreaOffset + colTileMap + Graphics::TILE_MAP_SIZE * rowTileMap];
        }

        {
            /* Get tile data from video ram using the tile number. */

            uint16_t tileDataAddress{};
            size_t   rowOffset{};

            if (_registers.LCDC & LCDControlFlags::BGAndWindowTileDataArea)
            {
                tileDataAddress = tileNumber * Graphics::BYTES_PER_LINE;
            }
            else
            {
                tileDataAddress = 0x1000 + static_cast<int8_t>(tileNumber) * Graphics::BYTES_PER_LINE;
            }

            rowOffset       = 2 * (_windowLineCounter % Graphics::TILE_SIZE);
            tileDataAddress = tileDataAddress + rowOffset;

            bgTileDataLow  = _videoRam[tileDataAddress];
            bgTileDataHigh = _videoRam[tileDataAddress + 1];
        }

        bgColOffset = 7 - ((x - (_registers.WX - 7)) % Graphics::TILE_SIZE);

        bgPixel = (((bgTileDataHigh >> bgColOffset) & 1) << 1) | ((bgTileDataLow >> bgColOffset) & 1);
    }
    else
    {
        {
            /* Get the tile number off the background tile map. */

            uint16_t bgTileMapAreaOffset{};
            uint8_t  colTileMap{};
            uint8_t  rowTileMap{};
            uint8_t  rowOffset{};
            uint8_t  colOffset{};

            if (_registers.LCDC & LCDControlFlags::BGTileMapSelect)
            {
                bgTileMapAreaOffset = 0x1C00;
            }
            else
            {
                bgTileMapAreaOffset = 0x1800;
            }

            colOffset  = _registers.SCX + x;
            rowOffset  = _registers.SCY + _registers.LY;
            colTileMap = colOffset / Graphics::TILE_SIZE;
            rowTileMap = rowOffset / Graphics::TILE_SIZE;

            tileNumber = _videoRam[bgTileMapAreaOffset + colTileMap + Graphics::TILE_MAP_SIZE * rowTileMap];
        }

        {
            /* Get tile data from video ram using the tile number. */

            uint16_t tileDataAddress{};
            size_t   rowOffset{};

            if (_registers.LCDC & LCDControlFlags::BGAndWindowTileDataArea)
            {
                tileDataAddress = tileNumber * Graphics::BYTES_PER_LINE;
            }
            else
            {
                tileDataAddress = 0x1000 + static_cast<int8_t>(tileNumber) * Graphics::BYTES_PER_LINE;
            }

            rowOffset       = 2 * ((_registers.SCY + _registers.LY) % Graphics::TILE_SIZE);
            tileDataAddress = tileDataAddress + rowOffset;

            bgTileDataLow  = _videoRam[tileDataAddress];
            bgTileDataHigh = _videoRam[tileDataAddress + 1];
        }

        bgColOffset = 7 - ((x + _pixelsToDiscard) % Graphics::TILE_SIZE);

        bgPixel = (((bgTileDataHigh >> bgColOffset) & 1) << 1) | ((bgTileDataLow >> bgColOffset) & 1);
    }

    return {isWnd, bgPixel};
}

uint8_t PPU::_pixelMixing(const ObjPixel& objPixel, const BgPixel& bgPixel) const
{
    const uint8_t* finalPixel{};
    uint8_t        finalPalette{};

    if (objPixel.first != _oamEntries.cend())
    {
        if (objPixel.first->priority)
        {
            /* Background has priority over object. */
            if (bgPixel.second >= 0b01 && bgPixel.second <= 0b11)
            {
                finalPixel = &bgPixel.second;
            }
            /* But not when the background pixel is 0 (transparent). */
            else
            {
                finalPixel = &objPixel.second;
            }
        }
        else
        {
            if (objPixel.second == 0b00)
            {
                finalPixel = &bgPixel.second;
            }
            else
            {
                finalPixel = &objPixel.second;
            }
        }
    }
    else
    {
        finalPixel = &bgPixel.second;
    }

    if (finalPixel == &bgPixel.second)
    {
        finalPalette = _registers.BGP;
    }
    else
    {
        if (objPixel.first->dmgPalette)
        {
            finalPalette = _registers.OBP1;
        }
        else
        {
            finalPalette = _registers.OBP0;
        }
    }

    return finalPalette >> (2 * *finalPixel) & 0b11;
}

void PPU::_transition(const Mode transitionTo)
{
    auto modeValue{std::to_underlying(_mode)};

    if (transitionTo == Mode::Disabled)
    {
        modeValue     = 0;
        _dots         = 0;
        _registers.LY = 0;
    }

    /* Clear the first two bits and write the current PPU mode. */
    _registers.STAT = (_registers.STAT & 0xFC) | modeValue;

    if (transitionTo == Mode::OAMScan || transitionTo == Mode::HorizontalBlank || transitionTo == Mode::VerticalBlank)
    {
        //_triggerStatInterrupt((_registers.STAT & (1 << (2 + modeValue))) != 0);
    }

    if (_mode == Mode::OAMScan && transitionTo == Mode::Drawing)
    {
        _pixelsToDiscard = _registers.SCX & 0x7;
    }
    else if (_mode == Mode::Drawing && transitionTo == Mode::HorizontalBlank)
    {
        _oamEntriesToDraw.clear();
    }
    else if (_mode == Mode::HorizontalBlank && transitionTo == Mode::VerticalBlank)
    {
        _windowLineCounter = 0;

        _bus.write(MemoryMap::IORegisters::IF, _bus.read(MemoryMap::IORegisters::IF) | 1 << Interrupts::VBlank);

        _renderer.render();
    }
    else if (_mode == Mode::VerticalBlank && transitionTo == Mode::OAMScan)
    {
        _registers.LY = 0;
    }

    _mode = transitionTo;
}

void PPU::_triggerStatInterrupt(const bool value)
{
    if (value && !_irq)
    {
        _irq = true;
        _bus.write(MemoryMap::IORegisters::IF, _bus.read(MemoryMap::IORegisters::IF) | (1 << Interrupts::LCD));
    }
    else
    {
        _irq = false;
    }
}
