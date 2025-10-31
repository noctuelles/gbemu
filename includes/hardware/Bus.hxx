#ifndef BUS_HXX
#define BUS_HXX

#include <array>
#include <memory>
#include <vector>

#include "Addressable.hxx"

class Bus final : public Addressable
{
  public:
    Bus() = default;

    void                           write(uint16_t address, uint8_t value) override;
    [[nodiscard]] uint8_t          read(uint16_t address) const override;
    [[nodiscard]] AddressableRange getAddressableRange() const noexcept override;

    [[nodiscard]] std::span<const uint8_t, 0x10000> getAddressSpace() const noexcept;

    void attach(Addressable& addressable);

  private:
    std::array<Addressable*, 0x10000> memory_map{};

    /**
     * @brief Used as a debugging purpose.
     */
    std::array<uint8_t, 0x10000> addressSpace{};
};

#endif  // BUS_HXX
