//
// Created by plouvel on 12/7/24.
//

#ifndef BUS_HXX
#define BUS_HXX

#include <array>
#include <cstddef>
#include <memory>

#include "Addressable.hxx"

class Bus final : public Addressable
{
  public:
    static constexpr size_t      RAM_SIZE      = 0x2000;
    static constexpr std::size_t HIGH_RAM_SIZE = 0x7f;

    explicit Bus(const std::shared_ptr<Addressable>& cartridge);

    void    write(uint16_t address, uint8_t value) override;
    uint8_t read(uint16_t address) override;

    std::size_t frame();

  private:
    void on_machine_cycle();

    std::unique_ptr<Component>   cpu;
    std::unique_ptr<Component>   ppu;
    std::unique_ptr<Component>   timer;
    std::shared_ptr<Addressable> cartridge;

    std::size_t m_cycles{};

    std::array<uint8_t, RAM_SIZE>      work_ram{};
    std::array<uint8_t, HIGH_RAM_SIZE> high_ram{};
};

#endif  // BUS_HXX
