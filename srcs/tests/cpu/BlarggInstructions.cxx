//
// Created by plouvel on 12/30/24.
//

#include <gtest/gtest.h>

#include <Bus.hxx>
#include <Cartridge.hxx>
#include <SM83.hxx>
#include <cstdint>
#include <initializer_list>

class BlarggInstructions : public testing::Test
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

    void execute_rom(const std::string& rom_name)
    {
        Cartridge   cart{std::string{ROMS_PATH} + std::string{"/blargg/cpu_instrs/"} + rom_name};
        std::string s{};

        for (std::size_t i = 0; i < cart.get_size(); i++)
        {
            bus->write(i, cart.read(i));
        }

        while (true)
        {
            bus->cpu->tick();

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
    ASSERT_NO_THROW(execute_rom("01-special.gb"));
}

TEST_F(BlarggInstructions, Interrupts02)
{
    ASSERT_NO_THROW(execute_rom("02-interrupts.gb"));
}

TEST_F(BlarggInstructions, OperationRegisterSP_RegisterHL_03)
{
    ASSERT_NO_THROW(execute_rom("03-op sp,hl.gb"));
}

TEST_F(BlarggInstructions, OperationRegisterImmediate04)
{
    ASSERT_NO_THROW(execute_rom("04-op r,imm.gb"));
}

TEST_F(BlarggInstructions, OperationRegister16_05)
{
    ASSERT_NO_THROW(execute_rom("05-op rp.gb"));
}

TEST_F(BlarggInstructions, LoadRegisterRegister06)
{
    ASSERT_NO_THROW(execute_rom("06-ld r,r.gb"));
}

TEST_F(BlarggInstructions, JumpCallReturnReset07)
{
    ASSERT_NO_THROW(execute_rom("07-jr,jp,call,ret,rst.gb"));
}

TEST_F(BlarggInstructions, Misc08)
{
    ASSERT_NO_THROW(execute_rom("08-misc instrs.gb"));
}

TEST_F(BlarggInstructions, OperationRegisterRegister09)
{
    ASSERT_NO_THROW(execute_rom("09-op r,r.gb"));
}

TEST_F(BlarggInstructions, BitOperations10)
{
    ASSERT_NO_THROW(execute_rom("10-bit ops.gb"));
}

TEST_F(BlarggInstructions, OperationRegisterA_MemHL_11)
{
    ASSERT_NO_THROW(execute_rom("11-op a,(hl).gb"));
}
