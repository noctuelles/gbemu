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
    void loadRom(const QString& path);
    void onKeyPressed(Key key) const;
    void onKeyReleased(Key key) const;
    void runFrame();

  signals:
    void frameReady(const Graphics::Framebuffer& framebuffer);
    void emulationFatalError(const QString& message);

  private:
    class Components
    {
        EmulationState _state;

      public:
        Components();

        Bus       bus;
        Cartridge cartridge;
        Timer     timer;
        PPU       ppu;
        SM83      cpu;
        WorkRAM   workRam;
        EchoRAM   echoRam;
        FakeRAM   fakeRam;
        Joypad    joypad;
    };

    std::unique_ptr<Components> _components;

    std::chrono::steady_clock::time_point _lastUpdate;
    bool                                  _running{true};
};

#endif  // GBEMU_EMULATOR_HXX
