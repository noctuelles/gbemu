#include "Utils.hxx"

#include "gtest/gtest.h"

TEST(TileTest, Test)
{
    /**
     * https://gbdev.io/pandocs/Tile_Data.html#data-format
     */

    std::array<uint8_t, 16> data{0x3C, 0x7E, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,
                                 0x7E, 0x5E, 0x7E, 0x0A, 0x7C, 0x56, 0x38, 0x7C};
    utils::Tile             tile{data};

    ASSERT_EQ(tile[0][0], std::bitset<2>{0b00});
    ASSERT_EQ(tile[0][1], std::bitset<2>{0b10});
    ASSERT_EQ(tile[0][2], std::bitset<2>{0b11});
    ASSERT_EQ(tile[0][3], std::bitset<2>{0b11});
    ASSERT_EQ(tile[0][4], std::bitset<2>{0b11});
    ASSERT_EQ(tile[0][5], std::bitset<2>{0b11});
    ASSERT_EQ(tile[0][6], std::bitset<2>{0b10});
    ASSERT_EQ(tile[0][7], std::bitset<2>{0b00});

    ASSERT_EQ(tile[1][0], std::bitset<2>{0b00});
    ASSERT_EQ(tile[1][1], std::bitset<2>{0b11});
    ASSERT_EQ(tile[1][2], std::bitset<2>{0b00});
    ASSERT_EQ(tile[1][3], std::bitset<2>{0b00});
    ASSERT_EQ(tile[1][4], std::bitset<2>{0b00});
    ASSERT_EQ(tile[1][5], std::bitset<2>{0b00});
    ASSERT_EQ(tile[1][6], std::bitset<2>{0b11});
    ASSERT_EQ(tile[1][7], std::bitset<2>{0b00});

    ASSERT_EQ(tile[2][0], std::bitset<2>{0b00});
    ASSERT_EQ(tile[2][1], std::bitset<2>{0b11});
    ASSERT_EQ(tile[2][2], std::bitset<2>{0b00});
    ASSERT_EQ(tile[2][3], std::bitset<2>{0b00});
    ASSERT_EQ(tile[2][4], std::bitset<2>{0b00});
    ASSERT_EQ(tile[2][5], std::bitset<2>{0b00});
    ASSERT_EQ(tile[2][6], std::bitset<2>{0b11});
    ASSERT_EQ(tile[2][7], std::bitset<2>{0b00});

    ASSERT_EQ(tile[3][0], std::bitset<2>{0b00});
    ASSERT_EQ(tile[3][1], std::bitset<2>{0b11});
    ASSERT_EQ(tile[3][2], std::bitset<2>{0b00});
    ASSERT_EQ(tile[3][3], std::bitset<2>{0b00});
    ASSERT_EQ(tile[3][4], std::bitset<2>{0b00});
    ASSERT_EQ(tile[3][5], std::bitset<2>{0b00});
    ASSERT_EQ(tile[3][6], std::bitset<2>{0b11});
    ASSERT_EQ(tile[3][7], std::bitset<2>{0b00});

    ASSERT_EQ(tile[4][0], std::bitset<2>{0b00});
    ASSERT_EQ(tile[4][1], std::bitset<2>{0b11});
    ASSERT_EQ(tile[4][2], std::bitset<2>{0b11});
    ASSERT_EQ(tile[4][3], std::bitset<2>{0b11});
    ASSERT_EQ(tile[4][4], std::bitset<2>{0b11});
    ASSERT_EQ(tile[4][5], std::bitset<2>{0b01});
    ASSERT_EQ(tile[4][6], std::bitset<2>{0b11});
    ASSERT_EQ(tile[4][7], std::bitset<2>{0b00});

    ASSERT_EQ(tile[5][0], std::bitset<2>{0b00});
    ASSERT_EQ(tile[5][1], std::bitset<2>{0b11});
    ASSERT_EQ(tile[5][2], std::bitset<2>{0b01});
    ASSERT_EQ(tile[5][3], std::bitset<2>{0b11});
    ASSERT_EQ(tile[5][4], std::bitset<2>{0b01});
    ASSERT_EQ(tile[5][5], std::bitset<2>{0b01});
    ASSERT_EQ(tile[5][6], std::bitset<2>{0b01});
    ASSERT_EQ(tile[5][7], std::bitset<2>{0b00});

    ASSERT_EQ(tile[6][0], std::bitset<2>{0b00});
    ASSERT_EQ(tile[6][1], std::bitset<2>{0b10});
    ASSERT_EQ(tile[6][2], std::bitset<2>{0b11});
    ASSERT_EQ(tile[6][3], std::bitset<2>{0b01});
    ASSERT_EQ(tile[6][4], std::bitset<2>{0b11});
    ASSERT_EQ(tile[6][5], std::bitset<2>{0b01});
    ASSERT_EQ(tile[6][6], std::bitset<2>{0b11});
    ASSERT_EQ(tile[6][7], std::bitset<2>{0b00});

    ASSERT_EQ(tile[7][0], std::bitset<2>{0b00});
    ASSERT_EQ(tile[7][1], std::bitset<2>{0b00});
    ASSERT_EQ(tile[7][2], std::bitset<2>{0b10});
    ASSERT_EQ(tile[7][3], std::bitset<2>{0b11});
    ASSERT_EQ(tile[7][4], std::bitset<2>{0b11});
    ASSERT_EQ(tile[7][5], std::bitset<2>{0b11});
    ASSERT_EQ(tile[7][6], std::bitset<2>{0b10});
    ASSERT_EQ(tile[7][7], std::bitset<2>{0b00});
}
