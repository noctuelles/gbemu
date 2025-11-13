//
// Created by plouvel on 12/11/2025.
//

#ifndef GBEMU_FRAMEBUFFER_HXX
#define GBEMU_FRAMEBUFFER_HXX
#include "Tile.hxx"

namespace Graphics
{
    using Framebuffer = std::array<std::array<Tile::ColorIndex, 160>, 144>;
}

#endif  // GBEMU_FRAMEBUFFER_HXX
