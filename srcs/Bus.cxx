//
// Created by plouvel on 12/10/24.
//

#include "Bus.hxx"

#include <SM83.hxx>
#include <utility>

#include "Timer.hxx"

Bus::Bus() : cpu(std::make_unique<SM83>(*this, [this] { timer->tick(); })), timer(std::make_unique<Timer>(*this)) {}

void Bus::write(const uint16_t address, uint8_t value)
{
    switch (address)
    {
        case 0xFF04:
        case 0xFF05:
        case 0xFF06:
        case 0xFF07:
            timer->write(address, value);
            break;
        case 0xFF0F:
        case 0xFFFF:
            cpu->write(address, value);
            break;
        default:
            ram[address] = std::byte{value};
    }
}

uint8_t Bus::read(const uint16_t address)
{
    switch (address)
    {
        case 0xFF04:
        case 0xFF05:
        case 0xFF06:
        case 0xFF07:
            return timer->read(address);
        case 0xFF02:
        case 0xFF44:
            return 0xFF;
        case 0xFF0F:
        case 0xFFFF:
            return cpu->read(address);
        default:
            return std::to_underlying(ram[address]);
    }
}