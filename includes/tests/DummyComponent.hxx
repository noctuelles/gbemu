//
// Created by plouvel on 12/4/25.
//

#ifndef GBEMU_DUMMYCOMPONENT_HXX
#define GBEMU_DUMMYCOMPONENT_HXX

#include "hardware/IAddressable.hxx"

class DummyComponent final : public IComponent
{
  public:
    [[nodiscard]] uint8_t          read(uint16_t address) const override;
    void                           write(uint16_t address, uint8_t value) override;
    [[nodiscard]] AddressableRange getAddressableRange() const noexcept override;
    void                           tick(size_t machineCycle) override;
};

#endif  // GBEMU_DUMMYCOMPONENT_HXX
