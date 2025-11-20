//
// Created by plouvel on 1/8/25.
//

#ifndef TIMER_HPP
#define TIMER_HPP

#include "hardware/Addressable.hxx"

class Timer final : public Component
{
  public:
    enum class State
    {
        NORMAL,
        SCHEDULE_INTERRUPT_AND_TMA_RELOAD,
        RELOADING_TIMA_TO_TMA,
    };

    explicit Timer(Addressable& bus);
    ~Timer() override;

    void                           write(uint16_t address, uint8_t value) override;
    uint8_t                        read(uint16_t address) const override;
    [[nodiscard]] AddressableRange getAddressableRange() const noexcept override;
    void                           tick() override;

  private:
    void set_system_counter(uint16_t value);
    void detect_falling_edge(bool bit);

    Addressable& bus;

    uint16_t system_counter{0xABCC};
    uint8_t  TIMA{};
    uint8_t  TMA{};
    uint8_t  TAC{};

    State state{State::NORMAL};
    bool  last_bit{};

    friend class MooneyeAcceptance;
};

#endif  // TIMER_HPP
