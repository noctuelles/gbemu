//
// Created by plouvel on 12/11/2025.
//

#ifndef GBEMU_FRAMEBUFFER_HXX
#define GBEMU_FRAMEBUFFER_HXX
#include <cstdint>

namespace Graphics
{
    using Framebuffer = std::array<std::array<uint8_t, 160>, 144>;
}

#endif  // GBEMU_FRAMEBUFFER_HXX
