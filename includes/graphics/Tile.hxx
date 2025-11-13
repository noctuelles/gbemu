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
    using RGB          = std::tuple<uint8_t, uint8_t, uint8_t>;
    using ColorPalette = std::array<RGB, 4>;

    class Tile
    {
      public:
        using ColorIndex = std::bitset<2>;
        using Data       = std::array<uint8_t, 16>;
        using DataSpan   = std::span<const uint8_t, 16>;

        class Row
        {
          public:
            Row() = default;
            Row(uint8_t low, uint8_t high);

            ColorIndex operator[](size_t i) const noexcept;

          private:
            std::bitset<16> _data{};
        };

        explicit Tile(DataSpan data);

        const Row& operator[](size_t i) const noexcept;

      private:
        std::array<Row, 8> _data;
    };

    inline Tile::ColorIndex getRealColorIndexFromPaletteRegister(const Tile::ColorIndex& colorIndex,
                                                                 const uint8_t           paletteRegister)
    {
        return paletteRegister >> (2 * static_cast<uint8_t>(colorIndex.to_ulong())) & 0b11;
    }

    inline const RGB& getPixelColorFromPalette(const Tile::ColorIndex& colorIndex, const uint8_t paletteRegister,
                                               const ColorPalette& colorPalette)
    {
        return colorPalette[getRealColorIndexFromPaletteRegister(colorIndex, paletteRegister).to_ulong()];
    }

}  // namespace Graphics

#endif  // GBEMU_TILE_HXX
