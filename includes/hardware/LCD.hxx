//
// Created by plouvel on 4/25/25.
//

#ifndef LCD_HXX
#define LCD_HXX

#include <array>

#include "Displayable.hxx"
#include "SDL2/SDL_render.h"

class LCD final : public Displayable
{
  public:
    explicit LCD(SDL_Renderer& renderer);

    ~LCD() override;

    void update() override;
    void turn_off() override;
    void turn_on() override;
    void put_pixel(std::size_t x, std::size_t y, std::uint8_t color_index) override;

  private:
    static SDL_Color PALETTE[4];

    SDL_Renderer&                        renderer;
    SDL_Texture*                         texture{};
};

#endif  // LCD_HXX
