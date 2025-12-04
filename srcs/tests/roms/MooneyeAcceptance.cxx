//
// Created by plouvel on 12/4/25.
//

#include "tests/roms/MooneyeAcceptance.hxx"

void MooneyeAcceptance::executeROM(const std::string& romName)
{
    std::string s{};

    loadROM(std::string{ROMS_PATH} + std::string{"/mooneye/acceptance/"} + romName);

    _component->cpu.setPostBootRomRegisters();
    _component->bus.setPostBootRomRegisters();
    _component->ppu.setPostBootRomRegisters();

    while (true)
    {
        _component->cpu.runInstruction();

        if (_component->bus.read(0xFF02) == 0x81)
        {
            s += static_cast<char>(_component->bus.read(0xFF01));
            _component->bus.write(0xFF02, 0x00);
        }

        if (_component->cpu.B == 3 && _component->cpu.C == 5 && _component->cpu.D == 8 && _component->cpu.E == 13 &&
            _component->cpu.H == 21 && _component->cpu.L == 34)
        {
            break;
        }
        if (_component->cpu.B == 0x42 && _component->cpu.C == 0x42 && _component->cpu.D == 0x42 &&
            _component->cpu.E == 0x42 && _component->cpu.H == 0x42 && _component->cpu.L == 0x42)
        {
            throw std::runtime_error("Failed ! " + s);
        }
    }
}

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
    /* Infinite loop ? */

    GTEST_SKIP();
    ASSERT_NO_THROW(executeROM("ret_timing.gb"));
}

TEST_F(MooneyeAcceptance, RetCcTiming)
{
    /* Infinite loop ? */

    GTEST_SKIP();
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