//
// Created by plouvel on 1/8/25.
//

#include "Timer.hxx"

#include <bitset>
#include <chrono>
#include <format>
#include <stdexcept>

Timer::Timer(Addressable& bus) : bus(bus) {}

Timer::~Timer() = default;

void Timer::write(uint16_t address, const uint8_t value)
{
    switch (address)
    {
        case 0xFF04:
            /**
             * Resetting the entire system counter (by writing to DIV) can reset the bit currently selected by the
             * multiplexer, thus sending a “Timer tick” and/or “DIV-APU event” pulse early.
             */
            (void) value;
            set_div(0);
            break;
        case 0xFF05:
            switch (state)
            {
                [[likely]] case State::NORMAL:
                    TIMA = value;
                    break;
                /* Writing to TIMA during cycle A acts as if the overflow did not happen! TMA will not be copied to TIMA
                 * (the value written will therefore stay), and bit 2 of IF will not be set. */
                [[unlikely]] case State::SCHEDULE_INTERRUPT_AND_TMA_RELOAD:
                    TIMA  = value;
                    state = State::NORMAL;
                    break;
                /* Writing to TIMA during the cycle when TIMA have been set to TMA will be ignored; TIMA will be equal
                 * to TMA at the end of the cycle anyway. */
                [[unlikely]] case State::RELOADING_TIMA_TO_TMA:
                    break;
            }
            break;
        case 0xFF06:
            switch (state)
            {
                [[likely]] case State::NORMAL:
                [[likely]] case State::SCHEDULE_INTERRUPT_AND_TMA_RELOAD:
                    TMA = value;
                    break;
                /* Writing to TMA during cycle B will have the same value copied to TIMA as well, on the same cycle. */
                [[unlikely]] case State::RELOADING_TIMA_TO_TMA:
                    TMA  = value;
                    TIMA = TMA;
                    break;
            }
            break;
        case 0xFF07:
        {
            /**
             * TODO
             *
             * Changing which bit of the system counter is selected (by changing the “Clock select” bits of TAC)
             * from a bit currently set to another that is currently unset, will send a “Timer tick” pulse. (For
             * example: if the system counter is equal to $3FF0 and TAC to $FC, writing $05 or $06 to TAC will
             * instantly send a “Timer tick”, but $04 or $07 won’t.)
             */

            TAC = value;
            break;
        }
        default:
            throw std::logic_error(std::format("Invalid timer write at 0x{:04X}", address));
    };
}

uint8_t Timer::read(const uint16_t address)
{
    switch (address)
    {
        case 0xFF04:
            return DIV >> 6;
        case 0xFF05:
            return TIMA;
        case 0xFF06:
            return TMA;
        case 0xFF07:
            return TAC;
        default:
            throw std::logic_error(std::format("Invalid timer read at 0x{:04X}", address));
    };
}

void Timer::tick()
{
    switch (state)
    {
        case State::NORMAL:
            break;
        case State::SCHEDULE_INTERRUPT_AND_TMA_RELOAD:
            state = State::RELOADING_TIMA_TO_TMA;
            TIMA  = TMA;
            bus.write(0xFF0F, bus.read(0xFF0F) | 1 << 2);
            break;
        case State::RELOADING_TIMA_TO_TMA:
            state = State::NORMAL;
    }

    set_div(DIV + 1);
}

void Timer::set_div(const uint8_t value)
{
    auto bit_set{false};

    DIV = value;

    // ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
    switch (static_cast<uint8_t>(TAC & 0b11))
    {
        case 0b00:
            bit_set = (DIV >> 7 & 1) != 0; /* Every 256 M-cycles. */
            break;
        case 0b01:
            bit_set = (DIV >> 1 & 1) != 0; /* Every 4 M-cycles */
            break;
        case 0b10:
            bit_set = (DIV >> 3 & 1) != 0; /* Every 16 M-cycles */
            break;
        case 0b11:
            bit_set = (DIV >> 5 & 1) != 0; /* Every 64 M-cycles */
            break;
    };

    bit_set = bit_set && (TAC & 0b100) != 0;
    detect_falling_edge(bit_set);
    last_bit = bit_set;
}

/**
 * @note Do not interpret a "falling edge" with a clock falling edge ! This is different.
 */
void Timer::detect_falling_edge(const bool bit)
{
    if (!bit & last_bit)
    {
        TIMA += 1;
        if (TIMA == 0)
        {
            /**
             * When TIMA overflows, the value from TMA is copied, and the timer flag is set in IF, but one M-cycle
             * later. This means that TIMA is equal to $00 for the M-cycle after it overflows. This only happens
             * when TIMA overflows from incrementing, it cannot be made to happen by manually writing to TIMA.
             */
            state = State::SCHEDULE_INTERRUPT_AND_TMA_RELOAD;
        }
    }
}
