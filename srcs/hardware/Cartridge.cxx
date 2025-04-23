//
// Created by plouvel on 12/18/24.

#include "../../includes/hardware/Cartridge.hxx"

#include <cstdint>
#include <fstream>
#include <iostream>
#include <string_view>
#include <unordered_map>
#include <utility>

static const std::unordered_map<std::uint8_t, std::string_view> old_licensee_code{{
    {0x00, "None"},
    {0x01, "Nintendo"},
    {0x08, "Capcom"},
    {0x09, "HOT-B"},
    {0x0A, "Jaleco"},
    {0x0B, "Coconuts Japan"},
    {0x0C, "Elite Systems"},
    {0x13, "EA (Electronic Arts)"},
    {0x18, "Hudson Soft"},
    {0x19, "ITC Entertainment"},
    {0x1A, "Yanoman"},
    {0x1D, "Japan Clary"},
    {0x1F, "Virgin Games Ltd.3"},
    {0x24, "PCM Complete"},
    {0x25, "San-X"},
    {0x28, "Kemco"},
    {0x29, "SETA Corporation"},
    {0x30, "Infogrames5"},
    {0x31, "Nintendo"},
    {0x32, "Bandai"},
    {0x34, "Konami"},
    {0x35, "HectorSoft"},
    {0x38, "Capcom"},
    {0x39, "Banpresto"},
    {0x3C, "Entertainment Interactive (stub)"},
    {0x3E, "Gremlin"},
    {0x41, "Ubi Soft1"},
    {0x42, "Atlus"},
    {0x44, "Malibu Interactive"},
    {0x46, "Angel"},
    {0x47, "Spectrum HoloByte"},
    {0x49, "Irem"},
    {0x4A, "Virgin Games Ltd.3"},
    {0x4D, "Malibu Interactive"},
    {0x4F, "U.S. Gold"},
    {0x50, "Absolute"},
    {0x51, "Acclaim Entertainment"},
    {0x52, "Activision"},
    {0x53, "Sammy USA Corporation"},
    {0x54, "GameTek"},
    {0x55, "Park Place13"},
    {0x56, "LJN"},
    {0x57, "Matchbox"},
    {0x59, "Milton Bradley Company"},
    {0x5A, "Mindscape"},
    {0x5B, "Romstar"},
    {0x5C, "Naxat Soft14"},
    {0x5D, "Tradewest"},
    {0x60, "Titus Interactive"},
    {0x61, "Virgin Games Ltd.3"},
    {0x67, "Ocean Software"},
    {0x69, "EA (Electronic Arts)"},
    {0x6E, "Elite Systems"},
    {0x6F, "Electro Brain"},
    {0x70, "Infogrames5"},
    {0x71, "Interplay Entertainment"},
    {0x72, "Broderbund"},
    {0x73, "Sculptured Software6"},
    {0x75, "The Sales Curve Limited7"},
    {0x78, "THQ"},
    {0x79, "Accolade15"},
    {0x7A, "Triffix Entertainment"},
    {0x7C, "MicroProse"},
    {0x7F, "Kemco"},
    {0x80, "Misawa Entertainment"},
    {0x83, "LOZC G."},
    {0x86, "Tokuma Shoten"},
    {0x8B, "Bullet-Proof Software2"},
    {0x8C, "Vic Tokai Corp.16"},
    {0x8E, "Ape Inc.17"},
    {0x8F, "I’Max18"},
    {0x91, "Chunsoft Co.8"},
    {0x92, "Video System"},
    {0x93, "Tsubaraya Productions"},
    {0x95, "Varie"},
    {0x96, "Yonezawa19/S’Pal"},
    {0x97, "Kemco"},
    {0x99, "Arc"},
    {0x9A, "Nihon Bussan"},
    {0x9B, "Tecmo"},
    {0x9C, "Imagineer"},
    {0x9D, "Banpresto"},
    {0x9F, "Nova"},
    {0xA1, "Hori Electric"},
    {0xA2, "Bandai"},
    {0xA4, "Konami"},
    {0xA6, "Kawada"},
    {0xA7, "Takara"},
    {0xA9, "Technos Japan"},
    {0xAA, "Broderbund"},
    {0xAC, "Toei Animation"},
    {0xAD, "Toho"},
    {0xAF, "Namco"},
    {0xB0, "Acclaim Entertainment"},
    {0xB1, "ASCII Corporation or Nexsoft"},
    {0xB2, "Bandai"},
    {0xB4, "Square Enix"},
    {0xB6, "HAL Laboratory"},
    {0xB7, "SNK"},
    {0xB9, "Pony Canyon"},
    {0xBA, "Culture Brain"},
    {0xBB, "Sunsoft"},
    {0xBD, "Sony Imagesoft"},
    {0xBF, "Sammy Corporation"},
    {0xC0, "Taito"},
    {0xC2, "Kemco"},
    {0xC3, "Square"},
    {0xC4, "Tokuma Shoten"},
    {0xC5, "Data East"},
    {0xC6, "Tonkin House"},
    {0xC8, "Koei"},
    {0xC9, "UFL"},
    {0xCA, "Ultra Games"},
    {0xCB, "VAP, Inc."},
    {0xCC, "Use Corporation"},
    {0xCD, "Meldac"},
    {0xCE, "Pony Canyon"},
    {0xCF, "Angel"},
    {0xD0, "Taito"},
    {0xD1, "SOFEL (Software Engineering Lab)"},
    {0xD2, "Quest"},
    {0xD3, "Sigma Enterprises"},
    {0xD4, "ASK Kodansha Co."},
    {0xD6, "Naxat Soft14"},
    {0xD7, "Copya System"},
    {0xD9, "Banpresto"},
    {0xDA, "Tomy"},
    {0xDB, "LJN"},
    {0xDD, "Nippon Computer Systems"},
    {0xDE, "Human Ent."},
    {0xDF, "Altron"},
    {0xE0, "Jaleco"},
    {0xE1, "Towa Chiki"},
    {0xE2, "Yutaka"},
    {0xE3, "Varie"},
    {0xE5, "Epoch"},
    {0xE7, "Athena"},
    {0xE8, "Asmik Ace Entertainment"},
    {0xE9, "Natsume"},
    {0xEA, "King Records"},
    {0xEB, "Atlus"},
    {0xEC, "Epic/Sony Records"},
    {0xEE, "IGS"},
    {0xF0, "A Wave"},
    {0xF3, "Extreme Entertainment"},
    {0xFF, "LJN"},
}};

