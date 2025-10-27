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

#include <memory>
#include <vector>

#include "ForkAwesomeFont.hxx"
#include "hardware/LCD.hxx"
#include "imgui.h"
#include "imgui_impl_sdl2.h"

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

auto make_sdl_renderer(SDL_Window* window, const int opt, Uint32 flags)
{
    return SDLRendererPtr{SDL_CreateRenderer(window, opt, flags), &destroyRenderer};
}

class GbEmu
{
  public:
    GbEmu()
        : window("gbemu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, SDL_WINDOW_SHOWN),
          renderer(window, -1, 0)
    {
        if (SDL_Init(SDL_INIT_VIDEO) != 0)
        {
            throw std::runtime_error("Failed to initialize SDL");
        }
        IMGUI_CHECKVERSION();

        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();

        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

        io.Fonts->AddFontDefault();

        float iconFontSize = 13.0f;

        static const ImWchar icons_ranges[] = { ICON_MIN_FK, ICON_MAX_16_FK, 0 };
        ImFontConfig icons_config;
        icons_config.MergeMode = true;
        icons_config.PixelSnapH = true;
        icons_config.GlyphMinAdvanceX = iconFontSize;
        io.Fonts->AddFontFromFileTTF( FONT_ICON_FILE_NAME_FK, iconFontSize, &icons_config, icons_ranges );     }

    void loop()
    {
        SDL_Event   event{};
        auto& io = ImGui::GetIO();

        ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
        ImGui_ImplSDLRenderer2_Init(renderer);

        while (running)
        {
            while (SDL_PollEvent(&event))
            {
                ImGui_ImplSDL2_ProcessEvent(&event);
                if (event.type == SDL_QUIT)
                {
                    running = false;
                }
            }

            ImGui_ImplSDLRenderer2_NewFrame();
            ImGui_ImplSDL2_NewFrame();

            ImGui::NewFrame();

            ImGui::Begin("Debugger");
            ImGui::BeginGroup();
            {
                ImGui::Button(ICON_FK_PLAY);
                if (ImGui::IsItemHovered())
                {
                    ImGui::SetTooltip("Continue");
                }
                ImGui::SameLine();
                ImGui::Button(ICON_FK_ARROW_DOWN);
                if (ImGui::IsItemHovered())
                {
                    ImGui::SetTooltip("Step in");
                }
                ImGui::SameLine();
                ImGui::Button(ICON_FK_ARROW_RIGHT);
                if (ImGui::IsItemHovered())
                {
                    ImGui::SetTooltip("Step over");
                }
            }
            ImGui::EndGroup();
            ImGui::Separator();

            if (ImGui::BeginListBox("I", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
            {
                ImGui::EndListBox();
            }
            ImGui::Separator();
            ImGui::End();

            ImGui::ShowDemoWindow();

            ImGui::Render();

            SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
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

// Window size
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

// Image size
#define IMAGE_WIDTH 256
#define IMAGE_HEIGHT 256

int main(int argc, char* args[])
{
    (void)argc;
    (void)args;
    GbEmu emu{};

    emu.loop();

    return 0;
}