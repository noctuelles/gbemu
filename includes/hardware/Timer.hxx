//
// Created by plouvel on 1/8/25.
//

#ifndef TIMER_HPP
#define TIMER_HPP

#include "hardware/IAddressable.hxx"

class Timer final : public IComponent
{
  public:
    enum class State
    {
        NORMAL,
        SCHEDULE_INTERRUPT_AND_TMA_RELOAD,
        RELOADING_TIMA_TO_TMA,
    };

    explicit Timer(IAddressable& bus);
    ~Timer() override;

    void                           write(uint16_t address, uint8_t value) override;
    uint8_t                        read(uint16_t address) const override;
    [[nodiscard]] AddressableRange getAddressableRange() const noexcept override;
    void                           tick(size_t machineCycle = 1) override;

  private:
    void setSystemCounter(uint16_t value);
    void detectFallingEdge(bool bit);

    IAddressable& bus;

    uint16_t system_counter{0xABCC};
    uint8_t  TIMA{};
    uint8_t  TMA{};
    uint8_t  TAC{};

    State state{State::NORMAL};
    bool  lastBit{};

    friend class MooneyeAcceptance;
};

#endif  // TIMER_HPP
