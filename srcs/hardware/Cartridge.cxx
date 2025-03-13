//
// Created by plouvel on 12/18/24.
//

#include "../../includes/hardware/Cartridge.hxx"

#include <cstdint>
#include <fstream>
#include <iostream>
#include <string_view>
#include <unordered_map>
#include <utility>

const std::unordered_map<std::byte, std::string_view> old_licensee_code{{
    {std::byte{0x00}, "None"},
    {std::byte{0x01}, "Nintendo"},
    {std::byte{0x08}, "Capcom"},
    {std::byte{0x09}, "HOT-B"},
    {std::byte{0x0A}, "Jaleco"},
    {std::byte{0x0B}, "Coconuts Japan"},
    {std::byte{0x0C}, "Elite Systems"},
    {std::byte{0x13}, "EA (Electronic Arts)"},
    {std::byte{0x18}, "Hudson Soft"},
    {std::byte{0x19}, "ITC Entertainment"},
    {std::byte{0x1A}, "Yanoman"},
    {std::byte{0x1D}, "Japan Clary"},
    {std::byte{0x1F}, "Virgin Games Ltd.3"},
    {std::byte{0x24}, "PCM Complete"},
    {std::byte{0x25}, "San-X"},
    {std::byte{0x28}, "Kemco"},
    {std::byte{0x29}, "SETA Corporation"},
    {std::byte{0x30}, "Infogrames5"},
    {std::byte{0x31}, "Nintendo"},
    {std::byte{0x32}, "Bandai"},
    {std::byte{0x34}, "Konami"},
    {std::byte{0x35}, "HectorSoft"},
    {std::byte{0x38}, "Capcom"},
    {std::byte{0x39}, "Banpresto"},
    {std::byte{0x3C}, "Entertainment Interactive (stub)"},
    {std::byte{0x3E}, "Gremlin"},
    {std::byte{0x41}, "Ubi Soft1"},
    {std::byte{0x42}, "Atlus"},
    {std::byte{0x44}, "Malibu Interactive"},
    {std::byte{0x46}, "Angel"},
    {std::byte{0x47}, "Spectrum HoloByte"},
    {std::byte{0x49}, "Irem"},
    {std::byte{0x4A}, "Virgin Games Ltd.3"},
    {std::byte{0x4D}, "Malibu Interactive"},
    {std::byte{0x4F}, "U.S. Gold"},
    {std::byte{0x50}, "Absolute"},
    {std::byte{0x51}, "Acclaim Entertainment"},
    {std::byte{0x52}, "Activision"},
    {std::byte{0x53}, "Sammy USA Corporation"},
    {std::byte{0x54}, "GameTek"},
    {std::byte{0x55}, "Park Place13"},
    {std::byte{0x56}, "LJN"},
    {std::byte{0x57}, "Matchbox"},
    {std::byte{0x59}, "Milton Bradley Company"},
    {std::byte{0x5A}, "Mindscape"},
    {std::byte{0x5B}, "Romstar"},
    {std::byte{0x5C}, "Naxat Soft14"},
    {std::byte{0x5D}, "Tradewest"},
    {std::byte{0x60}, "Titus Interactive"},
    {std::byte{0x61}, "Virgin Games Ltd.3"},
    {std::byte{0x67}, "Ocean Software"},
    {std::byte{0x69}, "EA (Electronic Arts)"},
    {std::byte{0x6E}, "Elite Systems"},
    {std::byte{0x6F}, "Electro Brain"},
    {std::byte{0x70}, "Infogrames5"},
    {std::byte{0x71}, "Interplay Entertainment"},
    {std::byte{0x72}, "Broderbund"},
    {std::byte{0x73}, "Sculptured Software6"},
    {std::byte{0x75}, "The Sales Curve Limited7"},
    {std::byte{0x78}, "THQ"},
    {std::byte{0x79}, "Accolade15"},
    {std::byte{0x7A}, "Triffix Entertainment"},
    {std::byte{0x7C}, "MicroProse"},
    {std::byte{0x7F}, "Kemco"},
    {std::byte{0x80}, "Misawa Entertainment"},
    {std::byte{0x83}, "LOZC G."},
    {std::byte{0x86}, "Tokuma Shoten"},
    {std::byte{0x8B}, "Bullet-Proof Software2"},
    {std::byte{0x8C}, "Vic Tokai Corp.16"},
    {std::byte{0x8E}, "Ape Inc.17"},
    {std::byte{0x8F}, "I’Max18"},
    {std::byte{0x91}, "Chunsoft Co.8"},
    {std::byte{0x92}, "Video System"},
    {std::byte{0x93}, "Tsubaraya Productions"},
    {std::byte{0x95}, "Varie"},
    {std::byte{0x96}, "Yonezawa19/S’Pal"},
    {std::byte{0x97}, "Kemco"},
    {std::byte{0x99}, "Arc"},
    {std::byte{0x9A}, "Nihon Bussan"},
    {std::byte{0x9B}, "Tecmo"},
    {std::byte{0x9C}, "Imagineer"},
    {std::byte{0x9D}, "Banpresto"},
    {std::byte{0x9F}, "Nova"},
    {std::byte{0xA1}, "Hori Electric"},
    {std::byte{0xA2}, "Bandai"},
    {std::byte{0xA4}, "Konami"},
    {std::byte{0xA6}, "Kawada"},
    {std::byte{0xA7}, "Takara"},
    {std::byte{0xA9}, "Technos Japan"},
    {std::byte{0xAA}, "Broderbund"},
    {std::byte{0xAC}, "Toei Animation"},
    {std::byte{0xAD}, "Toho"},
    {std::byte{0xAF}, "Namco"},
    {std::byte{0xB0}, "Acclaim Entertainment"},
    {std::byte{0xB1}, "ASCII Corporation or Nexsoft"},
    {std::byte{0xB2}, "Bandai"},
    {std::byte{0xB4}, "Square Enix"},
    {std::byte{0xB6}, "HAL Laboratory"},
    {std::byte{0xB7}, "SNK"},
    {std::byte{0xB9}, "Pony Canyon"},
    {std::byte{0xBA}, "Culture Brain"},
    {std::byte{0xBB}, "Sunsoft"},
    {std::byte{0xBD}, "Sony Imagesoft"},
    {std::byte{0xBF}, "Sammy Corporation"},
    {std::byte{0xC0}, "Taito"},
    {std::byte{0xC2}, "Kemco"},
    {std::byte{0xC3}, "Square"},
    {std::byte{0xC4}, "Tokuma Shoten"},
    {std::byte{0xC5}, "Data East"},
    {std::byte{0xC6}, "Tonkin House"},
    {std::byte{0xC8}, "Koei"},
    {std::byte{0xC9}, "UFL"},
    {std::byte{0xCA}, "Ultra Games"},
    {std::byte{0xCB}, "VAP, Inc."},
    {std::byte{0xCC}, "Use Corporation"},
    {std::byte{0xCD}, "Meldac"},
    {std::byte{0xCE}, "Pony Canyon"},
    {std::byte{0xCF}, "Angel"},
    {std::byte{0xD0}, "Taito"},
    {std::byte{0xD1}, "SOFEL (Software Engineering Lab)"},
    {std::byte{0xD2}, "Quest"},
    {std::byte{0xD3}, "Sigma Enterprises"},
    {std::byte{0xD4}, "ASK Kodansha Co."},
    {std::byte{0xD6}, "Naxat Soft14"},
    {std::byte{0xD7}, "Copya System"},
    {std::byte{0xD9}, "Banpresto"},
    {std::byte{0xDA}, "Tomy"},
    {std::byte{0xDB}, "LJN"},
    {std::byte{0xDD}, "Nippon Computer Systems"},
    {std::byte{0xDE}, "Human Ent."},
    {std::byte{0xDF}, "Altron"},
    {std::byte{0xE0}, "Jaleco"},
    {std::byte{0xE1}, "Towa Chiki"},
    {std::byte{0xE2}, "Yutaka"},
    {std::byte{0xE3}, "Varie"},
    {std::byte{0xE5}, "Epoch"},
    {std::byte{0xE7}, "Athena"},
    {std::byte{0xE8}, "Asmik Ace Entertainment"},
    {std::byte{0xE9}, "Natsume"},
    {std::byte{0xEA}, "King Records"},
    {std::byte{0xEB}, "Atlus"},
    {std::byte{0xEC}, "Epic/Sony Records"},
    {std::byte{0xEE}, "IGS"},
    {std::byte{0xF0}, "A Wave"},
    {std::byte{0xF3}, "Extreme Entertainment"},
    {std::byte{0xFF}, "LJN"},
}};

