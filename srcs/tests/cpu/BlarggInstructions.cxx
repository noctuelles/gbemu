//
// Created by plouvel on 12/30/24.
//

#include <gtest/gtest.h>

#include <cstdint>
#include <fstream>
#include <initializer_list>
#include <istream>

#include "hardware/Bus.hxx"
#include "hardware/Cartridge.hxx"
#include "hardware/core/SM83.hxx"

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

class BlarggInstructions : public testing::Test
{
  protected:
    std::unique_ptr<Bus>     bus{};
    std::unique_ptr<SM83>    cpu{};
    std::unique_ptr<FakeRAM> ram{};

    void SetUp() override
    {
        bus = std::make_unique<Bus>();

        cpu = std::make_unique<SM83>(*bus, [] {});
        ram = std::make_unique<FakeRAM>();

        bus->attach(*cpu);
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
        std::ifstream input{std::string{ROMS_PATH} + std::string{"/blargg/cpu_instrs/"} + rom_name, std::ios::binary};
        std::string s{};

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
