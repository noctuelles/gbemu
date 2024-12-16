//
// Created by plouvel on 12/14/24.
//

#define TEST_FRIENDS                         \
    friend class CPUTesting;                 \
    friend class CPUTesting_SET_R8_Test;     \
    friend class CPUTesting_RES_R8_Test;     \
    friend class CPUTesting_LD_R8_R8_Test;   \
    friend class CPUTesting_LD_R8_IMM8_Test; \
    friend class CPUTesting_LD_R8_MEM_HL_Test;

#include "CPU.hxx"

#include <gtest/gtest.h>

#include <algorithm>
#include <climits>
#include <random>

#include "Bus.hxx"

/**
 * @brief Unit Tests for the CPU. This is one of the most important part to test in an emulator !
 * @note https://gbdev.io/gb-opcodes/optables/
 */

class CPUTesting : public ::testing::Test
{
  public:
    static constexpr size_t TEST_REPEAT = 5000;

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
        size_t           eat{};

        this->bus->write(this->cpu->reg.PC, instructions);

        for (const auto& instruction : instructions)
        {
            if (eat)
            {
                eat--;
                continue;
            }

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
            if (inst == CPU::Instruction::LD_R8_IMM8())
            {
                eat = 1;
            }
            if (inst == CPU::Instruction::LD_R16_IMM16())
            {
                eat = 2;
            }

            while (inst.cycles--)
            {
                cpu->cycle();
            }
        }

        this->cpu->reg.PC -= instructions.size();
    }
};

static void repeat(size_t nbr, const std::function<void()>& func)
{
    while (nbr--)
    {
        func();
    }
}

TEST_F(CPUTesting, SET_R8)
{
    auto test_set_register = [this](uint8_t opcode)
    {
        uint8_t bit_nbr = 0;

        while (bit_nbr < 8)
        {
            const auto src      = CPU::get_register8_src_from_opcode(opcode);
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
            const auto src      = CPU::get_register8_src_from_opcode(opcode);
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
    auto test_ld_r8_r8 = [this](uint8_t opcode)
    {
        uint8_t                                i = 0;
        std::random_device                     rd{};
        std::mt19937                           gen{rd()};
        std::uniform_int_distribution<uint8_t> dist{0, 255};

        while (i < 8)
        {
            if (i != 6) /* Skip the MEM_HL */
            {
                auto [dest, src]    = CPU::get_register8_dest_src_from_opcode(opcode);
                this->cpu->reg.*src = dist(gen);
                this->execute_instructions({opcode});
                ASSERT_EQ(this->cpu->reg.*dest, this->cpu->reg.*src);
            }
            opcode++;
            i++;
        }
    };

    test_ld_r8_r8(0x78); /* LD A, R8 */
    test_ld_r8_r8(0x40); /* LD B, R8 */
    test_ld_r8_r8(0x48); /* LD C, R8 */
    test_ld_r8_r8(0x50); /* LD D, R8 */
    test_ld_r8_r8(0x58); /* LD E, R8 */
    test_ld_r8_r8(0x60); /* LD H, R8 */
    test_ld_r8_r8(0x68); /* LD L, R8 */
}

TEST_F(CPUTesting, LD_R8_IMM8)
{
    auto test_ld_r8_imm8 = [this](const uint8_t opcode)
    {
        std::random_device                     rd{};
        std::mt19937                           gen{rd()};
        std::uniform_int_distribution<uint8_t> dist{0, 255};
        const auto                             val = dist(gen);

        const auto dest = CPU::get_register8_dest_from_opcode(opcode);

        this->execute_instructions({opcode, val});

        ASSERT_EQ(this->cpu->reg.*dest, val);
    };

    repeat(TEST_REPEAT,
           [test_ld_r8_imm8]()
           {
               SCOPED_TRACE("LD A, n8");
               test_ld_r8_imm8(0x3E);
               SCOPED_TRACE("LD B, n8");
               test_ld_r8_imm8(0x06);
               SCOPED_TRACE("LD C, n8");
               test_ld_r8_imm8(0x0E);
               SCOPED_TRACE("LD D, n8");
               test_ld_r8_imm8(0x16);
               SCOPED_TRACE("LD E, n8");
               test_ld_r8_imm8(0x1E);
               SCOPED_TRACE("LD H, n8");
               test_ld_r8_imm8(0x26);
               SCOPED_TRACE("LD L, n8");
               test_ld_r8_imm8(0x2E);
           });
}

TEST_F(CPUTesting, LD_R8_MEM_HL)
{
    auto test_ld_r8_mem_hl = [this](const uint8_t opcode)
    {
        std::random_device                      rd{};
        std::mt19937                            gen{rd()};
        std::uniform_int_distribution<uint8_t>  dist_u8{0, UINT8_MAX};
        std::uniform_int_distribution<uint16_t> dist_u16{1, Bus::MEMORY_SIZE - 1};

        const auto val  = dist_u8(gen);
        const auto addr = dist_u16(gen);
        const auto dest = CPU::get_register8_dest_from_opcode(opcode);

        this->bus->write(addr, val);
        this->cpu->set_register16(CPU::OperandRegister16::HL, addr);

        this->execute_instructions({opcode});

        ASSERT_EQ(this->cpu->reg.*dest, val);
    };

    repeat(TEST_REPEAT,
           [test_ld_r8_mem_hl]()
           {
               SCOPED_TRACE("LD A, [HL]");
               test_ld_r8_mem_hl(0x7E);
               SCOPED_TRACE("LD B, [HL]");
               test_ld_r8_mem_hl(0x46);
               SCOPED_TRACE("LD C, [HL]");
               test_ld_r8_mem_hl(0x4E);
               SCOPED_TRACE("LD D, [HL]");
               test_ld_r8_mem_hl(0x56);
               SCOPED_TRACE("LD E, [HL]");
               test_ld_r8_mem_hl(0x5E);
               SCOPED_TRACE("LD H, [HL]");
               test_ld_r8_mem_hl(0x66);
               SCOPED_TRACE("LD L, [HL]");
               test_ld_r8_mem_hl(0x6E);
           });
}
