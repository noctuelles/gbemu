//
// Created by plouvel on 12/10/24.
//

#ifndef UTILS_HXX
#define UTILS_HXX

#include <cstdint>

constexpr uint8_t u16_lsb(const uint16_t x)
{
    return static_cast<uint8_t>(x & 0xFF);
}

constexpr uint8_t u16_msb(const uint16_t x)
{
    return static_cast<uint8_t>((x & 0xFF00) >> 8);
}

constexpr uint16_t u8_to_u16(const std::pair<uint8_t, uint8_t>&& msb_lsb)
{
    return static_cast<uint16_t>((msb_lsb.first) << 8 | msb_lsb.second);
}

#endif  // UTILS_HXX
