//
// Created by plouvel on 12/30/24.
//

#include <gtest/gtest.h>

#include <Bus.hxx>
#include <CPU.hxx>
#include <cstdint>
#include <initializer_list>

extern std::initializer_list<uint8_t> SPECIAL;
extern std::initializer_list<uint8_t> OP_SP_HL;
extern std::initializer_list<uint8_t> OP_R_IMM;
extern std::initializer_list<uint8_t> OP_RP;
extern std::initializer_list<uint8_t> OP_R_R;
extern std::initializer_list<uint8_t> LD_R_R;
extern std::initializer_list<uint8_t> BIT_OPS;
extern std::initializer_list<uint8_t> JR_JP_CALL_RET_RST;
extern std::initializer_list<uint8_t> MISC;
extern std::initializer_list<uint8_t> OP_A_MEM_HL;

class BlarggInstructions : public testing::Test
{
  protected:
    std::unique_ptr<Bus> bus{};
    std::unique_ptr<SM83> cpu{};

    void SetUp() override
    {
        this->bus = std::make_unique<Bus>();
        this->cpu = std::make_unique<SM83>(*bus);
    }

    void TearDown() override
    {
        this->bus.reset();
        this->cpu.reset();
    }

    void execute_rom(const std::initializer_list<uint8_t> rom) const
    {
        std::string s{};

        this->bus->write(0, rom);
        while (true)
        {
            cpu->tick();

            if (bus->read(0xFF02) == 0x81)
            {
                s += static_cast<char>(bus->read(0xFF01));
                bus->write(0xFF02, 0x00);
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
};

TEST_F(BlarggInstructions, Special01)
{
    ASSERT_NO_THROW(this->execute_rom(SPECIAL));
}

TEST_F(BlarggInstructions, OperationRegisterSP_RegisterHL_03)
{
    ASSERT_NO_THROW(this->execute_rom(OP_SP_HL));
}

TEST_F(BlarggInstructions, OperationRegisterImmediate04)
{
    ASSERT_NO_THROW(this->execute_rom(OP_R_IMM));
}

TEST_F(BlarggInstructions, OperationRegister16_05)
{
    ASSERT_NO_THROW(this->execute_rom(OP_RP));
}

TEST_F(BlarggInstructions, LoadRegisterRegister06)
{
    ASSERT_NO_THROW(this->execute_rom(LD_R_R));
}

TEST_F(BlarggInstructions, JumpCallReturnReset07)
{
    ASSERT_NO_THROW(this->execute_rom(JR_JP_CALL_RET_RST));
}

TEST_F(BlarggInstructions, Misc08)
{
    ASSERT_NO_THROW(this->execute_rom(MISC));
}

TEST_F(BlarggInstructions, OperationRegisterRegister09)
{
    ASSERT_NO_THROW(this->execute_rom(OP_R_R));
}

TEST_F(BlarggInstructions, BitOperations10)
{
    ASSERT_NO_THROW(this->execute_rom(BIT_OPS));
}

TEST_F(BlarggInstructions, OperationRegisterA_MemHL_11)
{
    ASSERT_NO_THROW(this->execute_rom(OP_A_MEM_HL));
}
