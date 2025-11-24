//
// Created by plouvel on 12/18/24.
//

#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <filesystem>
#include <string_view>
#include <vector>

#include "IAddressable.hxx"

class Cartridge final : public IAddressable
{
  public:
    enum class Type : std::uint8_t
    {
        ROM_ONLY                       = 0x00,
        MBC1                           = 0x01,
        MBC1_RAM                       = 0x02,
        MBC1_RAM_BATTERY               = 0x03,
        MBC2                           = 0x05,
        MBC2_BATTERY                   = 0x06,
        ROM_RAM                        = 0x08,
        ROM_RAM_BATTERY                = 0x09,
        MMM01                          = 0x0B,
        MMM01_RAM                      = 0x0C,
        MMM01_RAM_BATTERY              = 0x0D,
        MBC3_TIMER_BATTERY             = 0x0F,
        MBC3_TIMER_RAM_BATTERY         = 0x10,
        MBC3                           = 0x11,
        MBC3_RAM                       = 0x12,
        MBC3_RAM_BATTERY               = 0x13,
        MBC5                           = 0x19,
        MBC5_RAM                       = 0x1A,
        MBC5_RAM_BATTERY               = 0x1B,
        MBC5_RUMBLE                    = 0x1C,
        MBC5_RUMBLE_RAM                = 0x1D,
        MBC5_RUMBLE_RAM_BATTERY        = 0x1E,
        MBC6                           = 0x20,
        MBC7_SENSOR_RUMBLE_RAM_BATTERY = 0x22,
        POCKET_CAMERA                  = 0xFC,
        BANDAI_TAMA5                   = 0xFD,
        HuC3                           = 0xFE,
        HuC1_RAM_BATTERY               = 0xFF
    };

    explicit Cartridge(const std::filesystem::path& path);

    uint8_t                        read(uint16_t address) const override;
    void                           write(uint16_t address, uint8_t value) override;
    [[nodiscard]] AddressableRange getAddressableRange() const noexcept override;

    [[nodiscard]] const std::string_view& get_title() const;
    [[nodiscard]] const std::string_view& get_licensee() const;
    [[nodiscard]] Type                    get_type() const;
    [[nodiscard]] std::size_t             get_rom_size() const;

    explicit operator std::span<uint8_t>();

  private:
    std::string_view title{};
    std::string_view licensee{};
    std::size_t      rom_size{};
    Type             type{};

    std::vector<std::uint8_t> content{};
};

#endif  // CARTRIDGE_H
