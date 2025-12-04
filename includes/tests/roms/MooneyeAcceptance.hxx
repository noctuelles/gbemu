//
// Created by plouvel on 12/4/25.
//

#ifndef GBEMU_MOONEYEACCEPTANCE_HXX
#define GBEMU_MOONEYEACCEPTANCE_HXX

#include "TestRom.hxx"

class MooneyeAcceptance : public TestRom
{
  protected:
    void executeROM(const std::string& romName) override;
};

#endif  // GBEMU_MOONEYEACCEPTANCE_HXX
