#ifndef BUS_HXX
#define BUS_HXX

#include <array>
#include <memory>
#include <vector>

#include "EmulationState.hxx"
#include "IAddressable.hxx"

class Bus final : public IAddressable
{
  public:
    explicit Bus(const EmulationState& emulationState);

    void                           write(uint16_t address, uint8_t value) override;
    [[nodiscard]] uint8_t          read(uint16_t address) const override;
    [[nodiscard]] AddressableRange getAddressableRange() const noexcept override;

    [[nodiscard]] std::array<uint8_t, 0x10000> getAddressSpace() const noexcept;

    void attach(IAddressable& addressable);

  private:
    const EmulationState&             emulationState;
    std::array<IAddressable*, 0x10000> memoryMap{};
};

#endif  // BUS_HXX
