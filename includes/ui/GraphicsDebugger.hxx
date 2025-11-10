//
// Created by plouvel on 09/11/2025.
//

#ifndef GBEMU_GRAPHICS_HXX
#define GBEMU_GRAPHICS_HXX

#include <array>
#include <cstdint>

#include "SDL.hxx"

using Palette      = std::array<uint8_t, 3>;
using TileData     = std::array<uint8_t, 16>;
using TileDataSpan = std::span<const uint8_t, 16>;

class TileGrid
{
  public:
    static constexpr size_t TILE_SIZE{8};

    class OutOfBounds final : public std::exception
    {
    };

    TileGrid(sdl::shared_renderer renderer, size_t col, size_t row, size_t lineSize = 1,
             uint32_t lineColor = 0xFF0000FF);

    void setTile(const TileDataSpan& tileData, size_t x, size_t y);
    void clearTile(size_t x, size_t y);

    [[nodiscard]] size_t getSizeX() const;
    [[nodiscard]] size_t getSizeY() const;
    [[nodiscard]] size_t getCols() const;
    [[nodiscard]] size_t getRows() const;

    [[nodiscard]] sdl::unique_texture getTexture() const;

  private:
    sdl::shared_renderer _sdlRenderer;

    const size_t _lineSize;
    const size_t _col;
    const size_t _row;

    sdl::unique_surface _surface;
    std::span<uint32_t> _surfacePixels;
};

class GraphicsDebugger
{
  public:
    explicit GraphicsDebugger(sdl::shared_renderer renderer);

    void update(const std::array<uint8_t, 0x10000>& videoRam);
    void render();

  private:
    sdl::shared_renderer _sdlRenderer;
    TileGrid             _backgroundGrid;
    sdl::unique_texture  _backgroundTexture;
};

#endif  // GBEMU_GRAPHICS_HXX
