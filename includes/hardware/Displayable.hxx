//
// Created by plouvel on 4/25/25.
//

#ifndef DISPLAYABLE_HXX
#define DISPLAYABLE_HXX
#include <cstddef>
#include <cstdint>
#include <exception>
#include <stdexcept>


struct Displayable
{
    struct OutOfBound final : std::runtime_error
    {
        OutOfBound(std::size_t x, std::size_t y);
    };

    static constexpr std::size_t WIDTH{144};
    static constexpr std::size_t HEIGHT{160};

    Displayable()          = default;
    virtual ~Displayable() = default;

    virtual void turn_off() = 0;
    virtual void turn_on()  = 0;
    virtual void update()   = 0;

    virtual void put_pixel(std::size_t x, std::size_t y, std::uint8_t color_index);
};

#endif  // DISPLAYABLE_HXX
