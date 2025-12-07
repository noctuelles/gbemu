//
// Created by plouvel on 11/11/2025.
//

#include "graphics/Tile.hxx"
#include "gtest/gtest.h"


TEST(TileTest, HorizontalFlip)
{
    uint8_t byte{0b10001101};
    uint8_t result{Graphics::horizontalFlip(byte)};

    ASSERT_EQ(result, 0b10110001);
}