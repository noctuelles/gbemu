//
// Created by plouvel on 1/10/25.
//

#ifndef PPU_HXX
#define PPU_HXX

#include <array>

#include "Addressable.hxx"

class PPU final : public Component
{
  public:
    explicit PPU(Addressable& bus);

    enum class Mode
    {
        HorizontalBlank = 0,
        VerticalBlank   = 1,
        OAMScan         = 2,
        Drawing         = 3,
    };

    struct OAMEntry
    {
        uint8_t Y{};
        uint8_t X{};
        uint8_t tile_index{};
        uint8_t flags{};
    };

    uint8_t read(uint16_t address) override;
    void    write(uint16_t address, uint8_t value) override;
    void    tick() override;

  private:
    Addressable& bus;

    std::array<uint8_t, 0x2000> video_ram{};
    std::array<OAMEntry, 40>    oam_entries{};

    uint16_t dot_elapsed{};

    /**
     * @brief LCD Control (R/W).
     */
    uint8_t LCDC{};

    /**
     * @brief LCDC Status (R/W).
     */
    uint8_t STAT{};

    /**
     * @brief Background viewport Y position (R/W).
     */
    uint8_t SCY{};

    /**
     * @brief Background viewport X position X (R/W).
     */
    uint8_t SCX{};

    /**
     * @brief LY indicates the current horizontal line, which might be about to be drawn, being drawn, or just been
     * drawn. LY can hold any value from 0 to 153, with values from 144 to 153 indicating the VBlank period (R).
     */
    uint8_t LY{};
    uint8_t LYC{};
    uint8_t DMA{};
    uint8_t BGP{};
    uint8_t OBP0{};
    uint8_t OBP1{};
    uint8_t WY{};
    uint8_t WX{};

    Mode mode{Mode::OAMScan};
};

#endif  // PPU_HXX
