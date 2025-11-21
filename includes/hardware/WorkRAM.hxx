//
// Created by plouvel on 3/13/25.
//

#ifndef WORKRAM_HXX
#define WORKRAM_HXX

#include <array>

#include "Addressable.hxx"
#include "Common.hxx"

template <std::size_t N, uint16_t O = 0>
class FixedSizeRAM : public Addressable
{
  public:
    static constexpr uint16_t OFFSET = O;

    FixedSizeRAM() = default;

    [[nodiscard]] uint8_t read(uint16_t address) const override
    {
        return content[address - O];
    }

    void write(uint16_t address, uint8_t value) override
    {
        content[address - O] = value;
    }

  private:
    std::array<uint8_t, N> content{};
};

class WorkRAM final : public FixedSizeRAM<0x2000, MemoryMap::WORK_RAM.first>
{
  public:
    [[nodiscard]] AddressableRange getAddressableRange() const noexcept override;
};

class FakeRAM final : public FixedSizeRAM<0x10000>
{
  public:
    [[nodiscard]] AddressableRange getAddressableRange() const noexcept override;
};

#endif  // WORKRAM_HXX
