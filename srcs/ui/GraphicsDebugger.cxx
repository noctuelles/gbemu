//
// Created by plouvel on 09/11/2025.
//

#include "ui/GraphicsDebugger.hxx"

#include <ranges>
#include <utility>

#include "graphics/Tile.hxx"
#include "imgui.h"

TileGrid::TileGrid(sdl::shared_renderer renderer, const size_t col, const size_t row, const bool showGrid,
                   const size_t lineSize, const Graphics::RGB& lineColor)
    : _sdlRenderer(std::move(renderer)),
      _showGrid(showGrid),
      _lineSize(lineSize),
      _col(col),
      _row(row),
      _surface(SDL_CreateRGBSurfaceWithFormat(0, static_cast<int>(_calculatePixelWidth()),
                                              static_cast<int>(_calculatePixelHeigh()), 32, SDL_PIXELFORMAT_RGBA32),
               SDL_FreeSurface)
{
    if (!_surface)
    {
        throw sdl::exception{"SDL_CreateRGBSurfaceWithFormat"};
    }

    _surfacePixels =
        std::span{static_cast<uint32_t*>(_surface->pixels), static_cast<long unsigned int>(_surface->w * _surface->h)};

    setLineColor(lineColor);
}

void TileGrid::setTile(const Graphics::Tile& tile, const size_t x, const size_t y)
{
    if (x >= _col || y >= _row)
    {
        throw OutOfBounds{x, y, _col, _row};
    }

    size_t baseOffsetX{x * TILE_SIZE};
    size_t baseOffsetY{y * TILE_SIZE};

    if (_showGrid)
    {
        baseOffsetX += (x + 1) * _lineSize;
        baseOffsetY += (y + 1) * _lineSize;
    }

    sdl::surface_lockguard lock{_surface.get()};

    for (size_t i = 0; i < TILE_SIZE; i++)
    {
        for (size_t j = 0; j < TILE_SIZE; j++)
        {
            const auto color{Graphics::getPixelColorFromPalette(tile[i][j], _paletteRegister, _colorPalette)};
            _surfacePixels[baseOffsetY * _surface->w + baseOffsetX + j] =
                SDL_MapRGBA(_surface->format, std::get<0>(color), std::get<1>(color), std::get<2>(color), 0xFF);
        }

        baseOffsetY += 1;
    }
}

void TileGrid::clearTile(size_t x, size_t y)
{
    (void) x;
    (void) y;
}

size_t TileGrid::getPixelWidth() const
{
    return _surface->w;
}

size_t TileGrid::getPixelHeigh() const
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

void TileGrid::setLineColor(const Graphics::RGB& color) noexcept
{
    _lineColor = SDL_MapRGBA(_surface->format, std::get<0>(color), std::get<1>(color), std::get<2>(color), 0xFF);

    _drawGridLines();
}

void TileGrid::setColorPalette(const Graphics::ColorPalette& colorPalette) noexcept
{
    _colorPalette = colorPalette;
}

void TileGrid::setPaletteRegister(const uint8_t paletteRegister) noexcept
{
    _paletteRegister = paletteRegister;
}

[[nodiscard]] sdl::unique_texture TileGrid::getTexture() const
{
    sdl::unique_texture texture{SDL_CreateTextureFromSurface(_sdlRenderer.get(), _surface.get()), SDL_DestroyTexture};

    if (!texture)
    {
        throw sdl::exception{"SDL_CreateTextureFromSurface"};
    }

    return texture;
}

void TileGrid::_drawGridLines() const noexcept
{
    SDL_Rect rect{};
    int      x{};
    int      y{};

    if (!_showGrid)
    {
        return;
    }

    while (x < _surface->w)
    {
        rect.x = x;
        rect.y = 0;
        rect.w = 1;
        rect.h = _surface->h;

        SDL_FillRect(_surface.get(), &rect, _lineColor);

        x += static_cast<int>(_lineSize);
        x += TILE_SIZE;
    }

    while (y < _surface->h)
    {
        rect.x = 0;
        rect.y = y;
        rect.w = _surface->w;
        rect.h = 1;

        SDL_FillRect(_surface.get(), &rect, _lineColor);

        y += static_cast<int>(_lineSize);
        y += TILE_SIZE;
    }
}

size_t TileGrid::_calculatePixelWidth() const noexcept
{
    size_t pixelWidth{_col * TILE_SIZE};

    if (_showGrid)
    {
        pixelWidth += (_col + 1) * _lineSize;
    }

    return pixelWidth;
}

