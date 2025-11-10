//
// Created by plouvel on 29/10/2025.
//

#include "GbEmu.hxx"

#include "Emulator.hxx"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include "ui/ForkAwesomeFont.hxx"

GbEmu::GbEmu()
    : window(SDL_CreateWindow("gbemu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1300, 800,
                              SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE),
             SDL_DestroyWindow),
      renderer(SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED), SDL_DestroyRenderer),
      emu(eventQueue),
      emuThread(std::ref(emu)),
      graphicsDebugger(renderer)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        throw std::runtime_error("Failed to initialize SDL");
    }

    configureImGui();
}

GbEmu::~GbEmu()
{
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();

    ImGui::DestroyContext();

    SDL_Quit();
}

void GbEmu::pushEvent(const Emulator::Event& event)
{
    eventQueue.push(event);
}

void GbEmu::loop()
{
    SDL_Event   event{};
    const auto& io = ImGui::GetIO();

    while (mainLoopRunning)
    {
        if (auto eventPayload{eventQueue.try_pop()}; eventPayload.has_value())
        {
            emuEvent = eventPayload.value();

            switch (eventPayload.value().type)
            {
                case Emulator::Event::Type::Paused:

                    debugger.setCpuView(emuEvent.view);
                    debugger.setAddressSpace(emuEvent.addressSpace);

                    debugger.setScrollToCurrentInstruction();
                    debugger.setDisabled(false);

                    memEditor.setAddressSpace(emuEvent.addressSpace);

                    graphicsDebugger.update(emuEvent.addressSpace);

                    break;
                case Emulator::Event::Type::BreakpointRemoved:
                case Emulator::Event::Type::BreakpointSet:
                    /* TODO */
                    break;
            }
        }

        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
            {
                mainLoopRunning = false;
            }
        }

        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();

        ImGui::NewFrame();
        ImGui::DockSpaceOverViewport();

        uiCommands.emplace(debugger.render());
        memEditor.render();
        graphicsDebugger.render();

        ImGui::Render();

        SDL_RenderSetScale(renderer.get(), io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        SDL_RenderClear(renderer.get());
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer.get());
        SDL_RenderPresent(renderer.get());

        while (!uiCommands.empty())
        {
            if (uiCommands.front().has_value())
            {
                emu.pushCommand(uiCommands.front().value());
            }
            uiCommands.pop();
        }
    }

    emu.pushCommand(Emulator::Command(Emulator::Command::Type::Exit, {}));
}

void GbEmu::configureSDL() {}

void GbEmu::configureImGui()
{
    ImFontConfig         iconsConfig{};
    auto                 iconFontSize{13.0f};
    static const ImWchar iconsRanges[]{ICON_MIN_FK, ICON_MAX_16_FK, 0};

    IMGUI_CHECKVERSION();

    ImGui::CreateContext();

    ImGui_ImplSDL2_InitForSDLRenderer(window.get(), renderer.get());
    ImGui_ImplSDLRenderer2_Init(renderer.get());

    ImGuiIO& io = ImGui::GetIO();

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    io.Fonts->AddFontDefault();

    iconsConfig.MergeMode        = true;
    iconsConfig.PixelSnapH       = true;
    iconsConfig.GlyphMinAdvanceX = iconFontSize;
    io.Fonts->AddFontFromFileTTF(FONT_ICON_FILE_NAME_FK, iconFontSize, &iconsConfig, iconsRanges);
}
