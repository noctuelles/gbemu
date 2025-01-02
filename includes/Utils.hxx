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

constexpr uint16_t u16(const uint8_t msb, const uint8_t lsb)
{
    return static_cast<uint16_t>(msb << 8 | lsb);
}

#endif  // UTILS_HXX
