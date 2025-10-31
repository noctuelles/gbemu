//
// Created by plouvel on 29/10/2025.
//

#ifndef GBEMU_GBEMU_HXX
#define GBEMU_GBEMU_HXX

#include <thread>

#include "Emulator.hxx"
#include "SDL.hxx"
#include "ui/Debugger.hxx"

class GbEmu
{
  public:
    GbEmu();
    ~GbEmu();

    void pushEvent(const Emulator::Event& event);

    void loop();

  private:
    utils::ThreadSafeQueue<Emulator::Event> eventQueue;

    void configureSDL();
    void configureImGui();

    WrappedSDLWindow   window;
    WrappedSDLRenderer renderer;

    Emulator     emu;
    std::jthread emuThread;

    Debugger debugger;

    bool mainLoopRunning{true};
};

#endif  // GBEMU_GBEMU_HXX
