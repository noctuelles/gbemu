//
// Created by plouvel on 23/11/2025.
//

#ifndef GBEMU_EMULATOR_HXX
#define GBEMU_EMULATOR_HXX

#include <unordered_set>

#include "QtRenderer.hxx"
#include "hardware/Bus.hxx"
#include "hardware/Cartridge.hxx"
#include "hardware/EchoRAM.hxx"
#include "hardware/Joypad.hxx"
#include "hardware/Timer.hxx"
#include "hardware/WorkRAM.hxx"
#include "hardware/core/SM83.hxx"

using namespace std::chrono_literals;

class Emulator final : public QObject
{
    Q_OBJECT

  public:
    class Debugger
    {
      public:
        struct Breakpoint
        {
            enum class Type
            {
                OnPC,
                Conditional,
            };

            uint16_t                     address;
            std::variant<std::monostate> condition;
        };

        explicit Debugger(SM83& cpu);
        ~Debugger() = default;

        void addBreakpoint(const Breakpoint& breakpoint);
        void removeBreakpoint(const Breakpoint& breakpoint);
        void removeBreakpoint(uint16_t address);

        bool shouldBreak() const;

      private:
        SM83&                                    _cpu;
        std::unordered_map<uint16_t, Breakpoint> _breakpoints;
    };

    struct State
    {
        SM83::View cpuView{};
        Bus::View  busView{};
    };

    explicit Emulator(const std::optional<QString>& bootRom = std::nullopt, QObject* parent = nullptr);

  public slots:
    void startEmulation(const QString& path);
    void runFrame();
    bool nextInstruction();
    void stepInInstruction();

    /* Each of these events will be handled in between frames. */
    void onKeyPressed(Key key);
    void onKeyReleased(Key key);
    void setBreakpoint(const Emulator::Debugger::Breakpoint& breakpoint);
    void getEmulationStatus();

  private slots:
    void onRender(const Graphics::Framebuffer& framebuffer);

  signals:
    void breakpointHit(const Emulator::State& state);
    void emulationStatusUpdated(const Emulator::State& state);
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

    volatile bool _running{true};
    QtRenderer*   _renderer;
    Components    _components;
    Debugger      _backEndDebugger;
    State         _status;

    std::chrono::nanoseconds _frameDuration{16740000ns};
};

#endif  // GBEMU_EMULATOR_HXX
