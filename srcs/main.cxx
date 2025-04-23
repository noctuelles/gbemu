//
// Created by plouvel on 12/8/24.
//

#include <SDL2/SDL.h>
#include <stdio.h>

#include <iostream>
#include <ostream>
#include <stdexcept>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#include <imgui_impl_sdlrenderer2.h>

#include <hardware/Bus.hxx>
#include <memory>
#include <vector>

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl2.h"
#include "imgui_memory_editor/imgui_memory_editor.h"

template <typename T, auto Init, auto Release, typename... InitArgs>
class SDLObjWrapper final
{
  public:
    explicit SDLObjWrapper(InitArgs... args) : handle(Init(args...))
    {
        if (!this->handle)
        {
            throw std::runtime_error("Failed to initialize SDL resource.");
        }
    }

    ~SDLObjWrapper()
    {
        Release(handle);
    }

    SDLObjWrapper(const SDLObjWrapper&)            = delete;
    SDLObjWrapper& operator=(const SDLObjWrapper&) = delete;
    SDLObjWrapper(SDLObjWrapper&&)                 = default;
    SDLObjWrapper& operator=(SDLObjWrapper&&)      = default;

    T operator->() const
    {
        return this->handle;
    }

    operator T() const  // NOLINT
    {
        return this->handle;
    }

  protected:
    T handle{};
};

using SDLWindow =
    SDLObjWrapper<SDL_Window*, SDL_CreateWindow, SDL_DestroyWindow, const char*, int, int, int, int, Uint32>;
using SDLTexture =
    SDLObjWrapper<SDL_Texture*, SDL_CreateTexture, SDL_DestroyTexture, SDL_Renderer*, Uint32, int, int, int>;
using SDLRenderer = SDLObjWrapper<SDL_Renderer*, SDL_CreateRenderer, SDL_DestroyRenderer, SDL_Window*, int, Uint32>;

void destroyRenderer(SDL_Renderer* renderer)
{
    SDL_DestroyRenderer(renderer);
}

using SDLRendererPtr = std::unique_ptr<SDL_Renderer, decltype(&destroyRenderer)>;

auto make_sdl_renderer(SDL_Window *window, const int opt, Uint32 flags)
{
    return SDLRendererPtr{SDL_CreateRenderer(window, opt, flags), &destroyRenderer};
}

class GbEmu
{
  public:
    GbEmu()
        : window("gbemu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 800, SDL_WINDOW_SHOWN),
          renderer(window, -1, 0)
    {
        if (SDL_Init(SDL_INIT_VIDEO) != 0)
        {
            throw std::runtime_error("Failed to initialize SDL");
        }

        ImGui::CreateContext();

        ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
        ImGui_ImplSDLRenderer2_Init(renderer);
    }

    void loop()
    {
        SDL_Event        event{};
        std::vector<uint8_t> memory(0x10000);

        while (running)
        {
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    running = false;
                }
                ImGui_ImplSDL2_ProcessEvent(&event);
            }
            ImGui_ImplSDLRenderer2_NewFrame();
            ImGui_ImplSDL2_NewFrame();
            ImGui::NewFrame();

            static MemoryEditor editor;

            editor.DrawWindow("Test", memory.data(), memory.size());

            ImGui::Render();

            // ImGui::ShowDemoWindow();

            SDL_RenderClear(renderer);

            ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);

            SDL_RenderPresent(renderer);
        }
    }

    ~GbEmu()
    {
        ImGui_ImplSDLRenderer2_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
        SDL_Quit();
    }

  private:
    SDLWindow   window;
    SDLRenderer renderer;
    bool        running{true};
};

int main(int argc, char* args[])
{
    (void)argc;
    (void)args;
    GbEmu emu{};
    Bus bus{};

    emu.loop();

    return 0;
}