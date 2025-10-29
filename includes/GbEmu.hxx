//
// Created by plouvel on 29/10/2025.
//

#ifndef GBEMU_GBEMU_HXX
#define GBEMU_GBEMU_HXX

#include "SDL.hxx"
#include "hardware/Bus.hxx"
#include "hardware/Timer.hxx"
#include "hardware/core/SM83.hxx"
#include "ui/AddressSpaceMemoryEditor.hxx"
#include "ui/Debugger.hxx"

struct FakeRAM final : Addressable
{
    uint8_t read(const uint16_t address) override
    {
        return content[address];
    }
    void write(const uint16_t address, const uint8_t value) override
    {
        content[address] = value;
    }

    [[nodiscard]] AddressableRange get_addressable_range() const noexcept override
    {
        return {std::make_pair(0x0000, 0xFFFF)};
    }

    std::array<uint8_t, 0xFFFF> content{};
};

class GbEmu
{
  public:
    GbEmu();
    ~GbEmu();

    void loop();

  private:

    enum class EmulationState
    {
        NORMAL,
        HALTED,
        SINGLE_CPU_TICK,
    };

    void configureSDL();
    void configureImGui();

    void onCpuMachineCycle();

    WrappedSDLWindow   window;
    WrappedSDLRenderer renderer;
    EmulationState     emulationState{EmulationState::HALTED};
    bool               mainLoopRunning{true};

    Bus bus{};

    SM83    cpu;
    Timer   timer;
    FakeRAM ram{};

    Debugger                 debugger{cpu};
    AddressSpaceMemoryEditor addressSpaceMemoryEditor{bus};
};

#endif  // GBEMU_GBEMU_HXX
