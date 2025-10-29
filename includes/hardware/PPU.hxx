//
// Created by plouvel on 1/10/25.
//

#ifndef PPU_HXX
#define PPU_HXX

#include <array>
#include <utility>

#include "Displayable.hxx"
#include "hardware/Addressable.hxx"

class PPU final : public Component
{
  public:
    static constexpr std::size_t LCD_HEIGHT{144};
    static constexpr std::size_t LCD_WIDTH{144};
    static constexpr std::size_t VERTICAL_BLANK_SCANLINE{10};

    explicit PPU(Addressable& bus, Displayable& display);

    enum class Mode
    {
        /**
         * @brief The PPU has finished drawing a single scanline. VRAM, OAM, CGB palettes are accessible.
         */
        HorizontalBlank = 0,

        /**
         * @brief The PPU has finished drawing the entire frame. VRAM, OAM, CGB palettes are accessible.
         */
        VerticalBlank = 1,
        OAMScan       = 2,
        Drawing       = 3,
    };

    enum class LCDControlFlags : uint8_t
    {
        /**
         * @brief LCDC.0 has different meanings depending on Game Boy type and Mode:
         *
         * Non-CGB Mode (DMG, SGB and CGB in compatibility mode): BG and Window display
         * When Bit 0 is cleared, both background and window become blank (white), and the Window Display Bit is ignored
         in that
         * case. Only objects may still be displayed (if enabled in Bit 1).
         * CGB Mode: BG and Window master priority
         * When Bit 0 is cleared, the background and window lose their priority - the objects will always be displayed
         * on top of
         * background and window, independently of the priority flags in OAM and BG Map attributes.
         *
         * @ref https://gbdev.io/pandocs/LCDC.html#lcdc0--bg-and-window-enablepriority
         */
        BgWindowEnableOrPriority = 0x00,

        /**
         * @brief This bit toggles whether objects are displayed or not.
         *
         * @ref https://gbdev.io/pandocs/LCDC.html#lcdc1--obj-enable
         */
        ObjEnable = 0x01,

        /**
         * @brief This bit controls the size of all objects (1 tile or 2 stacked vertically).
         *
         * @see https://gbdev.io/pandocs/LCDC.html#lcdc2--obj-size
         */
        ObjSize = 0x02,
    };

    struct OAMEntry
    {
        /**
         * @brief Object’s vertical position on the screen + 16.
         * - Y=0 hides an object.
         * - Y=2 hides an 8×8 object but displays the last two rows of an 8×16 object.
         * - Y=16 displays an object at the top of the screen.
         * - Y=144 displays an 8×16 object aligned with the bottom of the screen.
         * - Y=152 displays an 8×8 object aligned with the bottom of the screen.
         * - Y=154 displays the first six rows of an object at the bottom of the screen.
         * - Y=160 hides an object.
         */
        uint8_t y{};

        /**
         * @brief Object’s horizontal position on the screen + 8. The width of an object is always 8. An off-screen
         * value (x=0 or x>=168) hides the object, but the object still contributes to the limit of ten objects per
         * scanline. This can cause objects later in OAM not to be drawn on that line. A better way to hide an object is
         * to set its Y-coordinate off-screen.
         */
        uint8_t x{};

        /**
         * @brief In 8×8 mode (LCDC bit 2 = 0), this byte specifies the object’s only tile index ($00-$FF). This
         * unsigned value selects a tile from the memory area at $8000-$8FFF. In CGB Mode this could be either in VRAM
         * bank 0 or 1, depending on bit 3 of the following byte. In 8×16 mode (LCDC bit 2 = 1), the memory area at
         * $8000-$8FFF is still interpreted as a series of 8×8 tiles, where every 2 tiles form an object. In this mode,
         * this byte specifies the index of the first (top) tile of the object. This is enforced by the hardware: the
         * least significant bit of the tile index is ignored; that is, the top 8×8 tile is “NN & $FE”, and the bottom
         * 8×8 tile is “NN | $01”.
         */
        uint8_t tile_index{};

        /*
         * @brief [CGB Mode Only]: Which of OBP0–7 to use.
         */
        uint8_t cbg_palette : 3 {};

        /**
         * @brief [CGB Mode Only]:
         * - 0 = Fetch tile from VRAM bank 0
         * - 1 = Fetch tile from VRAM bank 1
         */
        uint8_t bank : 1 {};

        /**
         * @brief [Non CGB Mode only]:
         * - 0 = OBP0
         * - 1 = OBP1.
         */
        uint8_t dmg_palette : 1 {};

        /**
         * @brief X Flip flag.
         * - 0 = Normal
         * - 1 = Entire OBJ is horizontally mirrored
         */
        uint8_t x_flip : 1 {};

        /**
         * @brief Y Flip flag.
         * - 0 = Normal
         * - 1 = Entire OBJ is vertically mirrored
         */
        uint8_t y_flip : 1 {};

        /**
         * @brief Priotity flag.
         * - 0 = No
         * - 1 = BG and Window color indices 1–3 are drawn over this OBJ
         */
        uint8_t priority : 1 {};
    } __attribute__((packed));

    uint8_t read(uint16_t address) const override;
    void    write(uint16_t address, uint8_t value) override;
    void    tick() override;

  private:
    using OAMArray = std::array<OAMEntry, 40>;

    void transition(Mode transition_to);

    Addressable& bus;
    Displayable& display;

    std::array<uint8_t, 0x2000> video_ram{};
    OAMArray                    oam_entries{};
    OAMArray::const_iterator    curr_oam_entry{};

    std::vector<decltype(curr_oam_entry)> obj_to_draw{};

    uint16_t dots{};

    uint8_t x{};

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

    /**
     * @brief The Game Boy constantly compares the value of the LYC and LY registers. When both values are identical,
     the “LYC=LY” flag in the STAT register is set, and (if enabled) a STAT interrupt is requested.
     */
    uint8_t LYC{};
    uint8_t DMA{};
    uint8_t BGP{};
    uint8_t OBP0{};
    uint8_t OBP1{};
    uint8_t WY{};
    uint8_t WX{};

    Mode mode{};
};

#endif  // PPU_HXX
