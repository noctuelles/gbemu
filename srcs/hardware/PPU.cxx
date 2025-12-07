//
// Created by plouvel on 1/10/25.
//
#include "hardware/PPU.hxx"

#include <Utils.hxx>
#include <stdexcept>

#include "graphics/Tile.hxx"
#include "hardware/core/SM83.hxx"

PPU::PPU(IAddressable& bus) : _bus(bus)
{
    _objsToDraw.reserve(10);
}

uint8_t PPU::read(const uint16_t address) const
{
    if (Utils::addressIn(address, MemoryMap::VIDEO_RAM))
    {
        return _videoRam[address - 0x8000];
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

    throw std::logic_error{"Invalid PPU Read"};
}

void PPU::write(const uint16_t address, uint8_t value)
{
    if (Utils::addressIn(address, MemoryMap::VIDEO_RAM))
    {
        _videoRam[address - 0x8000] = value;
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

        if (_registers.LY == _registers.LYC)
        {
            _registers.STAT |= Status::LYCCompare;
        }
        else
        {
            _registers.STAT &= ~Status::LYCCompare;
        }

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
                            if (_objsToDraw.size() < 10)
                            {
                                _objsToDraw.emplace_back(oamEntry);
                            }
                        }
                    }
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

const Graphics::Framebuffer& PPU::getFramebuffer() const noexcept
{
    return _framebuffer;
}

IAddressable::AddressableRange PPU::getAddressableRange() const noexcept
{
    return {MemoryMap::VIDEO_RAM,        MemoryMap::OAM,
            MemoryMap::IORegisters::SCX, MemoryMap::IORegisters::SCY,
            MemoryMap::IORegisters::WX,  MemoryMap::IORegisters::WY,
            MemoryMap::IORegisters::BGP, MemoryMap::IORegisters::LY,
            MemoryMap::IORegisters::LYC, MemoryMap::IORegisters::STAT,
            MemoryMap::IORegisters::LCDC};
}

void PPU::_drawLine()
{
    uint8_t bgTileDataLow{};
    uint8_t bgTileDataHigh{};

    uint8_t objTileDataLow{};
    uint8_t objTileDataHigh{};

    for (uint8_t x{0}; x < 160; ++x)
    {
        uint8_t pixelOffset{};
        uint8_t bgPixel{};
        uint8_t objPixel{};
        uint8_t finalPixel{};
        auto    objFetched{_oamEntries.cend()};

        if (_registers.LCDC & LCDControlFlags::ObjEnable)
        {
            for (const auto& objToDraw : _objsToDraw)
            {
                if (x + 8 >= objToDraw->x && x + 8 < objToDraw->x + 8)
                {
                    const auto rowOffset{2 * (_registers.LY % Graphics::TILE_SIZE)};
                    uint16_t   tileDataAddress{};

                    tileDataAddress = objToDraw->tileIndex * 16 + rowOffset;
                    objTileDataLow  = _videoRam[tileDataAddress];
                    objTileDataHigh = _videoRam[tileDataAddress + 1];

                    objFetched = objToDraw;

                    /* Stop at the first (highest priority) object found for this pixel. */
                    break;
                }
            }
        }

        if (_registers.LCDC & LCDControlFlags::BGWindowEnableOrPriority)
        {
            /* Load tile data every 8 pixels. */
            if (x % 8 == 0)
            {
                uint8_t tileNumber{};

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
            }
        }

        pixelOffset = 7 - (x % Graphics::TILE_SIZE);

        objPixel = (((objTileDataHigh >> pixelOffset) & 1) << 1) | ((objTileDataLow >> pixelOffset) & 1);
        bgPixel  = (((bgTileDataHigh >> pixelOffset) & 1) << 1) | ((bgTileDataLow >> pixelOffset) & 1);

        /* Pixel mixing. */

        if (objFetched != _oamEntries.cend())
        {
            if (objFetched->priority)
            {
                /* Background has priority over object. */
                if (bgPixel >= 0b01 && bgPixel <= 0b11)
                {
                    finalPixel = bgPixel;
                }
                else
                {
                    finalPixel = objPixel;
                }
            }
            else
            {
                finalPixel = objPixel;
            }
        }
        else
        {
            finalPixel = bgPixel;
        }

        _framebuffer[_registers.LY][x] = _registers.BGP >> (2 * finalPixel) & 0b11;
    }
}

void PPU::_transition(const Mode transitionTo)
{
    auto modeValue{std::to_underlying(_mode)};

    if (transitionTo == Mode::Disabled)
    {
        modeValue     = 0;
        _dots         = 0;
        _registers.LY = 0;
        _x            = 0;
    }

    /* Clear the first two bits and write the current PPU mode. */
    _registers.STAT = (_registers.STAT & 0xFC) | modeValue;

    if (transitionTo == Mode::OAMScan || transitionTo == Mode::HorizontalBlank || transitionTo == Mode::VerticalBlank)
    {
        //_triggerStatInterrupt((_registers.STAT & (1 << (2 + modeValue))) != 0);
    }

    if (_mode != Mode::OAMScan && transitionTo == Mode::OAMScan)
    {
        _objsToDraw.clear();
        _currentOamEntry = _oamEntries.cbegin();
    }
    if (_mode == Mode::OAMScan && transitionTo == Mode::Drawing)
    {
        _pixelsToDiscard = _registers.SCX & 0x7;
    }
    else if (_mode == Mode::Drawing && transitionTo == Mode::HorizontalBlank)
    {
        _x = 0;
        _objsToDraw.clear();
    }
    else if (_mode == Mode::HorizontalBlank && transitionTo == Mode::VerticalBlank)
    {
        _bus.write(MemoryMap::IORegisters::IF, _bus.read(MemoryMap::IORegisters::IF) | 1 << Interrupts::VBlank);
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

void PPU::off()
{
    _registers.STAT = _registers.STAT & 0xFC;
}
