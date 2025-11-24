//
// Created by plouvel on 23/11/2025.
//

#ifndef GBEMU_RENDERER_HXX
#define GBEMU_RENDERER_HXX

#include "graphics/Tile.hxx"

struct IRenderer
{
    virtual ~IRenderer()                                                                             = default;
    virtual void setPixel(const Graphics::Tile::ColorIndex& colorIndex, size_t x, size_t y) noexcept = 0;
    virtual void frameReady()                                                                        = 0;
};

#endif  // GBEMU_RENDERER_HXX
