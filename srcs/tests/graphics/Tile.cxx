//
// Created by plouvel on 11/11/2025.
//

#include "graphics/Tile.hxx"

#include "gtest/gtest.h"

static constexpr Graphics::ColorPalette gColorPalette{
    Graphics::RGB{0xFF, 0xFF, 0xFF},
    Graphics::RGB{0xAA, 0xAA, 0xAA},
    Graphics::RGB{0x55, 0x55, 0x55},
    Graphics::RGB{0x00, 0x00, 0x00},
};

TEST(TileTest, Test)
{
    /**
     * https://gbdev.io/pandocs/Tile_Data.html#data-format
     */

    using namespace Graphics;

    Tile::Data data{0x3C, 0x7E, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x7E, 0x5E, 0x7E, 0x0A, 0x7C, 0x56, 0x38, 0x7C};

    Tile tile{data};

    ASSERT_EQ(tile[0][0], Tile::ColorIndex{0b00});
    ASSERT_EQ(tile[0][1], Tile::ColorIndex{0b10});
    ASSERT_EQ(tile[0][2], Tile::ColorIndex{0b11});
    ASSERT_EQ(tile[0][3], Tile::ColorIndex{0b11});
    ASSERT_EQ(tile[0][4], Tile::ColorIndex{0b11});
    ASSERT_EQ(tile[0][5], Tile::ColorIndex{0b11});
    ASSERT_EQ(tile[0][6], Tile::ColorIndex{0b10});
    ASSERT_EQ(tile[0][7], Tile::ColorIndex{0b00});

    ASSERT_EQ(tile[1][0], Tile::ColorIndex{0b00});
    ASSERT_EQ(tile[1][1], Tile::ColorIndex{0b11});
    ASSERT_EQ(tile[1][2], Tile::ColorIndex{0b00});
    ASSERT_EQ(tile[1][3], Tile::ColorIndex{0b00});
    ASSERT_EQ(tile[1][4], Tile::ColorIndex{0b00});
    ASSERT_EQ(tile[1][5], Tile::ColorIndex{0b00});
    ASSERT_EQ(tile[1][6], Tile::ColorIndex{0b11});
    ASSERT_EQ(tile[1][7], Tile::ColorIndex{0b00});

    ASSERT_EQ(tile[2][0], Tile::ColorIndex{0b00});
    ASSERT_EQ(tile[2][1], Tile::ColorIndex{0b11});
    ASSERT_EQ(tile[2][2], Tile::ColorIndex{0b00});
    ASSERT_EQ(tile[2][3], Tile::ColorIndex{0b00});
    ASSERT_EQ(tile[2][4], Tile::ColorIndex{0b00});
    ASSERT_EQ(tile[2][5], Tile::ColorIndex{0b00});
    ASSERT_EQ(tile[2][6], Tile::ColorIndex{0b11});
    ASSERT_EQ(tile[2][7], Tile::ColorIndex{0b00});

    ASSERT_EQ(tile[3][0], Tile::ColorIndex{0b00});
    ASSERT_EQ(tile[3][1], Tile::ColorIndex{0b11});
    ASSERT_EQ(tile[3][2], Tile::ColorIndex{0b00});
    ASSERT_EQ(tile[3][3], Tile::ColorIndex{0b00});
    ASSERT_EQ(tile[3][4], Tile::ColorIndex{0b00});
    ASSERT_EQ(tile[3][5], Tile::ColorIndex{0b00});
    ASSERT_EQ(tile[3][6], Tile::ColorIndex{0b11});
    ASSERT_EQ(tile[3][7], Tile::ColorIndex{0b00});

    ASSERT_EQ(tile[4][0], Tile::ColorIndex{0b00});
    ASSERT_EQ(tile[4][1], Tile::ColorIndex{0b11});
    ASSERT_EQ(tile[4][2], Tile::ColorIndex{0b01});
    ASSERT_EQ(tile[4][3], Tile::ColorIndex{0b11});
    ASSERT_EQ(tile[4][4], Tile::ColorIndex{0b11});
    ASSERT_EQ(tile[4][5], Tile::ColorIndex{0b11});
    ASSERT_EQ(tile[4][6], Tile::ColorIndex{0b11});
    ASSERT_EQ(tile[4][7], Tile::ColorIndex{0b00});

    ASSERT_EQ(tile[5][0], Tile::ColorIndex{0b00});
    ASSERT_EQ(tile[5][1], Tile::ColorIndex{0b01});
    ASSERT_EQ(tile[5][2], Tile::ColorIndex{0b01});
    ASSERT_EQ(tile[5][3], Tile::ColorIndex{0b01});
    ASSERT_EQ(tile[5][4], Tile::ColorIndex{0b11});
    ASSERT_EQ(tile[5][5], Tile::ColorIndex{0b01});
    ASSERT_EQ(tile[5][6], Tile::ColorIndex{0b11});
    ASSERT_EQ(tile[5][7], Tile::ColorIndex{0b00});

    ASSERT_EQ(tile[6][0], Tile::ColorIndex{0b00});
    ASSERT_EQ(tile[6][1], Tile::ColorIndex{0b11});
    ASSERT_EQ(tile[6][2], Tile::ColorIndex{0b01});
    ASSERT_EQ(tile[6][3], Tile::ColorIndex{0b11});
    ASSERT_EQ(tile[6][4], Tile::ColorIndex{0b01});
    ASSERT_EQ(tile[6][5], Tile::ColorIndex{0b11});
    ASSERT_EQ(tile[6][6], Tile::ColorIndex{0b10});
    ASSERT_EQ(tile[6][7], Tile::ColorIndex{0b00});

    ASSERT_EQ(tile[7][0], Tile::ColorIndex{0b00});
    ASSERT_EQ(tile[7][1], Tile::ColorIndex{0b10});
    ASSERT_EQ(tile[7][2], Tile::ColorIndex{0b11});
    ASSERT_EQ(tile[7][3], Tile::ColorIndex{0b11});
    ASSERT_EQ(tile[7][4], Tile::ColorIndex{0b11});
    ASSERT_EQ(tile[7][5], Tile::ColorIndex{0b10});
    ASSERT_EQ(tile[7][6], Tile::ColorIndex{0b00});
    ASSERT_EQ(tile[7][7], Tile::ColorIndex{0b00});
}

