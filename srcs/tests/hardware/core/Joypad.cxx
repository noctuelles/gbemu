//
// Created by plouvel on 22/11/2025.
//

#include "hardware/Joypad.hxx"

#include "gtest/gtest.h"

class TestJoypad : public testing::Test
{
  protected:
    std::unique_ptr<Joypad> joypad{};

    void SetUp() override
    {
        joypad = std::make_unique<Joypad>();
    }

    void TearDown() override
    {
        joypad.reset();
    }
};

TEST_F(TestJoypad, SelectNothing) {}

TEST_F(TestJoypad, SelectButtons)
{
    joypad->press(Button::A);
    joypad->press(Button::B);

    joypad->write(MemoryMap::IORegisters::JOYPAD, 0b00010000);

    ASSERT_EQ(joypad->read(MemoryMap::IORegisters::JOYPAD), 0b11011100);
}

TEST_F(TestJoypad, SelectDirections) {}
