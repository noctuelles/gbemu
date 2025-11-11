//
// Created by plouvel on 09/11/2025.
//

#ifndef GBEMU_GRAPHICS_HXX
#define GBEMU_GRAPHICS_HXX

#include <array>
#include <cstdint>

#include "SDL.hxx"
#include "Utils.hxx"

using RGB = std::tuple<uint8_t, uint8_t, uint8_t>;

using Palette      = std::array<RGB, 3>;
using TileData     = std::array<uint8_t, 16>;
using TileDataSpan = std::span<const uint8_t, 16>;

class TileGrid
{
  public:
    static constexpr size_t TILE_SIZE{8};

    class OutOfBounds final : public std::exception
    {
      public:
        OutOfBounds(size_t x, size_t y, size_t cols, size_t rows) : _x{x}, _y{y}, _cols{cols}, _rows{rows} {}

        [[nodiscard]] const char* what() const noexcept override
        {
            return std::format("Coordinate {} and {} are out of bounds : max is {} {}", _x, _y, _cols, _rows).c_str();
        }

      private:
        size_t _x;
        size_t _y;
        size_t _cols;
        size_t _rows;
    };

    TileGrid(sdl::shared_renderer renderer, size_t col, size_t row, size_t lineSize = 1,
             const RGB& lineColor = {0xD3, 0xD3, 0xD3});

    void setTile(const utils::Tile& tile, size_t x, size_t y);
    void clearTile(size_t x, size_t y);

    [[nodiscard]] size_t getPixelWidth() const;
    [[nodiscard]] size_t getPixelHeigh() const;
    [[nodiscard]] size_t getCols() const;
    [[nodiscard]] size_t getRows() const;

    void setLineColor(const RGB& color);

    [[nodiscard]] sdl::unique_texture getTexture() const;

  private:
    void _drawSeparationLines() const noexcept;

    sdl::shared_renderer _sdlRenderer;

    uint32_t     _lineColor{};
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

    TileGrid            _backgroundGrid;
    sdl::unique_texture _backgroundTexture;
};

#endif  // GBEMU_GRAPHICS_HXX
