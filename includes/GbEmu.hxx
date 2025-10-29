//
// Created by plouvel on 29/10/2025.
//

#ifndef GBEMU_GBEMU_HXX
#define GBEMU_GBEMU_HXX

#include "SDL.hxx"
#include "hardware/WorkRAM.hxx"
#include "hardware/Bus.hxx"
#include "hardware/Timer.hxx"
#include "hardware/core/SM83.hxx"
#include "ui/AddressSpaceMemoryEditor.hxx"
#include "ui/Debugger.hxx"

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
