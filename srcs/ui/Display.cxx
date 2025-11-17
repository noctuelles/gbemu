//
// Created by plouvel on 13/11/2025.
//

#include "ui/Display.hxx"

#include <ranges>

#include "imgui.h"

Display::Display(sdl::shared_renderer renderer)
    : _renderer{std::move(renderer)},
      _texture(SDL_CreateTexture(_renderer.get(), SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT),
               SDL_DestroyTexture)
{
    if (!_texture)
    {
        throw sdl::exception{"SDL_CreateTexture"};
    }

    _format = sdl::unique_format{SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32)};

    if (!_format)
    {
        throw sdl::exception{"SDL_AllocFormat"};
    }
}

void Display::render()
{
    ImGui::Begin("Display");
    ImGui::Image(static_cast<ImTextureID>(reinterpret_cast<intptr_t>(_texture.get())),
                 ImVec2(WIDTH * 2.f, HEIGHT * 2.f));
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
    uint32_t* pixels{nullptr};
    int       pitch{};

    SDL_LockTexture(_texture.get(), nullptr, reinterpret_cast<void**>(&pixels), &pitch);

    for (const auto& [y, row] : std::views::enumerate(frameBuffer))
    {
        for (const auto& [x, line] : std::views::enumerate(row))
        {
            const auto& color{colorPalette[line.to_ulong()]};
            pixels[y * WIDTH + x] =
                SDL_MapRGBA(_format.get(), std::get<0>(color), std::get<1>(color), std::get<2>(color), 0xFF);
        }
    }

    SDL_UnlockTexture(_texture.get());
}
