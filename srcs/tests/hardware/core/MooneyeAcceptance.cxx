//
// Created by plouvel on 1/9/25.
//

#include <fstream>
#include <istream>
#include <ranges>

#include "gtest/gtest.h"
#include "hardware/Bus.hxx"
#include "hardware/Cartridge.hxx"
#include "hardware/EchoRAM.hxx"
#include "hardware/Timer.hxx"
#include "hardware/WorkRAM.hxx"
#include "hardware/core/SM83.hxx"

class MooneyeAcceptance : public testing::Test
{
  protected:
    Graphics::Framebuffer framebuffer{};
    EmulationState        emulationState{};

    std::unique_ptr<Bus>     bus{};
    std::unique_ptr<SM83>    cpu{};
    std::unique_ptr<EchoRAM> echoRam{};
    std::unique_ptr<FakeRAM> ram{};
    std::unique_ptr<Timer>   timer{};
    std::unique_ptr<PPU>     ppu{};

    void SetUp() override
    {
        bus = std::make_unique<Bus>(emulationState);
        ppu = std::make_unique<PPU>(*bus, framebuffer, [] {});

        timer = std::make_unique<Timer>(*bus);
        cpu   = std::make_unique<SM83>(emulationState, *bus, *timer, *ppu);
        ram   = std::make_unique<FakeRAM>();
        echoRam = std::make_unique<EchoRAM>(*ram);

        bus->attach(*cpu);
        bus->attach(*timer);
        bus->attach(*ppu);
        bus->attach(*echoRam);
        bus->attach(*ram);
    }

    void TearDown() override
    {
        ram.reset();
        cpu.reset();
        timer.reset();
        bus.reset();
    }

