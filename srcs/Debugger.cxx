//
// Created by plouvel on 12/10/25.
//

#include "Emulator.hxx"

Emulator::Debugger::Debugger(SM83& cpu) : _cpu(cpu) {}

void Emulator::Debugger::addBreakpoint(uint16_t address)
{
    _breakpoints.insert(address);
}

void Emulator::Debugger::removeBreakpoint(uint16_t address)
{
    _breakpoints.erase(address);
}

bool Emulator::Debugger::shouldBreak() const
{
    const SM83::View cpuView{_cpu.getView()};

    if (_breakpoints.contains(cpuView.registers.PC))
    {
        return true;
    }

    return false;
}
