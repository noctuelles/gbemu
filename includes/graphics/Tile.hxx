//
// Created by plouvel on 11/11/2025.
//

#ifndef GBEMU_TILE_HXX
#define GBEMU_TILE_HXX

#include <bitset>
#include <cstdint>
#include <span>

namespace Graphics
{
    static constexpr size_t TILE_MAP_SIZE{32};
    static constexpr size_t TILE_SIZE{8};
    static constexpr size_t BYTES_PER_LINE{16};

    inline uint8_t getRealColorIndexFromPaletteRegister(const uint8_t color, const uint8_t paletteRegister)
    {
        return paletteRegister >> (2 * color) & 0b11;
    }
}  // namespace Graphics

#endif  // GBEMU_TILE_HXX
