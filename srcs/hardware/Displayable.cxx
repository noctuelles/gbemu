//
// Created by plouvel on 4/25/25.
//

#include "hardware/Displayable.hxx"

#include <format>

Displayable::OutOfBound::OutOfBound(std::size_t x, std::size_t y)
    : std::runtime_error(std::format("Coordinate {} and {} are out of bounds.", x, y))
{
}
void Displayable::put_pixel(const std::size_t x, const std::size_t y, const std::uint8_t color_index)
{
    (void) color_index;

    if (x >= WIDTH || y >= HEIGHT)
    {
        throw OutOfBound{x, y};
    }
}
