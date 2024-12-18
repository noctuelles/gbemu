//
// Created by plouvel on 12/7/24.
//

#ifndef BUS_HXX
#define BUS_HXX

#include <array>
#include <cstdint>
#include <span>

class Bus
{
  public:
    Bus();
    ~Bus();

    static constexpr size_t MEMORY_SIZE = 8192;

    [[nodiscard]] uint8_t  read(uint16_t addr) const;

    void write(uint16_t addr, uint8_t data);
    void write(uint16_t addr, std::initializer_list<uint8_t> data);

  private:
    std::array<uint8_t, MEMORY_SIZE> memory;
};

#endif  // BUS_HXX
