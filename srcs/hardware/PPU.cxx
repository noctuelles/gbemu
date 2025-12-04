//
// Created by plouvel on 1/10/25.
//
#include "hardware/PPU.hxx"

#include <Utils.hxx>
#include <iostream>
#include <stdexcept>

#include "graphics/Tile.hxx"
#include "hardware/core/SM83.hxx"

PPU::PPU(IAddressable& bus) : _bus(bus)
{
    _objsToDraw.reserve(10);

    addrToRegister = {
        {MemoryMap::IORegisters::LY, _registers.LY},     {MemoryMap::IORegisters::LYC, _registers.LYC},
        {MemoryMap::IORegisters::LCDC, _registers.LCDC}, {MemoryMap::IORegisters::STAT, _registers.STAT},
        {MemoryMap::IORegisters::SCX, _registers.SCX},   {MemoryMap::IORegisters::SCY, _registers.SCY},
        {MemoryMap::IORegisters::WX, _registers.WX},     {MemoryMap::IORegisters::WY, _registers.WY},
        {MemoryMap::IORegisters::BGP, _registers.BGP},
    };
}

PPU::PixelFetcher::PixelFetcher(std::queue<FIFOEntry>& backgroundFifo, const VideoRAM& videoRam, Registers& registers)
    : _backgroundFIFO(backgroundFifo), _registers(registers), _videoRam{videoRam}, _dots(0)
{
}

void PPU::PixelFetcher::tick()
{
    constexpr auto isBackground{true};

    /* Just rendering background tiles. */
    switch (_state)
    {
        case State::GetTile:
            /* Get tile number. */
            if (_dots == 2)
            {
                if (isBackground)
                {
                    size_t tileOffset{};
                    size_t bgTileMapAreaOffset{};
                    /* The background tile map is a 32x32 tile grid. */

                    if ((_registers.LCDC & LCDControlFlags::BGTileMapSelect) == 0)
                    {
                        bgTileMapAreaOffset = 0x1800;
                    }
                    else
                    {
                        bgTileMapAreaOffset = 0x1C00;
                    }

                    tileOffset = ((_registers.SCX >> 3) + _x) & 0x1F;
                    tileOffset += 32 * (((_registers.LY + _registers.SCY) & 0xFF) >> 3);

                    _tileMapNbr = _videoRam[bgTileMapAreaOffset + tileOffset];
                }

                _state = State::GetTileDataLow;
            }
            break;
        case State::GetTileDataLow:
            if (_dots == 4)
            {
                _tileDataAddress = 0;

                /* Select the proper tile. */
                _tileDataAddress += _tileMapNbr * 16;

                /* Select the proper row within that tile. There are 2 bytes per row. */
                _tileDataAddress += 2 * ((_registers.LY + _registers.SCY) & 0x7);
                _tileDataLow = _videoRam[_tileDataAddress];

                _state = State::GetTileDataHigh;
            }
            break;
        case State::GetTileDataHigh:
            if (_dots == 6)
            {
                _tileDataHigh = _videoRam[_tileDataAddress + 1];
                _state        = State::Sleep;
            }
            break;
        case State::Sleep:
            if (_dots == 8)
            {
                _state = State::Push;
            }
            break;
        case State::Push:
            if (_backgroundFIFO.empty())
            {
                /* If the tile is flipped horizontally? */

                for (ssize_t i = 7; i >= 0; --i)
                {
                    const uint8_t value = (((_tileDataLow >> i) & 1) << 1) | ((_tileDataHigh >> i) & 1);
                    _backgroundFIFO.emplace(FIFOEntry{value, 0, false});
                }

                _state = State::GetTile;
                _dots  = 0;
                _x += 1;
            }
            break;
    }

    _dots += 1;
}

void PPU::PixelFetcher::start()
{
    _x     = 0;
    _dots  = 0;
    _state = State::GetTile;
}

uint8_t PPU::read(const uint16_t address) const
{
    if (Utils::addressIn(address, MemoryMap::VIDEO_RAM))
    {
        // if (!_videoRamAccessible)
        // {
        //     return 0xFF;
        // }

        if ((_registers.LCDC & LCDControlFlags::LCDAndPPUEnable) == 1)
        {
            return 0xFF;
        }

        return _videoRam[address - 0x8000];
    }
    if (Utils::addressIn(address, MemoryMap::OAM))
    {
        // if (!_oamAccessible)
        // {
        //     return 0xFF;
        // }

        return reinterpret_cast<const uint8_t*>(_oamEntries.data())[address - MemoryMap::OAM.first];
    }

    if (const auto it = addrToRegister.find(address); it != addrToRegister.end())
    {
        return it->second;
    }

    throw std::logic_error{"Invalid PPU Read"};
}

