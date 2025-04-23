//
// Created by plouvel on 4/23/25.
//

#ifndef JOYPAD_HXX
#define JOYPAD_HXX

#include "Addressable.hxx"

class Joypad final : public Addressable
{
  public:
    /**
     * Represents the buttons on a joypad. Each button corresponds to a specific action
     * or direction input.
     *
     * Enumerators:
     * - UP: Represents the "Up" direction input.
     * - DOWN: Represents the "Down" direction input.
     * - LEFT: Represents the "Left" direction input.
     * - RIGHT: Represents the "Right" direction input.
     * - A: Represents the "A" button.
     * - B: Represents the "B" button.
     * - SELECT: Represents the "Select" button.
     * - START: Represents the "Start" button.
     */
    enum class Button : uint8_t
    {
        Up     = 0x04,
        Down   = 0x08,
        Left   = 0x02,
        Right  = 0x01,
        A      = 0x01,
        B      = 0x02,
        Select = 0x04,
        Start  = 0x08,
    };

    Joypad() = default;

    [[nodiscard]] uint8_t          read(uint16_t address) override;
    void                           write(uint16_t address, uint8_t value) override;
    [[nodiscard]] AddressableRange get_addressable_range() const override;

    void press(Button button);
    void release(Button button);

  private:
    uint8_t state{0x30};
};

#endif  // JOYPAD_HXX
