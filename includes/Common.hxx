//
// Created by plouvel on 1/10/25.
//

#ifndef COMMON_HXX
#define COMMON_HXX

#include <cstdint>
#include <utility>

namespace MemoryMap
{
    using AddressRange = std::pair<uint16_t, uint16_t>;

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
    }  // namespace IORegisters

    constexpr AddressRange HIGH_RAM{0xFF80, 0xFFFE};

    constexpr uint16_t IE{0xFFFF};
}  // namespace MemoryMap

#endif  // COMMON_HXX
