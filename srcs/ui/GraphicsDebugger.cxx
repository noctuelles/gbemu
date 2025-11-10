//
// Created by plouvel on 09/11/2025.
//

#include "ui/GraphicsDebugger.hxx"

#include <numeric>
#include <ranges>
#include <utility>

#include "Utils.hxx"
#include "imgui.h"

TileGrid::TileGrid(sdl::shared_renderer renderer, const size_t col, const size_t row, const size_t lineSize,
                   const uint32_t lineColor)
    : _sdlRenderer(std::move(renderer)),
      _lineSize(lineSize),
      _col(col),
      _row(row),
      _surface(SDL_CreateRGBSurfaceWithFormat(0, static_cast<int>(col * TILE_SIZE + (col + 1) * lineSize),
                                              static_cast<int>(row * TILE_SIZE + (row + 1) * lineSize), 32,
                                              SDL_PIXELFORMAT_RGBA32),
               SDL_FreeSurface)
{
    SDL_Rect rect{};
    int      x{};
    int      y{};

    if (!_surface)
    {
        throw std::runtime_error(SDL_GetError());
    }

    _surfacePixels =
        std::span{static_cast<uint32_t*>(_surface->pixels), static_cast<long unsigned int>(_surface->w * _surface->h)};

    while (x < _surface->w)
    {
        rect.x = x;
        rect.y = 0;
        rect.w = 1;
        rect.h = _surface->h;

        SDL_FillRect(_surface.get(), &rect, lineColor);

        x += static_cast<int>(lineSize);
        x += TILE_SIZE;
    }

    while (y < _surface->h)
    {
        rect.x = 0;
        rect.y = y;
        rect.w = _surface->w;
        rect.h = 1;

        SDL_FillRect(_surface.get(), &rect, lineColor);

        y += static_cast<int>(lineSize);
        y += TILE_SIZE;
    }
}

void TileGrid::setTile(const TileDataSpan& tileData, size_t x, size_t y)
{
    if (x >= _col)
    {
        throw OutOfBounds{};
    }

    if (y >= _row)
    {
        throw OutOfBounds{};
    }

    const size_t baseOffsetX{x * TILE_SIZE + (x + 1) * _lineSize};
    size_t       baseOffsetY{y * TILE_SIZE + (y + 1) * _lineSize};

    SDL_LockSurface(_surface.get());

    for (size_t i = 0; i < tileData.size(); i += 2)
    {
        const auto tileDataLow{tileData[i]};
        const auto tileDataHigh{tileData[i + 1]};
        const auto pixels{utils::getPixelsFromTileData(tileDataLow, tileDataHigh)};

        for (const auto [pixelX, pixelValue] : std::views::enumerate(pixels))
        {
            uint32_t color{0x000000FF};

            if (pixelValue == 0)
            {
                color = 0xFFFFFFFF;
            }

            _surfacePixels[baseOffsetY * _surface->w + baseOffsetX + pixelX] = color;
        }

        baseOffsetY += 1;
    }

    SDL_UnlockSurface(_surface.get());
}

void TileGrid::clearTile(size_t x, size_t y)
{
    static TileData data{};

    setTile(data, x, y);
}

size_t TileGrid::getSizeX() const
{
    return _surface->w;
}

size_t TileGrid::getSizeY() const
{
    return _surface->h;
}

size_t TileGrid::getCols() const
{
    return _col;
}

size_t TileGrid::getRows() const
{
    return _row;
}

[[nodiscard]] sdl::unique_texture TileGrid::getTexture() const
{
    sdl::unique_texture texture{SDL_CreateTextureFromSurface(_sdlRenderer.get(), _surface.get()), SDL_DestroyTexture};

    if (!texture)
    {
        throw std::runtime_error(SDL_GetError());
    }

    return texture;
}

void GraphicsDebugger::render()
{
    ImGui::Begin("GraphicsDebugger", nullptr,
                 ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoResize);
    if (_backgroundTexture)
    {
        ImGui::Image(static_cast<ImTextureID>(reinterpret_cast<intptr_t>(_backgroundTexture.get())),
                     ImVec2(static_cast<float>(_backgroundGrid.getSizeX() * 3),
                            static_cast<float>(_backgroundGrid.getSizeY() * 3)));
    }
    ImGui::End();
}

GraphicsDebugger::GraphicsDebugger(sdl::shared_renderer renderer)
    : _sdlRenderer(renderer), _backgroundGrid(renderer, 16, 24, 1, 0xFFD3D3D3)
{
}

void GraphicsDebugger::update(const std::array<uint8_t, 65536>& videoRam)
{
    (void) videoRam;

    uint16_t baseAddress{0x8000};
    (void) baseAddress;

    for (size_t tileY = 0; tileY < _backgroundGrid.getRows(); tileY++)
    {
        for (size_t tileX = 0; tileX < _backgroundGrid.getCols(); tileX++)
        {
            TileDataSpan data{&videoRam[baseAddress], 16};

            _backgroundGrid.setTile(data, tileX, tileY);

            baseAddress += 0x10;
        }
    }

    _backgroundTexture = _backgroundGrid.getTexture();
}
