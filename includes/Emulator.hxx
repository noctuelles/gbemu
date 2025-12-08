//
// Created by plouvel on 23/11/2025.
//

#ifndef GBEMU_EMULATOR_HXX
#define GBEMU_EMULATOR_HXX

#include <QObject>

#include "QtRenderer.hxx"
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
    explicit Emulator(const std::optional<QString>& bootRom = std::nullopt, QObject* parent = nullptr);

  public slots:
    void startEmulation(const QString& path);
    void onKeyPressed(Key key);
    void onKeyReleased(Key key);
    void runFrame();
  private slots:
    void onRender();

  signals:
    void frameReady(const Graphics::Framebuffer& framebuffer);
    void emulationFatalError(const QString& message);

  private:
    class Components
    {
        EmulationState _state;

      public:
        explicit Components(IRenderer& renderer);

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

    bool        _running{true};
    QtRenderer* _renderer;
    Components  _components;

    std::chrono::steady_clock::time_point _lastUpdate;
};

#endif  // GBEMU_EMULATOR_HXX
