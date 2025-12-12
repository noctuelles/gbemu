//
// Created by plouvel on 1/10/25.
//

#ifndef COMMON_HXX
#define COMMON_HXX

#include <cstdint>
#include <utility>

namespace Interrupts
{
    static constexpr uint8_t VBlank = 0x00;
    static constexpr uint8_t LCD    = 0x01;
    static constexpr uint8_t Timer  = 0x04;
    static constexpr uint8_t Serial = 0x08;
    static constexpr uint8_t Joypad = 0x10;
};  // namespace Interrupts

enum class Key : uint8_t
{
    A,
    B,
    Select,
    Start,
    Right,
    Left,
    Up,
    Down
};

using AddressSpace = std::array<uint8_t, 0x10000>;

namespace MemoryMap
{
    using AddressRange = std::pair<uint16_t, uint16_t>;

    constexpr AddressRange BOOT_ROM{0x0000, 0x00FF};
    constexpr AddressRange ROM{0x0000, 0x7FFF};
    constexpr AddressRange VIDEO_RAM{0x8000, 0x9FFF};
    constexpr AddressRange EXT_RAM{0xA000, 0xBFFF};
    constexpr AddressRange WORK_RAM{0xC000, 0xDFFF};
    constexpr AddressRange ECHO_RAM{0xE000, 0xFDFF};
    constexpr AddressRange OAM{0xFE00, 0xFE9F};
    constexpr AddressRange NOT_USABLE{0xFEA0, 0xFEFF};

    constexpr AddressRange IO_REGISTERS{0xFF00, 0xFF7F};

    namespace IORegisters
    {
        constexpr uint16_t JOYPAD{0xFF00};
        constexpr uint16_t SB{0xFF01};
        constexpr uint16_t SC{0xFF02};
        constexpr uint16_t DIV{0xFF04};
        constexpr uint16_t TIMA{0xFF05};
        constexpr uint16_t TMA{0xFF06};
        constexpr uint16_t TAC{0xFF07};
        constexpr uint16_t IF{0xFF0F};

        constexpr uint16_t LCDC{0xFF40};
        constexpr uint16_t LYC{0xFF45};
        constexpr uint16_t STAT{0xFF41};
        constexpr uint16_t SCY{0xFF42};
        constexpr uint16_t SCX{0xFF43};
        constexpr uint16_t LY{0xFF44};
        constexpr uint16_t DMA{0xFF46};
        constexpr uint16_t BGP{0xFF47};
        constexpr uint16_t OBP0{0xFF48};
        constexpr uint16_t OBP1{0xFF49};
        constexpr uint16_t WY{0xFF4A};
        constexpr uint16_t WX{0xFF4B};
        constexpr uint16_t BOOTM{0xFF50};

    }  // namespace IORegisters

    constexpr AddressRange HIGH_RAM{0xFF80, 0xFFFE};

    constexpr uint16_t IE{0xFFFF};
}  // namespace MemoryMap

#endif  // COMMON_HXX