void PPU::write(const uint16_t address, const uint8_t value)
{
    if (Utils::addressIn(address, MemoryMap::VIDEO_RAM))
    {
        if (!_videoRamAccessible)
        {
            return;
        }

        _videoRam[address - 0x8000] = value;
    }
    else if (Utils::addressIn(address, MemoryMap::OAM))
    {
        if (!_oamAccessible)
        {
            return;
        }

        reinterpret_cast<uint8_t*>(_oamEntries.data())[address - MemoryMap::OAM.first] = value;
    }
    else if (const auto it = addrToRegister.find(address); it != addrToRegister.end())
    {
        it->second.get() = value;

        if (it->second == _registers.LCDC)
        {
            if ((_registers.LCDC & LCDControlFlags::LCDAndPPUEnable) == 0)
            {
                /* The mode report 0 when the PPU is disabled. */

                _dots         = 0;
                _registers.LY = 0;
                _x            = 0;
                _mode         = Mode::Disabled;
            }
            else
            {
                _mode = Mode::OAMScan;
            }
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

                if (_dots % 2 == 0 && _dots != 0)
                {
                    /**
                     * During each scanline’s OAM scan, the PPU compares LY (using LCDC bit 2 to determine their size)
                     * to each object’s Y position to select up to 10 objects to be drawn on that line. The PPU scans
                     * OAM sequentially (from $FE00 to $FE9F), selecting the first (up to) 10 suitably-positioned
                     * objects. Since the PPU only checks the Y coordinate to select objects, even off-screen objects
                     * count towards the 10-objects-per-scanline limit. Merely setting an object’s X coordinate to X = 0
                     * or X ≥ 168 (160 + 8) will hide it, but it will still count towards the limit, possibly causing
                     * another object later in OAM not to be drawn.
                     */
                    //if (const uint8_t objHeight = (_registers.LCDC & LCDControlFlags::ObjSize) != 0 ? 16 : 8;
                    //    _registers.LY >= _currentOamEntry->y && _registers.LY < _currentOamEntry->y + objHeight)
                    //{
                    //    if (_objsToDraw.size() < 10)
                    //    {
                    //        _objsToDraw.push_back(_currentOamEntry);
                    //    }
                    //}

                    //_currentOamEntry += 1;
                }

                _dots += 1;
                if (_dots == 80)
                {
                    _transition(Mode::Drawing);
                }
                break;
            case Mode::Drawing:
                /* Minimum length : 172 dots. */

                _videoRamAccessible = false;
                _oamAccessible      = false;

                _pixelFetcher.tick();

                if (!_backgroundFIFO.empty())
                {
                    if (_pixelsToDiscard != 0)
                    {
                        _pixelsToDiscard -= 1;
                    }
                    else
                    {
                        const auto& backgroundPixel{_backgroundFIFO.front()};

                        _framebuffer[_registers.LY][_x] =
                            Graphics::getRealColorIndexFromPaletteRegister(backgroundPixel.color, _registers.BGP);
                        ;
                        _x += 1;
                    }

                    _backgroundFIFO.pop();
                }

                _dots += 1;
                if (_x == 160)
                {
                    _transition(Mode::HorizontalBlank);
                }
                break;
            case Mode::HorizontalBlank:
                _videoRamAccessible = true;
                _oamAccessible      = true;

                _dots += 1;
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

                _dots += 1;
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

void PPU::_transition(const Mode transitionTo)
{
    const auto modeValue{std::to_underlying(_mode)};

    /* Clear the first two bits and write the current PPU mode. */
    _registers.STAT = (_registers.STAT & 0xFC) | modeValue;

    /* There is no Mode 3 interrupt. */
    if (transitionTo != Mode::Drawing)
    {
        // triggerStatInterrupt((registers.STAT & (1 << (2 + modeValue))) != 0);
    }

    if (_mode != Mode::OAMScan && transitionTo == Mode::OAMScan)
    {
        _objsToDraw.clear();
        _currentOamEntry = _oamEntries.cbegin();
    }
    if (_mode == Mode::OAMScan && transitionTo == Mode::Drawing)
    {
        decltype(_backgroundFIFO) emptyBackgroundFifo{};

        std::swap(_backgroundFIFO, emptyBackgroundFifo);
        _pixelFetcher.start();

        _pixelsToDiscard = _registers.SCX & 0x7;
    }
    else if (_mode == Mode::Drawing && transitionTo == Mode::HorizontalBlank)
    {
        _x = 0;
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
