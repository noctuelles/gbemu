#ifndef BUS_HXX
#define BUS_HXX

#include <array>
#include <memory>
#include <span>
#include <vector>

#include "EmulationState.hxx"
#include "IAddressable.hxx"

class Bus final : public IAddressable
{
  public:
    explicit Bus(const EmulationState& emulationState);

    void                           loadBootRom(const std::array<uint8_t, 256>& bootRom) noexcept;
    void                           write(uint16_t address, uint8_t value) override;
    [[nodiscard]] uint8_t          read(uint16_t address) const override;
    [[nodiscard]] AddressableRange getAddressableRange() const noexcept override;

    void setPostBootRomRegisters();

    [[nodiscard]] std::array<uint8_t, 0x10000> getAddressSpace() const noexcept;

    void attach(IAddressable& addressable);

  private:
    const EmulationState&              _emulationState;
    std::array<IAddressable*, 0x10000> _memoryMap{};

    /**
     * @brief Indicates whether the boot ROM is currently mapped to the memory map. Active low.
     */
    uint8_t                    _bootRomMapped{};
    std::array<uint8_t, 0x100> _bootRom{};
};

#endif  // BUS_HXX
