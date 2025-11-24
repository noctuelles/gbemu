//
// Created by plouvel on 23/11/2025.
//

#ifndef GBEMU_EMULATOR_HXX
#define GBEMU_EMULATOR_HXX

#include <QObject>

#include "hardware/Bus.hxx"
#include "hardware/Cartridge.hxx"
#include "hardware/EchoRAM.hxx"
#include "hardware/Joypad.hxx"
#include "hardware/Timer.hxx"
#include "hardware/WorkRAM.hxx"
#include "hardware/core/SM83.hxx"

class Emulator final : public QObject
{
    Q_OBJECT

  public:
    explicit Emulator(QObject* parent = nullptr);

  public slots:
    void runFrame();

  signals:
    void frameReady(const Graphics::Framebuffer& framebuffer);

  private:
    EmulationState _state;
    Bus            _bus;
    Cartridge      _cartridge;
    Timer          _timer;
    PPU            _ppu;
    SM83           _cpu;
    WorkRAM        _workRam;
    EchoRAM        _echoRam;
    Joypad         _joypad;
    /**
     * @brief Used to fill gaps.
     */
    FakeRAM _fakeRam;

    bool _running{true};
};

#endif  // GBEMU_EMULATOR_HXX
