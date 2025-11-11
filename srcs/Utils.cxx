//
// Created by plouvel on 12/31/24.
//

#include "Utils.hxx"

#include <ranges>

using namespace utils;

Tile::Row::Row(const uint8_t low, const uint8_t high)
{
    const std::bitset<8> lowBits{low};
    const std::bitset<8> highBits{high};

    for (size_t i = 8; i > 0; i--)
    {
        _data[(7 - (i - 1)) * 2]     = lowBits[i - 1];
        _data[(7 - (i - 1)) * 2 + 1] = highBits[i - 1];
    }
}

std::bitset<2> Tile::Row::operator[](const size_t i) const noexcept
{
    std::bitset<2> result{};

    result[0] = _data[i * 2];
    result[1] = _data[i * 2 + 1];

    return result;
}

Tile::Tile(const std::span<const uint8_t, 16> data)
{
    for (size_t i = 0; i < 8; i++)
    {
        _data[i] = Row{data[i * 2], data[i * 2 + 1]};
    }
}

const Tile::Row& Tile::operator[](size_t i) const noexcept
{
    return _data[i];
}