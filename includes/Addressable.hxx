//
// Created by plouvel on 1/6/25.
//

#ifndef ADDRESSABLE_HPP
#define ADDRESSABLE_HPP

#include <cstdint>

struct Addressable
{
    virtual ~Addressable()                                 = default;
    virtual uint8_t read(uint16_t address)                 = 0;
    virtual void    write(uint16_t address, uint8_t value) = 0;
};

struct Ticking
{
    virtual ~Ticking()  = default;
    virtual void tick() = 0;
};

struct Component : Addressable, Ticking
{
};

#endif  // ADDRESSABLE_HPP
