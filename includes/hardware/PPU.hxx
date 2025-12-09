//
// Created by plouvel on 1/10/25.
//

#ifndef PPU_HXX
#define PPU_HXX

#include <array>
#include <queue>

#include "IRenderer.hxx"
#include "graphics/Framebuffer.hxx"
#include "hardware/IAddressable.hxx"

class PPU final : public IComponent
{
  public:
    explicit PPU(IAddressable& bus, IRenderer& renderer);

    struct Status
    {
        static constexpr uint8_t PPUMode{0x3};
        static constexpr uint8_t LYCCompare{1 << 2};
        static constexpr uint8_t HBlank{1 << 3};
        static constexpr uint8_t VBlank{1 << 4};
        static constexpr uint8_t OAMSearch{1 << 5};
        static constexpr uint8_t LYC{1 << 6};
    };

    struct LCDControlFlags
    {
        static constexpr uint8_t BGWindowEnableOrPriority{1 << 0};
        static constexpr uint8_t ObjEnable{1 << 1};
        static constexpr uint8_t ObjSize{1 << 2};
        static constexpr uint8_t BGTileMapSelect{1 << 3};
        static constexpr uint8_t BGAndWindowTileDataArea{1 << 4};
        static constexpr uint8_t WindowEnable{1 << 5};
        static constexpr uint8_t WindowTileMapSelect{1 << 6};
        static constexpr uint8_t LCDAndPPUEnable{1 << 7};
    };

    [[nodiscard]] uint8_t read(uint16_t address) const override;
    void                  write(uint16_t address, uint8_t value) override;
    void                  tick(size_t machineCycle) override;
    void                  setPostBootRomRegisters();

    [[nodiscard]] AddressableRange getAddressableRange() const noexcept override;

  private:
    enum class Mode : uint8_t
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
        Disabled      = 4,
    };

    struct Registers
    {
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
         * @brief The Game Boy constantly compares the value of the LYC and LY registers. When both values are
         identical, the “LYC=LY” flag in the STAT register is set, and (if enabled) a STAT interrupt is requested.
         */
        uint8_t LYC{};
        uint8_t DMA{};
        uint8_t BGP{};
        uint8_t OBP0{};
        uint8_t OBP1{};
        uint8_t WY{};
        uint8_t WX{};
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
        uint8_t tileIndex{};

        /*
         * @brief [CGB Mode Only]: Which of OBP0–7 to use.
         */
        uint8_t cbgPalette : 3 {};

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
        uint8_t dmgPalette : 1 {};

        /**
         * @brief X Flip flag.
         * - 0 = Normal
         * - 1 = Entire OBJ is horizontally mirrored
         */
        uint8_t xFlip : 1 {};

        /**
         * @brief Y Flip flag.
         * - 0 = Normal
         * - 1 = Entire OBJ is vertically mirrored
         */
        uint8_t yFlip : 1 {};

        /**
         * @brief Priotity flag.
         * - 0 = No
         * - 1 = BG and Window color indices 1–3 are drawn over this OBJ
         */
        uint8_t priority : 1 {};
    };

    using OAMArray         = std::array<OAMEntry, 40>;
    using OAMArrayItVector = std::vector<OAMArray::const_iterator>;
    using VideoRAM         = std::array<uint8_t, 0x2000>;

    static_assert(sizeof(OAMEntry) == 4, "There should be no padding!");

    void    _drawLine();
    uint8_t _colorMixing(OAMArray::const_iterator objFetched, uint8_t objPixel, uint8_t bgPixel) const;
    void    _transition(Mode transitionTo);
    void    _triggerStatInterrupt(bool value);

    IAddressable&    _bus;
    IRenderer&       _renderer;
    bool             _irq{};
    VideoRAM         _videoRam{};
    bool             _videoRamAccessible{true};
    OAMArray         _oamEntries{};
    OAMArrayItVector _oamEntriesToDraw{};
    bool             _oamAccessible{true};
    Registers        _registers{};
    uint16_t         _dots{};
    uint8_t          _pixelsToDiscard{};
    uint8_t          _windowLineCounter{};
    Mode             _mode{Mode::Disabled};
    bool             _isFirstTile{};

    friend class MooneyeAcceptance;
};

#endif  // PPU_HXX
