//
// Created by plouvel on 3/13/25.
//

#ifndef WORKRAM_HXX
#define WORKRAM_HXX

#include "Addressable.hxx"
#include <array>

class WorkRAM final : public Addressable {
public:
    WorkRAM() = default;

    uint8_t read(uint16_t address) override;
    void write(uint16_t address, uint8_t value) override;
    [[nodiscard]] AddressableRange get_addressable_range() const override;

private:
    std::array<uint8_t, 8192> content{};
};

#endif //WORKRAM_HXX
