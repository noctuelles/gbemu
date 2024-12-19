//
// Created by plouvel on 12/7/24.
//

#ifndef BUS_HXX
#define BUS_HXX

#include <array>
#include <cstddef>
#include <cstdint>

class Bus
{
  public:
    static constexpr size_t RAM_SIZE = 1 << 16;

    Bus();
    ~Bus();

    [[nodiscard]] uint8_t read(uint16_t addr) const;
    void                  write(uint16_t addr, uint8_t data);
    void                  write(uint16_t addr, std::initializer_list<uint8_t> data);

  private:
    std::array<uint8_t, RAM_SIZE> ram{};
};

#endif  // BUS_HXX
