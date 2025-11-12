//
// Created by plouvel on 12/11/2025.
//

#ifndef GBEMU_DISPLAY_HXX
#define GBEMU_DISPLAY_HXX
#include "SDL.hxx"

class Display
{
  public:
    using FrameBuffer = std::array<uint8_t, 0x1680>;

    Display(sdl::shared_renderer renderer);

    void render();
    void clear();
    void update(const FrameBuffer& frameBuffer);

  private:
    sdl::shared_renderer renderer;
};

#endif  // GBEMU_DISPLAY_HXX
