//
// Created by plouvel on 12/4/25.
//

#ifndef GBEMU_TESTROM_HXX
#define GBEMU_TESTROM_HXX

#include <hardware/core/SM83.hxx>

#include "EmulationState.hxx"
#include "gtest/gtest.h"
#include "hardware/Bus.hxx"
#include "hardware/EchoRAM.hxx"
#include "hardware/Timer.hxx"
#include "hardware/WorkRAM.hxx"

class TestRom : public ::testing::Test
{
  protected:
    struct Component
    {
      private:
        EmulationState _state;

      public:
        Component();

        Bus     bus;
        SM83    cpu;
        WorkRAM workRam;
        EchoRAM echoRam;
        FakeRAM fakeRam;
        Timer   timer;
        PPU     ppu;
    };

    std::unique_ptr<Component> _component{};

    void SetUp() override;
    void TearDown() override;

    void         loadROM(const std::string& romPath) const;
    virtual void executeROM(const std::string& romName) = 0;
};

#endif  // GBEMU_TESTROM_HXX