    void executeROM(const std::string& rom_name) const
    {
        std::ifstream input{std::string{ROMS_PATH} + std::string{"/mooneye/acceptance/"} + rom_name, std::ios::binary};
        std::string   s{};
        std::array<uint8_t, 0x8000> buffer{};

        input.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        input.read(reinterpret_cast<char*>(buffer.data()), buffer.size());
        input.close();

        for (const auto [i, byte] : std::views::enumerate(buffer))
        {
            ram->write(i, byte);
        }

        cpu->A  = 0x01;
        cpu->C  = 0x13;
        cpu->E  = 0xD8;
        cpu->H  = 0x01;
        cpu->L  = 0x4D;
        cpu->SP = 0xFFFE;
        cpu->PC = 0x0100;

        cpu->IF = 0xE1;

        ppu->registers.LCDC = 0x91;
        ppu->registers.STAT = 0x85;

        timer->TAC = 0xF8;

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

TEST_F(MooneyeAcceptance, OamDma_Basic)
{
    ASSERT_NO_THROW(executeROM("oam_dma/basic.gb"));
}

TEST_F(MooneyeAcceptance, OamDma_RegisterRead)
{
    ASSERT_NO_THROW(executeROM("oam_dma/reg_read.gb"));
}

TEST_F(MooneyeAcceptance, OamDma_Sources)
{
    /* Failing on 0xFE00 as source. */
    ASSERT_NO_THROW(executeROM("oam_dma/sources-GS.gb"));
}

TEST_F(MooneyeAcceptance, OamDmaTiming)
{
    ASSERT_NO_THROW(executeROM("oam_dma_timing.gb"));
}

TEST_F(MooneyeAcceptance, OamDmaStart)
{
    ASSERT_NO_THROW(executeROM("oam_dma_start.gb"));
}

TEST_F(MooneyeAcceptance, OamDmaRestart)
{
    ASSERT_NO_THROW(executeROM("oam_dma_restart.gb"));
}

TEST_F(MooneyeAcceptance, InstructionDAA)
{
    ASSERT_NO_THROW(executeROM("instr/daa.gb"));
}



TEST_F(MooneyeAcceptance, EiTiming)
{
    ASSERT_NO_THROW(executeROM("ei_timing.gb"));
}

TEST_F(MooneyeAcceptance, DiTiming)
{
    ASSERT_NO_THROW(executeROM("di_timing-GS.gb"));
}

TEST_F(MooneyeAcceptance, InterruptTiming)
{
    ASSERT_NO_THROW(executeROM("intr_timing.gb"));
}

TEST_F(MooneyeAcceptance, PopTiming)
{
    ASSERT_NO_THROW(executeROM("pop_timing.gb"));
}

TEST_F(MooneyeAcceptance, PushTiming)
{
    ASSERT_NO_THROW(executeROM("push_timing.gb"));
}

TEST_F(MooneyeAcceptance, RstTiming)
{
    ASSERT_NO_THROW(executeROM("rst_timing.gb"));
}

TEST_F(MooneyeAcceptance, RetTiming)
{
    ASSERT_NO_THROW(executeROM("ret_timing.gb"));
}

TEST_F(MooneyeAcceptance, RetCcTiming)
{
    ASSERT_NO_THROW(executeROM("ret_cc_timing.gb"));
}

TEST_F(MooneyeAcceptance, DivTiming)
{
    ASSERT_NO_THROW(executeROM("div_timing.gb"));
}

TEST_F(MooneyeAcceptance, IF_IE_Registers)
{
    ASSERT_NO_THROW(executeROM("if_ie_registers.gb"));
}

TEST_F(MooneyeAcceptance, RapidDiEi)
{
    ASSERT_NO_THROW(executeROM("rapid_di_ei.gb"));
}

TEST_F(MooneyeAcceptance, EiSequence)
{
    ASSERT_NO_THROW(executeROM("ei_sequence.gb"));
}

TEST_F(MooneyeAcceptance, HaltIme0EI)
{
    /**
     * We know that the effect of EI has a delay. This tests how EI before HALT behaves.
     * If EI is before HALT, the HALT instruction is expected to perform its normal
     * IME=1 behaviour.
     */
    GTEST_SKIP();
    ASSERT_NO_THROW(executeROM("halt_ime0_ei.gb"));
}

TEST_F(MooneyeAcceptance, HaltIme1Timing)
{
    ASSERT_NO_THROW(executeROM("halt_ime1_timing.gb"));
}

TEST_F(MooneyeAcceptance, HaltBug)
{
    /* Infinite loop. */
    GTEST_SKIP();
    ASSERT_NO_THROW(executeROM("halt_bug.gb"));
}

TEST_F(MooneyeAcceptance, TimerDivWrite)
{
    ASSERT_NO_THROW(executeROM("timer/div_write.gb"));
}

TEST_F(MooneyeAcceptance, TimerRapidToggle)
{
    ASSERT_NO_THROW(executeROM("timer/rapid_toggle.gb"));
}

TEST_F(MooneyeAcceptance, TimerTim00)
{
    ASSERT_NO_THROW(executeROM("timer/tim00.gb"));
}

TEST_F(MooneyeAcceptance, TimerTim00DivTrigger)
{
    ASSERT_NO_THROW(executeROM("timer/tim00_div_trigger.gb"));
}

TEST_F(MooneyeAcceptance, TimerTim01)
{
    ASSERT_NO_THROW(executeROM("timer/tim01.gb"));
}

TEST_F(MooneyeAcceptance, TimerTim01DivTrigger)
{
    ASSERT_NO_THROW(executeROM("timer/tim01_div_trigger.gb"));
}

TEST_F(MooneyeAcceptance, TimerTim10)
{
    ASSERT_NO_THROW(executeROM("timer/tim10.gb"));
}

TEST_F(MooneyeAcceptance, TimerTim10DivTrigger)
{
    ASSERT_NO_THROW(executeROM("timer/tim10_div_trigger.gb"));
}

TEST_F(MooneyeAcceptance, TimerTim11)
{
    ASSERT_NO_THROW(executeROM("timer/tim11.gb"));
}

TEST_F(MooneyeAcceptance, TimerTim11DivTrigger)
{
    ASSERT_NO_THROW(executeROM("timer/tim11_div_trigger.gb"));
}

TEST_F(MooneyeAcceptance, TimaReload)
{
    ASSERT_NO_THROW(executeROM("timer/tima_reload.gb"));
}

TEST_F(MooneyeAcceptance, TimaWriteReloading)
{
    ASSERT_NO_THROW(executeROM("timer/tima_write_reloading.gb"));
}

TEST_F(MooneyeAcceptance, TmaWriteReloading)
{
    ASSERT_NO_THROW(executeROM("timer/tma_write_reloading.gb"));
}
