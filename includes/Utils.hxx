//
// Created by plouvel on 12/10/24.
//

#ifndef UTILS_HXX
#define UTILS_HXX

#include <cstdint>

namespace utils
{
    constexpr auto word_lsb(const SM83::word x)
    {
        return static_cast<SM83::byte>(x & 0xFF);
    }

    constexpr auto word_msb(const SM83::word x)
    {
        return static_cast<SM83::byte>((x & 0xFF00) >> 8);
    }

    constexpr auto to_word(const SM83::byte msb, const SM83::byte lsb)
    {
        return static_cast<SM83::word>(msb << 8 | lsb);
    }

    constexpr void to_bytes(const SM83::word word, SM83::byte& msb, SM83::byte& lsb)
    {
        msb = static_cast<SM83::byte>(word >> 8);
        lsb = static_cast<SM83::byte>(word & 0xFF);
    }
};  // namespace utils

#endif  // UTILS_HXX
