//
// Created by plouvel on 12/10/24.
//

#ifndef UTILS_HXX
#define UTILS_HXX

#include <cstdint>

#include "Common.hxx"

namespace Utils
{
    template <typename E>
    concept EnumType = std::is_enum_v<E> && std::is_same_v<std::underlying_type_t<E>, uint8_t>;

    constexpr auto wordLsb(const uint16_t x)
    {
        return static_cast<uint8_t>(x & 0xFF);
    }

    constexpr auto wordMsb(const uint16_t x)
    {
        return static_cast<uint8_t>((x & 0xFF00) >> 8);
    }

    constexpr auto to_word(const uint8_t msb, const uint8_t lsb)
    {
        return static_cast<uint16_t>(msb << 8 | lsb);
    }

    constexpr void to_bytes(const uint16_t word, uint8_t& msb, uint8_t& lsb)
    {
        msb = static_cast<uint8_t>(word >> 8);
        lsb = static_cast<uint8_t>(word & 0xFF);
    }

    constexpr bool addressIn(const uint16_t addr, const MemoryMap::AddressRange& range)
    {
        return addr >= range.first && addr <= range.second;
    }

    inline uint8_t reverseByte(uint8_t byte)
    {
        byte = byte >> 4 | byte << 4;
        byte = (byte & 0xCC) >> 2 | (byte & 0x33) << 2;
        byte = (byte & 0xAA) >> 1 | (byte & 0x55) << 1;
        return byte;
    }

    template <EnumType Enum>
    constexpr std::underlying_type_t<Enum> operator&(const std::underlying_type_t<Enum> lhs, const Enum rhs)
    {
        return lhs & std::to_underlying(rhs);
    }

    template <EnumType Enum>
    constexpr std::underlying_type_t<Enum> operator|(const std::underlying_type_t<Enum> lhs, const Enum rhs)
    {
        return lhs | std::to_underlying(rhs);
    }

}  // namespace Utils

#endif  // UTILS_HXX
