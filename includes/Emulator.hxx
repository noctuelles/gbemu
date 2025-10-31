//
// Created by plouvel on 30/10/2025.
//

#ifndef GBEMU_EMULATOR_HXX
#define GBEMU_EMULATOR_HXX

#include <set>

#include "Utils.hxx"
#include "hardware/Bus.hxx"
#include "hardware/Timer.hxx"
#include "hardware/WorkRAM.hxx"
#include "hardware/core/SM83.hxx"

class GbEmu;
class Emulator
{
    static const std::array<uint8_t, 0x100> BOOT_ROM;
    static constexpr uint16_t               BOOT_ROM_START_ADDRESS{0x0000};

  public:
    struct Command
    {
        struct Breakpoint
        {
            uint16_t address;
        };

        enum class Type
        {
            Continue,
            Step,
            SetBreakpoint,
            RemoveBreakpoint,
        };

        Type                                     type;
        std::variant<std::monostate, Breakpoint> payload;
    };

    struct Event
    {
        enum class Type
        {
            Paused,
        };

        Type type;

        SM83::View                        view;
        std::span<const uint8_t, 0x10000> addressSpace;
    };

    explicit Emulator(GbEmu& gbEmu);

    void pushCommand(const Command& cmd);
    void operator()();

  private:
    void onCpuMachineCycle();
    void pushEvent(Event::Type type) const;

    GbEmu& gbEmu;

    utils::ThreadSafeQueue<Command> cmdQueue;

    bool running{true};
    bool paused{true};
    bool requestedPause{false};

    Bus     bus;
    SM83    cpu;
    Timer   timer;
    FakeRAM ram;

    std::set<uint16_t> breakpoints;
};

#endif  // GBEMU_EMULATOR_HXX
