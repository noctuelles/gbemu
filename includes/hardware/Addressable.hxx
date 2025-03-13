//
// Created by plouvel on 1/6/25.
//

#ifndef ADDRESSABLE_HPP
#define ADDRESSABLE_HPP

#include <cstdint>
#include <variant>
#include <vector>

struct Addressable
{
    using AddressRange = std::pair<uint16_t, uint16_t>;
    using AddressableRange = std::vector<std::variant<uint16_t, AddressRange>>;

    virtual ~Addressable()                                 = default;

    virtual uint8_t read(uint16_t address)                 = 0;
    virtual void    write(uint16_t address, uint8_t value) = 0;

    [[nodiscard]] virtual AddressableRange get_addressable_range() const = 0;
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