static const std::unordered_map<std::string_view, std::string_view> new_licensee_code{{
    {"00", "None"},
    {"01", "Nintendo Research & Development 1"},
    {"08", "Capcom"},
    {"13", "EA (Electronic Arts)"},
    {"18", "Hudson Soft"},
    {"19", "B-AI"},
    {"20", "KSS"},
    {"22", "Planning Office WADA"},
    {"24", "PCM Complete"},
    {"25", "San-X"},
    {"28", "Kemco"},
    {"29", "SETA Corporation"},
    {"30", "Viacom"},
    {"31", "Nintendo"},
    {"32", "Bandai"},
    {"33", "Ocean Software/Acclaim Entertainment"},
    {"34", "Konami"},
    {"35", "HectorSoft"},
    {"37", "Taito"},
    {"38", "Hudson Soft"},
    {"39", "Banpresto"},
    {"41", "Ubi Soft1"},
    {"42", "Atlus"},
    {"44", "Malibu Interactive"},
    {"46", "Angel"},
    {"47", "Bullet-Proof Software2"},
    {"49", "Irem"},
    {"50", "Absolute"},
    {"51", "Acclaim Entertainment"},
    {"52", "Activision"},
    {"53", "Sammy USA Corporation"},
    {"54", "Konami"},
    {"55", "Hi Tech Expressions"},
    {"56", "LJN"},
    {"57", "Matchbox"},
    {"58", "Mattel"},
    {"59", "Milton Bradley Company"},
    {"60", "Titus Interactive"},
    {"61", "Virgin Games Ltd.3"},
    {"64", "Lucasfilm Games4"},
    {"67", "Ocean Software"},
    {"69", "EA (Electronic Arts)"},
    {"70", "Infogrames5"},
    {"71", "Interplay Entertainment"},
    {"72", "Broderbund"},
    {"73", "Sculptured Software6"},
    {"75", "The Sales Curve Limited7"},
    {"78", "THQ"},
    {"79", "Accolade"},
    {"80", "Misawa Entertainment"},
    {"83", "lozc"},
    {"86", "Tokuma Shoten"},
    {"87", "Tsukuda Original"},
    {"91", "Chunsoft Co.8"},
    {"92", "Video System"},
    {"93", "Ocean Software/Acclaim Entertainment"},
    {"95", "Varie"},
    {"96", "Yonezawa/s’pal"},
    {"97", "Kaneko"},
    {"99", "Pack-In-Video"},
    {"9H", "Bottom Up"},
    {"A4", "Konami (Yu-Gi-Oh!)"},
    {"BL", "MTO"},
    {"DK", "Kodansha"},
}};

Cartridge::Cartridge(const std::filesystem::path& path)
{
    const auto    cartridge_size{std::filesystem::file_size(path)};
    std::ifstream input{path, std::ios::binary};

    if (cartridge_size < 2 << 14)
    {
        throw std::runtime_error(std::format("Cartridge size ({:#010x}) is too small.", cartridge_size));
    }

    input.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    content.resize(cartridge_size);
    input.read(reinterpret_cast<char*>(content.data()), static_cast<std::streamsize>(cartridge_size));

    const auto title        = reinterpret_cast<const char*>(&content[0x0134]);
    const auto new_licensee = reinterpret_cast<const char*>(&content[0x0144]);
    const auto old_licensee = content[0x014B];
    const auto type         = content[0x0147];
    const auto rom_size     = content[0x0149];

    this->title    = std::string_view{title, 16};
    this->rom_size = (2 << 14) * (1 << rom_size);
    if (old_licensee == 0x33)
    {
        this->licensee = new_licensee_code.at(std::string_view{new_licensee, 2});
    }
    else
    {
        this->licensee = old_licensee_code.at(old_licensee);
    }

    switch (type)
    {
        case std::to_underlying<Type>(Type::ROM_ONLY):
        case std::to_underlying<Type>(Type::MBC1):
            this->type = static_cast<Type>(type);
            break;
        default:
            throw std::runtime_error(
                std::format("Cartridge type ({:#04x}) not supported.", type));
    }
}

uint8_t Cartridge::read(const uint16_t address)
{
    return content[address];
}

void Cartridge::write(const uint16_t address, uint8_t value)
{
    content[address] = value;
}

Addressable::AddressableRange Cartridge::get_addressable_range() const noexcept
{
    return {std::make_pair(0x0000, 0x7FFF)};
}

const std::string_view& Cartridge::get_title() const
{
    return title;
}

const std::string_view& Cartridge::get_licensee() const
{
    return licensee;
}

Cartridge::Type Cartridge::get_type() const
{
    return type;
}

std::size_t Cartridge::get_rom_size() const
{
    return rom_size;
}

Cartridge::operator std::span<uint8_t>()
{
    return {content.data(), content.size()};
}