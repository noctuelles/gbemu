//
// Created by plouvel on 23/11/2025.
//

#include "Emulator.hxx"

#include <QThread>
#include <QTimer>
#include <iostream>

Emulator::Emulator(IRenderer* renderer, QObject* parent)
    : QThread(parent),
      _state(),
      _bus(_state),
      _cartridge("../roms/tetris.gb"),
      _timer(_bus),
      _ppu(_bus, *renderer),
      _cpu(_state, _bus, _timer, _ppu),
      _echoRam(_workRam)
{
    _bus.attach(_cartridge);
    _bus.attach(_timer);
    _bus.attach(_ppu);
    _bus.attach(_cpu);
    _bus.attach(_echoRam);
    _bus.attach(_joypad);
    _bus.attach(_workRam);
    _bus.attach(_fakeRam);
}

void Emulator::run()
{
    while (_running)
    {
        _cpu.tick();
    }
}
