//
// Created by plouvel on 29/10/2025.
//

#ifndef GBEMU_SDL_HXX
#define GBEMU_SDL_HXX

#include <SDL2/SDL.h>

#include <stdexcept>

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

    operator T() const // NOLINT
    {
        return this->handle;
    }

  protected:
    T handle{};
};

using WrappedSDLWindow =
    SDLObjWrapper<SDL_Window*, SDL_CreateWindow, SDL_DestroyWindow, const char*, int, int, int, int, Uint32>;
using WrappedSDLTexture =
    SDLObjWrapper<SDL_Texture*, SDL_CreateTexture, SDL_DestroyTexture, SDL_Renderer*, Uint32, int, int, int>;
using WrappedSDLRenderer = SDLObjWrapper<SDL_Renderer*, SDL_CreateRenderer, SDL_DestroyRenderer, SDL_Window*, int, Uint32>;

#endif  // GBEMU_SDL_HXX
