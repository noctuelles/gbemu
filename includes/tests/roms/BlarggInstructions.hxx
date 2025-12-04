//
// Created by plouvel on 12/4/25.
//

#ifndef GBEMU_BLARGGINSTRUCTIONS_HXX
#define GBEMU_BLARGGINSTRUCTIONS_HXX
#include "TestRom.hxx"

class BlarggInstructions : public TestRom
{
  protected:
    void executeROM(const std::string& rom_name) override;
};

#endif  // GBEMU_BLARGGINSTRUCTIONS_HXX
