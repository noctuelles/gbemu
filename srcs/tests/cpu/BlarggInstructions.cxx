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
    std::unique_ptr<Bus>  bus{};
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

    void execute_rom(const std::string& rom_name) const
    {
        Cartridge   cart{std::string{ROMS_PATH} + std::string{"/blargg/cpu_instrs/"} + rom_name};
        std::string s{};

        for (std::size_t i = 0; i < cart.get_size(); i++)
        {
            bus->write(i, cart.read(i));
        }

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

TEST_F(BlarggInstructions, Special01) {}

TEST_F(BlarggInstructions, OperationRegisterSP_RegisterHL_03) {}

TEST_F(BlarggInstructions, OperationRegisterImmediate04) {}

TEST_F(BlarggInstructions, OperationRegister16_05) {}

TEST_F(BlarggInstructions, LoadRegisterRegister06)
{
    ASSERT_NO_THROW(execute_rom("06-ld r,r.gb"));
}

TEST_F(BlarggInstructions, JumpCallReturnReset07) {}

TEST_F(BlarggInstructions, Misc08) {}

TEST_F(BlarggInstructions, OperationRegisterRegister09) {}

TEST_F(BlarggInstructions, BitOperations10) {}

TEST_F(BlarggInstructions, OperationRegisterA_MemHL_11) {}
