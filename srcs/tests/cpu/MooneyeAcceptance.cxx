//
// Created by plouvel on 1/9/25.
//

#include <Bus.hxx>
#include <SM83.hxx>

#include "Cartridge.hxx"
#include "gtest/gtest.h"

class MooneyeAcceptance : public testing::Test
{
  protected:
    std::unique_ptr<Bus> bus{};

    void SetUp() override
    {
        this->bus = std::make_unique<Bus>();
    }

    void TearDown() override
    {
        this->bus.reset();
    }

    void execute_rom(const std::string& rom_name) const
    {
        Cartridge   cart{std::string{ROMS_PATH} + std::string{"/mooneye/acceptance/"} + rom_name};
        std::string s{};

        for (std::size_t i = 0; i < cart.get_size(); i++)
        {
            bus->write(i, cart.read(i));
        }
        while (true)
        {
            bus->cpu->tick();

            const auto ptr  = bus->cpu.get();
            const auto sm83 = dynamic_cast<SM83*>(ptr);

            if (sm83->B == 3 && sm83->C == 5 && sm83->D == 8 && sm83->E == 13 && sm83->H == 21 && sm83->L == 34)
            {
                break;
            }
            if (sm83->B == 0x42 && sm83->C == 0x42 && sm83->D == 0x42 && sm83->E == 0x42 && sm83->H == 0x42 &&
                sm83->L == 0x42)
            {
                throw std::runtime_error("Failed ! " + s);
            }
        }
    }
};

TEST_F(MooneyeAcceptance, InstructionDAA)
{
    ASSERT_NO_THROW(execute_rom("instr/daa.gb"));
}

TEST_F(MooneyeAcceptance, TimingRet)
{
    /* TODO: Infinite Loop ? Looks like it is related to LCD registers that are not yet implemented. */

    GTEST_SKIP();
    ASSERT_NO_THROW(execute_rom("ret_timing.gb"));
}

TEST_F(MooneyeAcceptance, TimerDivWrite)
{
    ASSERT_NO_THROW(execute_rom("timer/div_write.gb"));
}

TEST_F(MooneyeAcceptance, TimerRapidToggle)
{
    ASSERT_NO_THROW(execute_rom("timer/rapid_toggle.gb"));
}

TEST_F(MooneyeAcceptance, TimerTim00)
{
    ASSERT_NO_THROW(execute_rom("timer/tim00.gb"));
}

TEST_F(MooneyeAcceptance, TimerTim00DivTrigger)
{
    ASSERT_NO_THROW(execute_rom("timer/tim00_div_trigger.gb"));
}

TEST_F(MooneyeAcceptance, TimerTim01)
{
    ASSERT_NO_THROW(execute_rom("timer/tim01.gb"));
}

TEST_F(MooneyeAcceptance, TimerTim01DivTrigger)
{
    ASSERT_NO_THROW(execute_rom("timer/tim01_div_trigger.gb"));
}

TEST_F(MooneyeAcceptance, TimerTim10)
{
    ASSERT_NO_THROW(execute_rom("timer/tim10.gb"));
}

TEST_F(MooneyeAcceptance, TimerTim10DivTrigger)
{
    ASSERT_NO_THROW(execute_rom("timer/tim10_div_trigger.gb"));
}

TEST_F(MooneyeAcceptance, TimerTim11)
{
    ASSERT_NO_THROW(execute_rom("timer/tim11.gb"));
}

TEST_F(MooneyeAcceptance, TimerTim11DivTrigger)
{
    ASSERT_NO_THROW(execute_rom("timer/tim11_div_trigger.gb"));
}

TEST_F(MooneyeAcceptance, TimaReload)
{
    ASSERT_NO_THROW(execute_rom("timer/tima_reload.gb"));
}

TEST_F(MooneyeAcceptance, TimaWriteReloading)
{
    ASSERT_NO_THROW(execute_rom("timer/tima_write_reloading.gb"));
}

TEST_F(MooneyeAcceptance, TmaWriteReloading)
{
    ASSERT_NO_THROW(execute_rom("timer/tma_write_reloading.gb"));
}