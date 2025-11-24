//
// Created by plouvel on 4/23/25.
//

#ifndef JOYPAD_HXX
#define JOYPAD_HXX

#include "Common.hxx"
#include "IAddressable.hxx"

class Joypad final : public IAddressable
{
  public:
    Joypad() = default;

    [[nodiscard]] uint8_t          read(uint16_t address) const override;
    void                           write(uint16_t address, uint8_t value) override;
    [[nodiscard]] AddressableRange getAddressableRange() const noexcept override;

    void press(Button button);
    void release(Button button);

  private:
    uint8_t _state{0xFF};
    uint8_t _selectButtons{0x20};
    uint8_t _selectDirections{0x10};
};

#endif  // JOYPAD_HXX
