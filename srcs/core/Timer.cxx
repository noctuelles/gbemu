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
            /**
             * Writing to TIMA during cycle A acts as if the overflow didn’t happen! TMA will not be copied to TIMA (the
             * value written will therefore stay), and bit 2 of IF will not be set. Writing to DIV, TAC, or other
             * registers won’t prevent the IF flag from being set or TIMA from being reloaded.
             */

            if (schedule_interrupt_for_next_cycle)
            {
                schedule_interrupt_for_next_cycle = false;
            }
            if (!setting_tima_to_tma)
            {
                TIMA = value;
            }
            else
            {
                (void) value;
            }
            break;
        case 0xFF06:
            /**
             * Writing to TMA during cycle B will have the same value copied to TIMA as well, on the same cycle.
             */
            if (setting_tima_to_tma)
            {
                TIMA = value;
            }
            TMA = value;
            break;
        case 0xFF07:
        {
            /**
             * TODO
             *
             * Changing which bit of the system counter is selected (by changing the “Clock select” bits of TAC) from a
             * bit currently set to another that is currently unset, will send a “Timer tick” pulse. (For example: if
             * the system counter is equal to $3FF0 and TAC to $FC, writing $05 or $06 to TAC will instantly send a
             * “Timer tick”, but $04 or $07 won’t.)
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
    setting_tima_to_tma = false;
    if (schedule_interrupt_for_next_cycle)
    {
        schedule_interrupt_for_next_cycle = false;
        setting_tima_to_tma               = true;
        TIMA                              = TMA;
        bus.write(0xFF0F, bus.read(0xFF0F) | 1 << 2);
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

void Timer::detect_falling_edge(const bool bit)
{
    if (!bit & last_bit)
    {
        TIMA += 1;
        if (TIMA == 0)
        {
            schedule_interrupt_for_next_cycle = true;
        }
    }
}
