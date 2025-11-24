//
// Created by plouvel on 23/11/2025.
//

#include "Emulator.hxx"

#include <QThread>
#include <QTimer>
#include <iostream>

Emulator::Emulator(QObject* parent)
    : QObject(parent),
      _state(),
      _bus(_state),
      _cartridge("../roms/tetris.gb"),
      _timer(_bus),
      _ppu(_bus),
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

void Emulator::runFrame()
{
    /* Run the emulation for a whole frame. A frame = 70,224 dots = 17,556 machine cycles. */
    _cpu.tick(17556);

    emit frameReady(_ppu.getFramebuffer());
}
