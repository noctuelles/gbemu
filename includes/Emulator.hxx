//
// Created by plouvel on 23/11/2025.
//

#ifndef GBEMU_EMULATOR_HXX
#define GBEMU_EMULATOR_HXX


#include "hardware/Bus.hxx"
#include "hardware/Cartridge.hxx"
#include "hardware/EchoRAM.hxx"
#include "hardware/Joypad.hxx"
#include "hardware/Timer.hxx"
#include "hardware/WorkRAM.hxx"
#include "hardware/core/SM83.hxx"

#include <QThread>

class Emulator final : public QThread
{
    Q_OBJECT

  public:
    explicit Emulator(IRenderer* renderer, QObject* parent = nullptr);

  protected:
    void run() override;

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
