//
// Created by plouvel on 12/10/24.
//

#include "Bus.hxx"

#include <SM83.hxx>
#include <Utils.hxx>
#include <utility>

#include "Cartridge.hxx"
#include "Common.hxx"
#include "PPU.hxx"
#include "Timer.hxx"
#include "Utils.hxx"

Bus::Bus(const std::shared_ptr<Addressable>& cartridge)
    : cpu(std::make_unique<SM83>(*this, [this] { on_machine_cycle(); })),
      ppu(std::make_unique<PPU>(*this)),
      timer(std::make_unique<Timer>(*this)),
      cartridge(cartridge)
{
}

void Bus::write(const uint16_t address, uint8_t value)
{
    using namespace utils;

    if (address_in(address, MemoryMap::ROM))
    {
        throw std::logic_error{"Writing to ROM"};
    }
    else if (address_in(address, MemoryMap::VIDEO_RAM))
    {
        ppu->write(address, value);
    }
    else if (address_in(address, MemoryMap::EXT_RAM))
    {
        throw std::logic_error{"External RAM not implemented"};
    }
    else if (address_in(address, MemoryMap::WORK_RAM) || address_in(address, MemoryMap::ECHO_RAM))
    {
        work_ram[address & 0x1fff] = value;
    }
    else if (address_in(address, MemoryMap::OAM))
    {
        ppu->write(address, value);
    }
    else if (address_in(address, MemoryMap::NOT_USABLE))
    {
        (void) address;
        (void) value;
    }
    else if (address_in(address, MemoryMap::IO_REGISTERS))
    {
        using namespace MemoryMap::IORegisters;

        switch (address)
        {
            case DIV:
            case TIMA:
            case TMA:
            case TAC:
                timer->write(address, value);
                break;
            case IF:
                cpu->write(address, value);
                break;
            default:
                break;
        }
    }
    else if (address_in(address, MemoryMap::HIGH_RAM))
    {
        high_ram[address & 0x7f] = value;
    }
    else if (address == MemoryMap::IE)
    {
        cpu->write(address, value);
    }

    throw std::logic_error{"Invalid bus write"};
}

uint8_t Bus::read(const uint16_t address)
{
    using namespace utils;

    if (address_in(address, MemoryMap::ROM))
    {
        return cartridge->read(address);
    }
    if (address_in(address, MemoryMap::VIDEO_RAM))
    {
        return ppu->read(address);
    }
    if (address_in(address, MemoryMap::EXT_RAM))
    {
        throw std::logic_error{"External RAM not implemented"};
    }
    if (address_in(address, MemoryMap::WORK_RAM) || address_in(address, MemoryMap::ECHO_RAM))
    {
        return work_ram[address & 0x1fff];
    }
    if (address_in(address, MemoryMap::OAM))
    {
        return ppu->read(address);
    }
    if (address_in(address, MemoryMap::NOT_USABLE))
    {
        (void) address;
    }
    if (address_in(address, MemoryMap::IO_REGISTERS))
    {
        using namespace MemoryMap::IORegisters;

        switch (address)
        {
            case DIV:
            case TIMA:
            case TMA:
            case TAC:
                return timer->read(address);
            case IF:
                return cpu->read(address);
            default:
                return 0xFF;
        }
    }
    if (address_in(address, MemoryMap::HIGH_RAM))
    {
        return high_ram[address & 0x7f];
    }
    if (address == MemoryMap::IE)
    {
        return cpu->read(address);
    }

    throw std::logic_error{"Invalid bus read"};
}

std::size_t Bus::frame()  // NOLINT
{
    m_cycles = 0;

    cpu->tick();

    return m_cycles;
}

void Bus::on_machine_cycle()
{
    m_cycles += 1;

    timer->tick();
}