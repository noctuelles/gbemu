//
// Created by plouvel on 13/11/2025.
//

#include "ui/Display.hxx"

#include <ranges>

#include "imgui.h"

Display::Display(sdl::shared_renderer renderer)
    : _renderer{std::move(renderer)},
      _surface{SDL_CreateRGBSurfaceWithFormat(0, WIDTH, HEIGHT, 32, SDL_PIXELFORMAT_RGBA32), SDL_FreeSurface}
{
    if (!_surface)
    {
        throw sdl::exception{"SDL_CreateRGBSurfaceWithFormat"};
    }
}

void Display::render()
{
    ImGui::Begin("Display");
    if (_surface)
    {
        ImGui::Image(static_cast<ImTextureID>(reinterpret_cast<intptr_t>(_texture.get())),
                     ImVec2(_surface->w * 2.f, _surface->h * 2.f));
    }
    ImGui::End();
}

void Display::update(const Graphics::Framebuffer& frameBuffer)
{
    constexpr Graphics::ColorPalette colorPalette{
        Graphics::RGB{0xFF, 0xFF, 0xFF},
        Graphics::RGB{0xAA, 0xAA, 0xAA},
        Graphics::RGB{0x55, 0x55, 0x55},
        Graphics::RGB{0x00, 0x00, 0x00},
    };

    const auto             pixels{static_cast<uint32_t*>(_surface->pixels)};
    sdl::surface_lockguard lock{_surface.get()};

    for (const auto& [y, row] : std::views::enumerate(frameBuffer))
    {
        for (const auto& [x, line] : std::views::enumerate(row))
        {
            const auto& color{colorPalette[line.to_ulong()]};
            pixels[y * WIDTH + x] =
                SDL_MapRGBA(_surface->format, std::get<0>(color), std::get<1>(color), std::get<2>(color), 0xFF);
        }
    }

    sdl::unique_texture texture{SDL_CreateTextureFromSurface(_renderer.get(), _surface.get()), SDL_DestroyTexture};
    _texture = std::move(texture);
}
