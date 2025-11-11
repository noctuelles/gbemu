//
// Created by plouvel on 09/11/2025.
//

#ifndef GBEMU_GRAPHICS_HXX
#define GBEMU_GRAPHICS_HXX

#include "SDL.hxx"
#include "graphics/Tile.hxx"

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
             const Graphics::RGB& lineColor = {0xD3, 0xD3, 0xD3});

    void setTile(const Graphics::Tile& tile, size_t x, size_t y);
    void clearTile(size_t x, size_t y);

    [[nodiscard]] size_t getPixelWidth() const;
    [[nodiscard]] size_t getPixelHeigh() const;
    [[nodiscard]] size_t getCols() const;
    [[nodiscard]] size_t getRows() const;

    void setLineColor(const Graphics::RGB& color);
    void setColorPalette(const Graphics::ColorPalette& colorPalette);
    void setPaletteRegister(uint8_t paletteRegister);

    [[nodiscard]] sdl::unique_texture getTexture() const;

  private:
    void _drawSeparationLines() const noexcept;

    sdl::shared_renderer   _sdlRenderer;
    uint32_t               _lineColor{};
    const size_t           _lineSize;
    const size_t           _col;
    const size_t           _row;
    Graphics::ColorPalette _colorPalette;
    uint8_t                _paletteRegister;
    sdl::unique_surface    _surface;
    std::span<uint32_t>    _surfacePixels;
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
