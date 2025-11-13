//
// Created by plouvel on 12/11/2025.
//

#ifndef GBEMU_DISPLAY_HXX
#define GBEMU_DISPLAY_HXX

#include "SDL.hxx"
#include "graphics/Framebuffer.hxx"

class Display
{
  public:
    static constexpr size_t WIDTH  = 160;
    static constexpr size_t HEIGHT = 144;

    explicit Display(sdl::shared_renderer renderer);

    void render();
    void update(const Graphics::Framebuffer& frameBuffer);

  private:
    sdl::shared_renderer _renderer;
    sdl::unique_surface  _surface;
    sdl::unique_texture  _texture;
};

#endif  // GBEMU_DISPLAY_HXX
