//
// Created by plouvel on 12/8/25.
//

#ifndef GBEMU_IRENDERER_HXX
#define GBEMU_IRENDERER_HXX
#include <cstdint>

struct IRenderer
{
    virtual ~IRenderer()                                                = default;
    virtual void setPixel(uint8_t x, uint8_t y, uint8_t pixel) noexcept = 0;
    virtual void render()                                               = 0;
};

#endif  // GBEMU_IRENDERER_HXX
