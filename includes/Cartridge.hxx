//
// Created by plouvel on 12/18/24.
//

#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <filesystem>
#include <vector>
#include <string_view>

#include "Addressable.hxx"

class Cartridge final : public Addressable
{
  public:
    enum class Type
    {
        ROM_ONLY,
        MBC1
    };

    static constexpr std::size_t SIZE = 32768;

    explicit Cartridge(const std::filesystem::path& path);

    uint8_t read(uint16_t address) override;
    void    write(uint16_t address, uint8_t value) override;

    [[nodiscard]] const std::string_view& get_title() const;
    [[nodiscard]] const std::string_view& get_licensee() const;
    [[nodiscard]] Type                    get_type() const;
    [[nodiscard]] std::size_t             get_size() const;

    explicit operator std::span<uint8_t>();

  private:
    std::string_view title{};
    std::string_view licensee{};
    std::size_t      size{};
    Type             type{};

    std::vector<std::byte> content{};
};

#endif  // CARTRIDGE_H
