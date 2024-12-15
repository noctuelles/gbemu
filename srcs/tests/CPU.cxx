//
// Created by plouvel on 12/14/24.
//

#define TEST_FRIENDS                     \
    friend class CPUTesting;             \
    friend class CPUTesting_SET_R8_Test; \
    friend class CPUTesting_RES_R8_Test; \
    friend class CPUTesting_LD_R8_R8_Test;

#include "CPU.hxx"

#include <gtest/gtest.h>

#include <random>

#include "Bus.hxx"
#include "utils.hxx"

/**
 * @brief Unit Tests for the CPU. This is one of the most important part to test in an emulator !
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

    /**
     * @brief Execute on the fly instructions on the CPU.
     * @param instructions Instructions to be executed by the CPU.
     */
    void execute_instructions(const std::initializer_list<uint8_t> instructions) const
    {
        bool             cb_prefixed{false};
        CPU::Instruction inst{};

        this->bus->write(this->cpu->reg.PC, instructions);

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

        this->cpu->reg.PC -= instructions.size();
    }
};

TEST_F(CPUTesting, SET_R8)
{
    auto test_set_register = [this](uint8_t opcode)
    {
        uint8_t bit_nbr   = 0;

        while (bit_nbr < 8)
        {
            const auto src = CPU::get_register8_src_from_opcode(opcode);
            this->cpu->reg.*src = 0b00000000;

            this->execute_instructions({0xCB, opcode});

            ASSERT_EQ(this->cpu->reg.*src, (1U << bit_nbr));

            opcode += 0x08;
            bit_nbr++;
        }
    };

    test_set_register(0xC7);
    test_set_register(0xC0);
    test_set_register(0xC1);
    test_set_register(0xC2);
    test_set_register(0xC3);
    test_set_register(0xC4);
    test_set_register(0xC5);
}

TEST_F(CPUTesting, RES_R8)
{
    auto test_res_register = [this](uint8_t opcode)
    {
        uint8_t bit_nbr = 0;

        while (bit_nbr < 8)
        {
            const auto src = CPU::get_register8_src_from_opcode(opcode);
            this->cpu->reg.*src = 0b11111111;

            this->execute_instructions({0xCB, opcode});

            ASSERT_EQ(this->cpu->reg.*src, 0b11111111 & ~(1U << bit_nbr));

            opcode += 0x08;
            bit_nbr++;
        }
    };

    test_res_register(0x87); /* RES U3, A */
    test_res_register(0x80); /* RES U3, B */
    test_res_register(0x81); /* RES U3, C */
    test_res_register(0x82); /* RES U3, D */
    test_res_register(0x83); /* RES U3, E */
    test_res_register(0x84); /* RES U3, F */
    test_res_register(0x85); /* RES U3, G */
}

TEST_F(CPUTesting, LD_R8_R8)
{
    auto test_ld_register = [this](const uint8_t base_inst)
    {
        uint8_t                                inst = base_inst;
        uint8_t                                i    = 0;
        std::random_device                     rd{};
        std::mt19937                           gen{rd()};
        std::uniform_int_distribution<uint8_t> dist{0, 255};

        while (i < 8)
        {
            if (i != 6) /* Skip the MEM_HL */
            {
                auto [dest, src]    = CPU::get_register8_dest_src_from_opcode(inst);
                this->cpu->reg.*src = dist(gen);
                this->execute_instructions({inst});
                ASSERT_EQ(this->cpu->reg.*dest, this->cpu->reg.*src);
            }
            inst++;
            i++;
        }
    };

    test_ld_register(0x78); /* LD A, R8 */
    test_ld_register(0x40); /* LD B, R8 */
    test_ld_register(0x48); /* LD C, R8 */
    test_ld_register(0x50); /* LD D, R8 */
    test_ld_register(0x58); /* LD E, R8 */
    test_ld_register(0x60); /* LD H, R8 */
    test_ld_register(0x68); /* LD L, R8 */
}
