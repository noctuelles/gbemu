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
    Bus();

    static constexpr size_t RAM_SIZE = 1 << 16;

    static constexpr uint16_t IE = 0xFFFF;
    static constexpr uint16_t IF = 0xFF0F;

    static constexpr uint16_t LY = 0xFF44;
    static constexpr uint16_t SC = 0xFF02;

    void    write(uint16_t address, uint8_t value) override;
    uint8_t read(uint16_t address) override;

    std::unique_ptr<Component> cpu;
    std::unique_ptr<Component> timer;

  private:
    std::array<std::byte, RAM_SIZE> ram{};
};

#endif  // BUS_HXX
