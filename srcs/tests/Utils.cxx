//
// Created by plouvel on 12/7/25.
//

#include "Utils.hxx"

#include <gtest/gtest.h>

#include <algorithm>

using ReversePair = std::pair<uint8_t, uint8_t>;

class ReverseByteTest : public ::testing::TestWithParam<ReversePair>
{
};

TEST_P(ReverseByteTest, ReversesCorrectly)
{
    auto [input, expected] = GetParam();
    EXPECT_EQ(Utils::reverseByte(input), expected);
}

INSTANTIATE_TEST_SUITE_P(ReverseByteCases, ReverseByteTest,
                         ::testing::Values(ReversePair{0b10001101, 0b10110001}, ReversePair{0b10000000, 0b00000001},
                                           ReversePair{0b11110000, 0b00001111}, ReversePair{0b10101010, 0b01010101}));