TEST(getPixelColorFromPalette, BootromPaletteRegister)
{
    using namespace Graphics;

    constexpr uint8_t bootromPalette{0b11111100};

    ASSERT_EQ(getPixelColorFromPalette(std::bitset<2>(0b00), bootromPalette, gColorPalette), gColorPalette[0]);
    ASSERT_EQ(getPixelColorFromPalette(std::bitset<2>(0b01), bootromPalette, gColorPalette), gColorPalette[3]);
    ASSERT_EQ(getPixelColorFromPalette(std::bitset<2>(0b10), bootromPalette, gColorPalette), gColorPalette[3]);
    ASSERT_EQ(getPixelColorFromPalette(std::bitset<2>(0b11), bootromPalette, gColorPalette), gColorPalette[3]);
}

TEST(getPixelColorFromPalette, IdentityPaletteRegister)
{
    using namespace Graphics;

    constexpr uint8_t identityPalette{0b11100100};

    ASSERT_EQ(getPixelColorFromPalette(std::bitset<2>(0b00), identityPalette, gColorPalette), gColorPalette[0]);
    ASSERT_EQ(getPixelColorFromPalette(std::bitset<2>(0b01), identityPalette, gColorPalette), gColorPalette[1]);
    ASSERT_EQ(getPixelColorFromPalette(std::bitset<2>(0b10), identityPalette, gColorPalette), gColorPalette[2]);
    ASSERT_EQ(getPixelColorFromPalette(std::bitset<2>(0b11), identityPalette, gColorPalette), gColorPalette[3]);
}