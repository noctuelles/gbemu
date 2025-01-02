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

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

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
        std::cout << "Releasing" << std::endl;
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

    operator T() const
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

class GbEmu
{
  public:
    GbEmu()
        : window("gbemu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
                 SDL_WINDOW_SHOWN)
    {
        if (SDL_Init(SDL_INIT_VIDEO) != 0)
        {
            throw std::runtime_error("Failed to initialize SDL");
        }

        SDL_Surface* screenSurface{nullptr};

        screenSurface = SDL_GetWindowSurface(window);
        SDL_FillRect(screenSurface, nullptr, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
        SDL_UpdateWindowSurface(window);
        SDL_Delay(2000);
    }

    ~GbEmu()
    {
        std::cout << "GbEmu Destructor" << std::endl;
        SDL_Quit();
    }

  private:
    SDLWindow window;
};

int main(int argc, char* args[])
{
    GbEmu emu{};

    return 0;
}