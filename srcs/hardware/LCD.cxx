//
// Created by plouvel on 4/25/25.
//

#include "hardware/LCD.hxx"

#include "SDL2.hxx"

SDL_Color LCD::PALETTE[4] = {
    {255, 255, 255, 255},
    {255, 192, 192, 192},
    {255, 96, 96, 96},
    {255, 0, 0, 0},
};

LCD::LCD(SDL_Renderer& renderer) : renderer(renderer)
{
    SDL_SetRenderDrawColor(&renderer, PALETTE[0].r, PALETTE[0].g, PALETTE[0].b, PALETTE[0].a);
    texture = SDL_CreateTexture(&renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
    if (texture == nullptr)
    {
        throw std::runtime_error{"Failed to create LCD texture."};
    }
}

LCD::~LCD()
{
    if (texture == nullptr)
    {
        return;
    }

    SDL_DestroyTexture(texture);
}

void LCD::update()
{
    SDL_RenderCopy(&renderer, texture, nullptr, nullptr);
}

void LCD::turn_off()
{
    SDL_RenderClear(&renderer);
}

void LCD::turn_on()
{
    SDL_RenderPresent(&renderer);
}

void LCD::put_pixel(const std::size_t x, const std::size_t y, const std::uint8_t color_index)
{
    Displayable::put_pixel(x, y, color_index);

    const auto [r, g, b, a]{PALETTE[color_index & 0x03]};
    uint32_t* pixels{nullptr};
    int      pitch{};

    SDL_LockTexture(texture, nullptr, reinterpret_cast<void**>(&pixels), &pitch);
    pixels[y * WIDTH + x] = a << 24 | r << 16 | g << 8 | b;
    SDL_UnlockTexture(texture);
}