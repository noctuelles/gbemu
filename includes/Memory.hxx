//
// Created by plouvel on 12/5/24.
//

#ifndef MEMORY_HXX
#define MEMORY_HXX

#include <array>
#include <cstdint>

class Memory
{
public:
    Memory();
    ~Memory();

    Memory(const Memory &) = default;
    Memory& operator=(const Memory &) = default;

    uint8_t read(uint16_t addr) const;
    void write(uint8_t val, uint16_t addr);

private:
    std::array<uint8_t, 8192> memory;
};


#endif //MEMORY_HXX
