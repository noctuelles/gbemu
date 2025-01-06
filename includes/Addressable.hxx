//
// Created by plouvel on 1/6/25.
//

#ifndef ADDRESSABLE_HPP
#define ADDRESSABLE_HPP

#include <cstdint>

class Addressable {
    public:
      virtual uint8_t read(uint16_t address) = 0;
      virtual void write(uint16_t address, uint8_t value) = 0;
};

#endif //ADDRESSABLE_HPP
