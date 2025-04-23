//
// Created by plouvel on 1/9/25.
//

#include <istream>
#include <fstream>

#include "hardware/Bus.hxx"
#include "hardware/Cartridge.hxx"
#include "hardware/Timer.hxx"
#include "hardware/core/SM83.hxx"

#include "gtest/gtest.h"

struct FakeRAM final : Addressable
{
    uint8_t read(const uint16_t address) override
    {
        return content[address];
    }
    void write(const uint16_t address, const uint8_t value) override
    {
        content[address] = value;
    }

    [[nodiscard]] AddressableRange get_addressable_range() const override
    {
        return {std::make_pair(0x0000, 0xFFFF)};
    }

    std::array<uint8_t, 0xFFFF> content{};
};

class MooneyeAcceptance : public testing::Test
{
  protected:
    std::unique_ptr<Bus>     bus{};
    std::unique_ptr<SM83>    cpu{};
    std::unique_ptr<FakeRAM> ram{};
    std::unique_ptr<Timer>   timer{};

    void SetUp() override
    {
        bus = std::make_unique<Bus>();

        timer = std::make_unique<Timer>(*bus);
        cpu   = std::make_unique<SM83>(*bus, [this] { timer->tick(); });
        ram   = std::make_unique<FakeRAM>();

        bus->attach(*cpu);
        bus->attach(*timer);
        bus->attach(*ram);
    }

    void TearDown() override
    {
        bus.reset();
        cpu.reset();
        ram.reset();
    }

    void execute_rom(const std::string& rom_name)
    {
        std::ifstream input{std::string{ROMS_PATH} + std::string{"/mooneye/acceptance/"} + rom_name, std::ios::binary};
        std::string   s{};

        input.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        input.read(reinterpret_cast<char*>(ram->content.data()), 0x8000);
        input.close();

        while (true)
        {
            cpu->tick();

            if (bus->read(0xFF02) == 0x81)
            {
                s += static_cast<char>(bus->read(0xFF01));
                bus->write(0xFF02, 0x00);
            }

            if (cpu->B == 3 && cpu->C == 5 && cpu->D == 8 && cpu->E == 13 && cpu->H == 21 && cpu->L == 34)
            {
                break;
            }
            if (cpu->B == 0x42 && cpu->C == 0x42 && cpu->D == 0x42 && cpu->E == 0x42 && cpu->H == 0x42 &&
                cpu->L == 0x42)
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

TEST_F(MooneyeAcceptance, EiTiming)
{
    ASSERT_NO_THROW(execute_rom("ei_timing.gb"));
}

TEST_F(MooneyeAcceptance, DiTiming)
{
    GTEST_SKIP();
    ASSERT_NO_THROW(execute_rom("di_timing.gb"));
}

TEST_F(MooneyeAcceptance, IntrerrupTiming)
{
    ASSERT_NO_THROW(execute_rom("intr_timing.gb"));
}

TEST_F(MooneyeAcceptance, DivTiming)
{
    ASSERT_NO_THROW(execute_rom("div_timing.gb"));
}

TEST_F(MooneyeAcceptance, IF_IE_Registers)
{
    ASSERT_NO_THROW(execute_rom("if_ie_registers.gb"));
}

TEST_F(MooneyeAcceptance, RapidDiEi)
{
    ASSERT_NO_THROW(execute_rom("rapid_di_ei.gb"));
}

TEST_F(MooneyeAcceptance, EiSequence)
{
    ASSERT_NO_THROW(execute_rom("ei_sequence.gb"));
}

TEST_F(MooneyeAcceptance, HaltIme0EI)
{
    /**
     * We know that the effect of EI has a delay. This tests how EI before HALT behaves.
     * If EI is before HALT, the HALT instruction is expected to perform its normal
     * IME=1 behaviour.
     */
    GTEST_SKIP();
    ASSERT_NO_THROW(execute_rom("halt_ime0_ei.gb"));
}

TEST_F(MooneyeAcceptance, HaltIme1Timing)
{
    ASSERT_NO_THROW(execute_rom("halt_ime1_timing.gb"));
}

TEST_F(MooneyeAcceptance, HaltBug)
{
    GTEST_SKIP();
    ASSERT_NO_THROW(execute_rom("halt_bug.gb"));
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
