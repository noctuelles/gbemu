//
// Created by plouvel on 12/7/24.
//

#ifndef BUS_HXX
#define BUS_HXX

#include <cstdint>
#include <array>

class Bus
{
  public:
    Bus();
    ~Bus();

    uint8_t read(uint16_t addr);
    void    write(uint16_t addr, uint8_t data);

  private:
    std::array<uint8_t, 8192> memory;
};


#endif //BUS_HXX