size_t TileGrid::_calculatePixelHeigh() const noexcept
{
    size_t pixelHeigh{_row * TILE_SIZE};

    if (_showGrid)
    {
        pixelHeigh += (_row + 1) * _lineSize;
    }

    return pixelHeigh;
}

void GraphicsDebugger::render()
{
    ImGui::Begin("GraphicsDebugger", nullptr,
                 ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoResize);
    if (ImGui::BeginTabBar("##graphicsDebuggerTabs", ImGuiTabBarFlags_None))
    {
        if (ImGui::BeginTabItem("VRAM"))
        {
            auto optionBarHeight{ImGui::GetFontSize() * 2.f + ImGui::GetStyle().FramePadding.y * 2.f};
            auto availableSpace{ImGui::GetContentRegionAvail()};

            ImGui::BeginChild("##vramGrid", ImVec2(availableSpace.x, availableSpace.y - optionBarHeight),
                              ImGuiChildFlags_None, ImGuiWindowFlags_None);
            {
                if (_backgroundTexture)
                {
                    ImGui::Spacing();
                    ImGui::Image(static_cast<ImTextureID>(reinterpret_cast<intptr_t>(_backgroundTexture.get())),
                                 ImVec2(_backgroundGrid.getPixelWidth() * 2.f, _backgroundGrid.getPixelHeigh() * 2.f));
                }
            }
            ImGui::EndChild();
            ImGui::Spacing();
            ImGui::Separator();

            ImGui::BeginChild("##vramOptions", ImVec2(0, optionBarHeight), ImGuiChildFlags_None, ImGuiWindowFlags_None);
            {
                if (ImGui::Button("Options"))
                {
                    ImGui::OpenPopup("vramOptionsPopup");
                }
                if (ImGui::BeginPopup("vramOptionsPopup"))
                {
                    ImGui::Text("Hi");
                    ImGui::EndPopup();
                }
            }
            ImGui::EndChild();

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Tile Maps"))
        {
            if (_tileMapTexture)
            {
                ImGui::Spacing();
                ImGui::Image(static_cast<ImTextureID>(reinterpret_cast<intptr_t>(_tileMapTexture.get())),
                             ImVec2(_tileMapGrid.getPixelWidth() * 1.f, _tileMapGrid.getPixelHeigh() * 1.f));
            }
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Sprites"))
        {
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Palette"))
        {
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
}

GraphicsDebugger::GraphicsDebugger(sdl::shared_renderer renderer)
    : _sdlRenderer(renderer), _backgroundGrid(renderer, 16, 24), _tileMapGrid(renderer, 32, 32)
{
    constexpr Graphics::ColorPalette colorPalette{
        Graphics::RGB{0xFF, 0xFF, 0xFF},
        Graphics::RGB{0xAA, 0xAA, 0xAA},
        Graphics::RGB{0x55, 0x55, 0x55},
        Graphics::RGB{0x00, 0x00, 0x00},
    };

    _backgroundGrid.setColorPalette(colorPalette);
    _backgroundGrid.setPaletteRegister(0b11111100);

    _tileMapGrid.setColorPalette(colorPalette);
    _tileMapGrid.setPaletteRegister(0b11111100);
}

void GraphicsDebugger::update(const std::array<uint8_t, 65536>& videoRam)
{
    uint16_t baseAddress{0x8000};
    uint16_t baseAddressTileMap{0x9800};

    for (size_t tileY = 0; tileY < _backgroundGrid.getRows(); tileY++)
    {
        for (size_t tileX = 0; tileX < _backgroundGrid.getCols(); tileX++)
        {
            const Graphics::Tile::DataSpan data{&videoRam[baseAddress], 16};

            _backgroundGrid.setTile(Graphics::Tile{data}, tileX, tileY);

            baseAddress += 0x10;
        }
    }

    baseAddress = 0x8000;

    for (size_t tileY = 0; tileY < _tileMapGrid.getRows(); tileY++)
    {
        for (size_t tileX = 0; tileX < _tileMapGrid.getCols(); tileX++)
        {
            const auto                     index{videoRam[baseAddressTileMap]};
            const Graphics::Tile::DataSpan data{&videoRam[baseAddress + index * 0x10], 16};

            _tileMapGrid.setTile(Graphics::Tile{data}, tileX, tileY);

            baseAddressTileMap += 1;
        }
    }

    _backgroundTexture = _backgroundGrid.getTexture();
    _tileMapTexture    = _tileMapGrid.getTexture();
}
