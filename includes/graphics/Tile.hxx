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
    inline uint8_t getRealColorIndexFromPaletteRegister(const uint8_t color, const uint8_t paletteRegister)
    {
        return paletteRegister >> (2 * color) & 0b11;
    }
}  // namespace Graphics

#endif  // GBEMU_TILE_HXX
