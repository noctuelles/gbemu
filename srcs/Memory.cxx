#include "Memory.hxx"

Memory::Memory()
    : memory()
{
}
Memory::~Memory()= default;

uint8_t Memory::read(uint16_t addr) const{
    return memory.at(addr);
}

void Memory::write(const uint8_t val, const uint16_t addr){
    memory.at(addr) = val;
}





