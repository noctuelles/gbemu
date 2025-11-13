//
// Created by plouvel on 29/10/2025.
//

#ifndef GBEMU_GBEMU_HXX
#define GBEMU_GBEMU_HXX

#include <thread>

#include "Emulator.hxx"
#include "SDL.hxx"
#include "ui/AddressSpaceMemoryEditor.hxx"
#include "ui/Debugger.hxx"
#include "ui/Display.hxx"
#include "ui/GraphicsDebugger.hxx"

class GbEmu
{
  public:
    GbEmu();
    ~GbEmu();

    void pushEvent(const Emulator::Event& event);

    void loop();

  private:
    utils::ThreadSafeQueue<Emulator::Event>      eventQueue;
    std::queue<std::optional<Emulator::Command>> uiCommands;

    void configureSDL();
    void configureImGui();

    sdl::unique_window   window;
    sdl::shared_renderer renderer;

    Emulator        emu;
    Emulator::Event emuEvent;
    std::jthread    emuThread;

    Display                  display;
    Debugger                 debugger;
    AddressSpaceMemoryEditor memEditor;
    GraphicsDebugger         graphicsDebugger;

    bool mainLoopRunning{true};
};

#endif  // GBEMU_GBEMU_HXX
