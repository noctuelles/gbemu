//
// Created by plouvel on 12/11/2025.
//

#ifndef GBEMU_FRAMEBUFFER_HXX
#define GBEMU_FRAMEBUFFER_HXX

#include <array>
#include <cstdint>

namespace Graphics
{
    /**
     * @brief Represents a single pixel in the framebuffer.
     *
     * The pixel is encoded as an 8-bit value with the following bit layout:
     *
     *   Bit:  7  6  5  4  3  2  1  0
     *         [unused ] [type] [color]
     *
     * - Bits 0-1: Color index (0-3) after palette application.
     * - Bits 2-3: Pixel type (see PixelType), indicates the source of the pixel:
     *             0 = Background, 1 = Window, 2 = Object (sprite).
     * - Bits 4-7: Unused.
     *
     * This encoding allows the debugger/UI to extract both the final color
     * and the origin of the pixel for debugging or visualization purposes.
     */
    using Pixel = uint8_t;

    using Framebuffer = std::array<std::array<Pixel, 160>, 144>;

    struct PixelType
    {
        static constexpr uint8_t Background{0x00};
        static constexpr uint8_t Window{0x01};
        static constexpr uint8_t Object{0x02};
    };

}  // namespace Graphics

#endif  // GBEMU_FRAMEBUFFER_HXX
