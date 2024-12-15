//
// Created by plouvel on 12/14/24.
//

#include "CPU.hxx"

#include <gtest/gtest.h>

#include "Bus.hxx"

/**
 * @note https://gbdev.io/gb-opcodes/optables/
 */

class CPUTesting : public ::testing::Test
{
  protected:
    CPU* cpu = nullptr;
    Bus* bus = nullptr;

    void SetUp() override
    {
        bus = new Bus();
        cpu = new CPU(*bus);
    }

    void TearDown() override
    {
        delete bus;
        delete cpu;
    }

    void execute_instructions(const std::initializer_list<uint8_t> instructions) const
    {
        bool             cb_prefixed{false};
        CPU::Instruction inst{};

        this->bus->write(this->cpu->get_register().PC, instructions);

        for (const auto& instruction : instructions)
        {
            if (cb_prefixed)
            {
                inst        = CPU::cb_prefixed_inst_lookup[instruction];
                cb_prefixed = false;
            }
            else
            {
                inst = CPU::inst_lookup[instruction];
            }

            if (inst == CPU::Instruction::PREFIX())
            {
                cb_prefixed = true;
            }
            while (inst.cycles--)
            {
                cpu->cycle();
            }
        }
    }
};

TEST_F(CPUTesting, SET_R8)
{
    auto test_set_register = [this](const uint8_t base_inst, auto cpu_reg)
    {
        uint8_t inst      = base_inst;
        uint8_t bit_nbr   = 0;
        uint8_t reg_value = 0;

        while (bit_nbr < 8)
        {
            this->execute_instructions({0xCB, inst});

            reg_value |= (1 << bit_nbr);
            ASSERT_EQ(cpu_reg(), reg_value);

            inst += 0x08;
            bit_nbr++;
        }
    };

    test_set_register(0xC7, [this]() { return this->cpu->get_register().A; });
    test_set_register(0xC0, [this]() { return this->cpu->get_register().B; });
    test_set_register(0xC1, [this]() { return this->cpu->get_register().C; });
    test_set_register(0xC2, [this]() { return this->cpu->get_register().D; });
    test_set_register(0xC3, [this]() { return this->cpu->get_register().E; });
    test_set_register(0xC4, [this]() { return this->cpu->get_register().H; });
    test_set_register(0xC5, [this]() { return this->cpu->get_register().L; });
}


TEST_F(CPUTesting, RES_R8)
{
    auto test_res_register = [this](const uint8_t base_inst, auto cpu_reg)
    {
        uint8_t inst      = base_inst;
        uint8_t bit_nbr   = 0;

        while (bit_nbr < 8)
        {
            /* Execute a SET instruction */
            this->execute_instructions({0xCB, inst + 0x40});
            /* And then a RES instruction */
            this->execute_instructions({0xCB, inst});

            /* Should result in a zero register. */
            ASSERT_EQ(cpu_reg(), 0b00000000);

            inst += 0x08;
            bit_nbr++;
        }
    };

    test_res_register(0x87, [this]() { return this->cpu->get_register().A; });
    test_res_register(0x80, [this]() { return this->cpu->get_register().B; });
    test_res_register(0x81, [this]() { return this->cpu->get_register().C; });
    test_res_register(0x82, [this]() { return this->cpu->get_register().D; });
    test_res_register(0x83, [this]() { return this->cpu->get_register().E; });
    test_res_register(0x84, [this]() { return this->cpu->get_register().H; });
    test_res_register(0x85, [this]() { return this->cpu->get_register().L; });
}
