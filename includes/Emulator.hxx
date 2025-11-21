//
// Created by plouvel on 30/10/2025.
//

#ifndef GBEMU_EMULATOR_HXX
#define GBEMU_EMULATOR_HXX

#include <set>

#include "Utils.hxx"
#include "graphics/Tile.hxx"
#include "hardware/Bus.hxx"
#include "hardware/EchoRAM.hxx"
#include "hardware/PPU.hxx"
#include "hardware/Timer.hxx"
#include "hardware/WorkRAM.hxx"
#include "hardware/core/SM83.hxx"

class GbEmu;
class Emulator
{
    using Breakpoints = std::set<uint16_t>;

    static const std::array<uint8_t, 0x100 + 52> BOOT_ROM;
    static constexpr uint16_t                    BOOT_ROM_START_ADDRESS{0x0000};

  public:
    struct Command
    {
        struct Breakpoint
        {
            uint16_t address;
        };

        struct WriteAddressSpace
        {
            uint16_t address;
            uint8_t  byte;
        };

        enum class Type
        {
            Continue,
            Step,
            SetBreakpoint,
            RemoveBreakpoint,
            WriteAddressSpace,
            Exit
        };

        Type                                                        type;
        std::variant<std::monostate, Breakpoint, WriteAddressSpace> payload;
    };

    struct Event
    {
        enum class Type
        {
            Paused,
            BreakpointSet,
            BreakpointRemoved,
            FramebufferReady,
        };

        struct Paused
        {
            SM83::View                   view;
            std::array<uint8_t, 0x10000> addressSpace;
        };

        struct FramebufferReady
        {
            Graphics::Framebuffer framebuffer;
        };

        using Payload = std::variant<std::monostate, Paused, FramebufferReady>;

        Type    type;
        Payload payload;
    };

    explicit Emulator(Utils::ThreadSafeQueue<Event>& eventQueue);

    void pushCommand(const Command& cmd);
    void operator()();

  private:
    void onCpuMachineCycle();
    void onFramebufferReady() const;
    void pushEvent(Event::Type event, Event::Payload payload) const;

    std::condition_variable         cmdCv;
    std::mutex                      cmdMutex;
    Utils::ThreadSafeQueue<Command> cmdQueue;
    Utils::ThreadSafeQueue<Event>&  uiEventQueue;

    bool running{true};
    bool paused{true};
    bool requestedPause{false};

    EmulationState        emulationState{};
    Bus                   bus;
    Timer                 timer;
    PPU                   ppu;
    FakeRAM               ram;
    SM83                  cpu;
    EchoRAM               echoRam;
    Graphics::Framebuffer framebuffer;

    Breakpoints breakpoints;
};

#endif  // GBEMU_EMULATOR_HXX
