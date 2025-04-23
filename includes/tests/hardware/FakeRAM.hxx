//
// Created by plouvel on 4/23/25.
//

#ifndef FAKERAM_HXX
#define FAKERAM_HXX

#include <array>

#include "hardware/Addressable.hxx"

struct FakeRAM final : Addressable
{
    uint8_t                        read(uint16_t address) override;
    void                           write(uint16_t address, uint8_t value) override;
    [[nodiscard]] AddressableRange get_addressable_range() const noexcept override;

    std::array<uint8_t, 0xFFFF> content{};
};

#endif  // FAKERAM_HXX
