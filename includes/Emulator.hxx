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
    using Breakpoints = std::set<uint16_t>;

    static const std::array<uint8_t, 0x100> BOOT_ROM;
    static constexpr uint16_t               BOOT_ROM_START_ADDRESS{0x0000};

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
        };

        Type type;

        SM83::View                   view;
        std::array<uint8_t, 0x10000> addressSpace;
    };

    explicit Emulator(utils::ThreadSafeQueue<Event>& eventQueue);

    void pushCommand(const Command& cmd);
    void operator()();

  private:
    void onCpuMachineCycle();
    void pushEvent(Event::Type type) const;

    std::condition_variable         cmdCv;
    std::mutex                      cmdMutex;
    utils::ThreadSafeQueue<Command> cmdQueue;
    utils::ThreadSafeQueue<Event>&  uiEventQueue;

    bool running{true};
    bool paused{true};
    bool requestedPause{false};

    Bus     bus;
    SM83    cpu;
    Timer   timer;
    FakeRAM ram;

    Breakpoints breakpoints;
};

#endif  // GBEMU_EMULATOR_HXX
