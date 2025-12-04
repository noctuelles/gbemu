//
// Created by plouvel on 12/30/24.
//

#include "tests/roms/BlarggInstructions.hxx"

void BlarggInstructions::executeROM(const std::string& rom_name)
{
    std::string s{};

    loadROM(ROMS_PATH + std::string{"/blargg/cpu_instrs/"} + rom_name);

    _component->cpu.setPostBootRomRegisters();
    _component->bus.setPostBootRomRegisters();

    while (true)
    {
        _component->cpu.runInstruction();

        if (_component->bus.read(0xFF02) == 0x81)
        {
            s += static_cast<char>(_component->bus.read(0xFF01));
            _component->bus.write(0xFF02, 0x00);
        }

        if (s.ends_with("Passed"))
        {
            break;
        }
        if (s.ends_with("Failed"))
        {
            throw std::runtime_error(s);
        }
    }
}

TEST_F(BlarggInstructions, Special01)
{
    ASSERT_NO_THROW(executeROM("01-special.gb"));
}

TEST_F(BlarggInstructions, Interrupts02)
{
    ASSERT_NO_THROW(executeROM("02-interrupts.gb"));
}

TEST_F(BlarggInstructions, OperationRegisterSP_RegisterHL_03)
{
    ASSERT_NO_THROW(executeROM("03-op sp,hl.gb"));
}

TEST_F(BlarggInstructions, OperationRegisterImmediate04)
{
    ASSERT_NO_THROW(executeROM("04-op r,imm.gb"));
}

TEST_F(BlarggInstructions, OperationRegister16_05)
{
    ASSERT_NO_THROW(executeROM("05-op rp.gb"));
}

TEST_F(BlarggInstructions, LoadRegisterRegister06)
{
    ASSERT_NO_THROW(executeROM("06-ld r,r.gb"));
}

TEST_F(BlarggInstructions, JumpCallReturnReset07)
{
    ASSERT_NO_THROW(executeROM("07-jr,jp,call,ret,rst.gb"));
}

TEST_F(BlarggInstructions, Misc08)
{
    ASSERT_NO_THROW(executeROM("08-misc instrs.gb"));
}

TEST_F(BlarggInstructions, OperationRegisterRegister09)
{
    ASSERT_NO_THROW(executeROM("09-op r,r.gb"));
}

TEST_F(BlarggInstructions, BitOperations10)
{
    ASSERT_NO_THROW(executeROM("10-bit ops.gb"));
}

TEST_F(BlarggInstructions, OperationRegisterA_MemHL_11)
{
    ASSERT_NO_THROW(executeROM("11-op a,(hl).gb"));
}

TEST_F(BlarggInstructions, Timing)
{
    ASSERT_NO_THROW(executeROM("instr_timing.gb"));
}