const std::unordered_map<std::string_view, std::string_view> new_licensee_code{{
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
    std::ifstream input{path, std::ios::binary};

    content.resize(file_size(path));

    input.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    input.read(reinterpret_cast<char*>(content.data()), content.size());
    input.close();

    // const auto old_licensee_byte = content[0x014B];
    const auto title_byte_addr   = &content[0x0134];
    // const auto new_licensee_addr = &content[0x0144];
    const auto size_byte         = content[0x0148];

    title = std::string_view{reinterpret_cast<const char*>(title_byte_addr), 16};

    // if (old_licensee_byte == std::byte{0x33})
    // {
    //     licensee = new_licensee_code.at(std::string_view{reinterpret_cast<const char*>(new_licensee_addr), 2});
    // }
    // licensee = old_licensee_code.at(old_licensee_byte);

    switch (const auto type_byte = static_cast<std::underlying_type_t<Type>>(content[0x0147]);
            static_cast<Type>(type_byte))
    {
        case Type::MBC1:
        case Type::ROM_ONLY:
            type = static_cast<Type>(type_byte);
            break;
        default:
            break;
            // throw std::runtime_error(std::format("Invalid or unsupported cartridge type {:02X}", type_byte));
    }
    size = SIZE * (1 << std::to_integer<decltype(size)>(size_byte));
}

uint8_t Cartridge::read(const uint16_t address)
{
    return std::to_underlying(content[address]);
}

void Cartridge::write(const uint16_t address, uint8_t value)
{
    content[address] = std::byte{value};
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

std::size_t Cartridge::get_size() const
{
    return size;
}

Cartridge::operator std::span<uint8_t>()
{
    return {reinterpret_cast<uint8_t*>(content.data()), content.size()};
}