//
// Created by plouvel on 12/14/24.
//

#define TEST_FRIENDS                           \
    friend class CPUTesting;                   \
    friend class CPUTesting_SET_R8_Test;       \
    friend class CPUTesting_SET_MEM_HL_Test;   \
    friend class CPUTesting_RES_R8_Test;       \
    friend class CPUTesting_RES_MEM_HL_Test;   \
    friend class CPUTesting_BIT_Test;          \
    friend class CPUTesting_SWAP_R8_Test;      \
    friend class CPUTesting_SWAP_MEM_HL_Test;  \
    friend class CPUTesting_SRL_R8_Test;       \
    friend class CPUTesting_SRL_MEM_HL_Test;   \
    friend class CPUTesting_SRA_R8_Test;       \
    friend class CPUTesting_SRA_MEM_HL_Test;   \
    friend class CPUTesting_SLA_R8_Test;       \
    friend class CPUTesting_SLA_MEM_HL_Test;   \
    friend class CPUTesting_RL_R8_Test;        \
    friend class CPUTesting_RL_MEM_HL_Test;    \
    friend class CPUTesting_RR_R8_Test;        \
    friend class CPUTesting_RR_MEM_HL_Test;    \
    friend class CPUTesting_LD_R8_R8_Test;     \
    friend class CPUTesting_LD_R8_IMM8_Test;   \
    friend class CPUTesting_LD_R8_MEM_HL_Test; \
    friend class CPUTesting_LD_MEM_HL_R8_Test; \
    friend class CPUTesting_LD_R16_IMM16_Test; \
    friend class CPUTesting_JP_CC_IMM16_Test;  \
    friend class CPUTesting_JP_IMM16_Test;     \
    friend class CPUTesting_JP_HL_Test;

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
    static constexpr size_t TEST_REPEAT = 10000;

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

  public:
    /**
     * @brief Execute on the fly instructions on the CPU.
     * @param instruction Instructions to be executed by the CPU.
     */
    void execute_instruction(const std::initializer_list<uint8_t> instruction) const
    {
        this->bus->write(this->cpu->reg.u16.PC, instruction);
        while (cpu->cycle() != 0);
    }
};

static void repeat(size_t nbr, const std::function<void()>& func)
{
    while (nbr--)
    {
        func();
    }
}

/**
 * @brief CB-prefixed opcodes.
 */

TEST_F(CPUTesting, SET_R8)
{
    auto test_set_register = [this](uint8_t opcode)
    {
        uint8_t bit_nbr = 0;

        while (bit_nbr < 8)
        {
            const auto src         = CPU::get_register8_src_from_opcode(opcode);
            this->cpu->reg.u8.*src = 0b00000000;

            this->execute_instruction({0xCB, opcode});

            ASSERT_EQ(this->cpu->reg.u8.*src, (1U << bit_nbr));

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

TEST_F(CPUTesting, SET_MEM_HL)
{
    auto test_set_mem_hl = [this](uint8_t opcode)
    {
        uint8_t bit_nbr = 0;

        while (bit_nbr < 8)
        {
            constexpr uint16_t addr = 0x0CB8U;
            this->cpu->reg.u16.HL   = addr;
            this->bus->write(addr, 0b00000000U);

            this->execute_instruction({0xCB, opcode});

            ASSERT_EQ(this->bus->read(addr), (1U << bit_nbr));

            opcode += 0x08;
            bit_nbr++;
        }
    };

    SCOPED_TRACE("SET U3, [HL]");
    test_set_mem_hl(0xC6);
}

TEST_F(CPUTesting, RES_R8)
{
    auto test_res_register = [this](uint8_t opcode)
    {
        uint8_t bit_nbr = 0;

        while (bit_nbr < 8)
        {
            const auto src         = CPU::get_register8_src_from_opcode(opcode);
            this->cpu->reg.u8.*src = 0b11111111;

            this->execute_instruction({0xCB, opcode});

            ASSERT_EQ(this->cpu->reg.u8.*src, 0b11111111 & ~(1U << bit_nbr));

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

TEST_F(CPUTesting, RES_MEM_HL)
{
    auto test_res_mem_hl = [this](uint8_t opcode)
    {
        uint8_t bit_nbr = 0;

        while (bit_nbr < 8)
        {
            constexpr uint16_t addr = 0x1012U;

            this->cpu->reg.u16.HL = addr;
            this->bus->write(addr, 0b11111111U);

            this->execute_instruction({0xCB, opcode});

            ASSERT_EQ(this->bus->read(addr), 0b11111111U & ~(1U << bit_nbr));

            opcode += 0x08;
            bit_nbr++;
        }
    };

    test_res_mem_hl(0x86); /* RES U3, [HL] */
}

TEST_F(CPUTesting, BIT)
{
    auto test_bit = [this](uint8_t opcode, const bool mem_hl = false)
    {
        uint8_t bit_nbr = 0;

        while (bit_nbr < 8)
        {
            const uint8_t      val  = (1U << bit_nbr);
            constexpr uint16_t addr = 0x1012U;
            const auto         src  = !mem_hl ? CPU::get_register8_src_from_opcode(opcode) : nullptr;

            if (mem_hl)
            {
                this->cpu->reg.u16.HL = addr;
                this->bus->write(addr, val);
            }
            else
            {
                this->cpu->reg.u8.*src = val;
            }

            this->cpu->reg.u8.F |= CPU::Flags::SUBTRACT; /* This flag should be reset. */
            this->cpu->reg.u8.F |= CPU::Flags::CARRY;    /* This flag should not be modified. */

            this->execute_instruction({0xCB, opcode});

            ASSERT_EQ(this->cpu->reg.u8.F, CPU::Flags::HALF_CARRY | CPU::Flags::CARRY);

            if (mem_hl)
            {
                this->bus->write(addr, this->bus->read(addr) ^ val);
            }
            else
            {
                this->cpu->reg.u8.*src ^= val;
            }

            this->execute_instruction({0xCB, opcode});

            ASSERT_EQ(this->cpu->reg.u8.F, CPU::Flags::HALF_CARRY | CPU::Flags::CARRY | CPU::Flags::ZERO);

            opcode += 0x08;
            bit_nbr++;
        }
    };

    SCOPED_TRACE("BIT U3, A");
    test_bit(0x47);
    SCOPED_TRACE("BIT U3, B");
    test_bit(0x40);
    SCOPED_TRACE("BIT U3, C");
    test_bit(0x41);
    SCOPED_TRACE("BIT U3, D");
    test_bit(0x42);
    SCOPED_TRACE("BIT U3, E");
    test_bit(0x43);
    SCOPED_TRACE("BIT U3, H");
    test_bit(0x44);
    SCOPED_TRACE("BIT U3, L");
    test_bit(0x45);
    SCOPED_TRACE("BIT U3, [HL]");
    test_bit(0x46, true);
}

TEST_F(CPUTesting, SWAP_R8)
{
    auto test_swap = [this](uint8_t opcode)
    {
        const auto        src         = CPU::get_register8_src_from_opcode(opcode);
        constexpr uint8_t val         = 0b10100101U;
        constexpr uint8_t swapped_val = 0b01011010U;

        this->cpu->reg.u8.*src = val;
        this->cpu->reg.u8.F |=
            CPU::Flags::SUBTRACT | CPU::Flags::CARRY | CPU::Flags::HALF_CARRY; /* These flags should be cleared. */

        this->execute_instruction({0xCB, opcode});

        ASSERT_EQ(this->cpu->reg.u8.*src, swapped_val);
        ASSERT_EQ(this->cpu->reg.u8.F, 0);

        this->cpu->reg.u8.*src = 0;

        this->execute_instruction({0xCB, opcode});

        ASSERT_EQ(this->cpu->reg.u8.*src, 0);
        ASSERT_EQ(this->cpu->reg.u8.F, CPU::Flags::ZERO);
    };

    SCOPED_TRACE("SWAP A");
    test_swap(0x37);
    SCOPED_TRACE("SWAP B");
    test_swap(0x30);
    SCOPED_TRACE("SWAP C");
    test_swap(0x31);
    SCOPED_TRACE("SWAP D");
    test_swap(0x32);
    SCOPED_TRACE("SWAP E");
    test_swap(0x33);
    SCOPED_TRACE("SWAP H");
    test_swap(0x34);
    SCOPED_TRACE("SWAP L");
    test_swap(0x35);
}

TEST_F(CPUTesting, SWAP_MEM_HL)
{
    auto test_swap = [this](uint8_t opcode)
    {
        constexpr uint16_t addr        = 0x0B8;
        constexpr uint8_t  val         = 0b10100101U;
        constexpr uint8_t  swapped_val = 0b01011010U;

        this->cpu->reg.u16.HL = addr;
        this->bus->write(addr, val);
        this->cpu->reg.u8.F |=
            CPU::Flags::SUBTRACT | CPU::Flags::CARRY | CPU::Flags::HALF_CARRY; /* These flags should be cleared. */

        this->execute_instruction({0xCB, opcode});

        ASSERT_EQ(this->bus->read(addr), swapped_val);
        ASSERT_EQ(this->cpu->reg.u8.F, 0);

        this->bus->write(addr, 0);

        this->execute_instruction({0xCB, opcode});

        ASSERT_EQ(this->bus->read(addr), 0);
        ASSERT_EQ(this->cpu->reg.u8.F, CPU::Flags::ZERO);
    };

    SCOPED_TRACE("SWAP [HL]");
    test_swap(0x36);
}

TEST_F(CPUTesting, SRL_R8)
{
    auto test_srl = [this](uint8_t opcode)
    {
        const auto src       = CPU::get_register8_src_from_opcode(opcode);
        uint8_t    val       = 0b10100101U;
        uint8_t    shift_val = 0b01010010U;

        this->cpu->reg.u8.F |= CPU::Flags::SUBTRACT | CPU::Flags::HALF_CARRY; /* These flags should be cleared. */

        this->cpu->reg.u8.*src = val;
        this->execute_instruction({0xCB, opcode});
        ASSERT_EQ(this->cpu->reg.u8.*src, shift_val);
        ASSERT_EQ(this->cpu->reg.u8.F, CPU::Flags::CARRY);

        val       = 0b00000001U;
        shift_val = 0b00000000U;

        this->cpu->reg.u8.*src = val;
        this->execute_instruction({0xCB, opcode});
        ASSERT_EQ(this->cpu->reg.u8.*src, shift_val);
        ASSERT_EQ(this->cpu->reg.u8.F, CPU::Flags::CARRY | CPU::Flags::ZERO);

        val       = 0b00010000U;
        shift_val = 0b00001000U;

        this->cpu->reg.u8.*src = val;
        this->execute_instruction({0xCB, opcode});
        ASSERT_EQ(this->cpu->reg.u8.*src, shift_val);
        ASSERT_EQ(this->cpu->reg.u8.F, 0);
    };

    SCOPED_TRACE("SRL A");
    test_srl(0x3F);
    SCOPED_TRACE("SRL B");
    test_srl(0x38);
    SCOPED_TRACE("SRL C");
    test_srl(0x39);
    SCOPED_TRACE("SRL D");
    test_srl(0x3A);
    SCOPED_TRACE("SRL E");
    test_srl(0x3B);
    SCOPED_TRACE("SRL H");
    test_srl(0x3C);
    SCOPED_TRACE("SRL L");
    test_srl(0x3D);
}

TEST_F(CPUTesting, SRA_R8)
{
    auto test_srl = [this](uint8_t opcode)
    {
        const auto src       = CPU::get_register8_src_from_opcode(opcode);
        uint8_t    val       = 0b10100101U;
        uint8_t    shift_val = 0b11010010U; /* Sign bit is preserved : 1 */

        this->cpu->reg.u8.F |= CPU::Flags::SUBTRACT | CPU::Flags::HALF_CARRY; /* These flags should be cleared. */

        this->cpu->reg.u8.*src = val;
        this->execute_instruction({0xCB, opcode});
        ASSERT_EQ(this->cpu->reg.u8.*src, shift_val);
        ASSERT_EQ(this->cpu->reg.u8.F, CPU::Flags::CARRY);

        val       = 0b00000001U;
        shift_val = 0b00000000U; /* Sign bit is preserved : 0 */

        this->cpu->reg.u8.*src = val;
        this->execute_instruction({0xCB, opcode});
        ASSERT_EQ(this->cpu->reg.u8.*src, shift_val);
        ASSERT_EQ(this->cpu->reg.u8.F, CPU::Flags::CARRY | CPU::Flags::ZERO);

        val       = 0b00100101U;
        shift_val = 0b00010010U; /* Sign bit is preserved : 0 */

        this->cpu->reg.u8.*src = val;
        this->execute_instruction({0xCB, opcode});
        ASSERT_EQ(this->cpu->reg.u8.*src, shift_val);
        ASSERT_EQ(this->cpu->reg.u8.F, CPU::Flags::CARRY);
    };

    SCOPED_TRACE("SRA A");
    test_srl(0x2F);
    SCOPED_TRACE("SRA B");
    test_srl(0x28);
    SCOPED_TRACE("SRA C");
    test_srl(0x29);
    SCOPED_TRACE("SRA D");
    test_srl(0x2A);
    SCOPED_TRACE("SRA E");
    test_srl(0x2B);
    SCOPED_TRACE("SRA H");
    test_srl(0x2C);
    SCOPED_TRACE("SRA L");
    test_srl(0x2D);
}

TEST_F(CPUTesting, SLA_R8)
{
    auto test_sla = [this](uint8_t opcode)
    {
        const auto src       = CPU::get_register8_src_from_opcode(opcode);
        uint8_t    val       = 0b00100101U;
        uint8_t    shift_val = 0b01001010U;

        this->cpu->reg.u8.F |= CPU::Flags::SUBTRACT | CPU::Flags::HALF_CARRY; /* These flags should be cleared. */

        this->cpu->reg.u8.*src = val;
        this->execute_instruction({0xCB, opcode});
        ASSERT_EQ(this->cpu->reg.u8.*src, shift_val);
        ASSERT_EQ(this->cpu->reg.u8.F, 0);

        val       = 0b10000000U;
        shift_val = 0b00000000U;

        this->cpu->reg.u8.*src = val;
        this->execute_instruction({0xCB, opcode});
        ASSERT_EQ(this->cpu->reg.u8.*src, shift_val);
        ASSERT_EQ(this->cpu->reg.u8.F, CPU::Flags::CARRY | CPU::Flags::ZERO);

        val       = 0b00100101U;
        shift_val = 0b01001010U;

        this->cpu->reg.u8.*src = val;
        this->execute_instruction({0xCB, opcode});
        ASSERT_EQ(this->cpu->reg.u8.*src, shift_val);
        ASSERT_EQ(this->cpu->reg.u8.F, 0);
    };

    SCOPED_TRACE("SLA A");
    test_sla(0x27);
    SCOPED_TRACE("SLA B");
    test_sla(0x20);
    SCOPED_TRACE("SLA C");
    test_sla(0x21);
    SCOPED_TRACE("SLA D");
    test_sla(0x22);
    SCOPED_TRACE("SLA E");
    test_sla(0x23);
    SCOPED_TRACE("SLA H");
    test_sla(0x24);
    SCOPED_TRACE("SLA L");
    test_sla(0x25);
}

TEST_F(CPUTesting, RL_R8)
{
    auto test_rl = [this](uint8_t opcode)
    {
        const auto src     = CPU::get_register8_src_from_opcode(opcode);
        uint8_t    val     = 0b00100100U;
        uint8_t    rot_val = 0b01001000U;

        this->cpu->reg.u8.*src = val;
        this->cpu->reg.u8.F    = CPU::Flags::SUBTRACT | CPU::Flags::HALF_CARRY; /* These flags should be cleared. */

        this->execute_instruction({0xCB, opcode});
        ASSERT_EQ(this->cpu->reg.u8.*src, rot_val);
        ASSERT_EQ(this->cpu->reg.u8.F, 0);

        val     = 0b10000000U;
        rot_val = 0b00000000U;

        this->cpu->reg.u8.*src = val;
        this->execute_instruction({0xCB, opcode});
        ASSERT_EQ(this->cpu->reg.u8.*src, rot_val);
        ASSERT_EQ(this->cpu->reg.u8.F, CPU::Flags::CARRY | CPU::Flags::ZERO);

        val     = 0b00000000U;
        rot_val = 0b00000001U;

        this->cpu->reg.u8.*src = val;
        this->execute_instruction({0xCB, opcode});
        ASSERT_EQ(this->cpu->reg.u8.*src, rot_val);
        ASSERT_EQ(this->cpu->reg.u8.F, 0);
    };

    SCOPED_TRACE("RL A");
    test_rl(0x17);

    SCOPED_TRACE("RL B");
    test_rl(0x10);

    SCOPED_TRACE("RL C");
    test_rl(0x11);

    SCOPED_TRACE("RL D");
    test_rl(0x12);

    SCOPED_TRACE("RL E");
    test_rl(0x13);

    SCOPED_TRACE("RL H");
    test_rl(0x14);

    SCOPED_TRACE("RL L");
    test_rl(0x15);
}

TEST_F(CPUTesting, RR_R8)
{
    auto test_rl = [this](uint8_t opcode)
    {
        const auto src     = CPU::get_register8_src_from_opcode(opcode);
        uint8_t    val     = 0b00100100U;
        uint8_t    rot_val = 0b00010010U;

        this->cpu->reg.u8.*src = val;
        this->cpu->reg.u8.F    = CPU::Flags::SUBTRACT | CPU::Flags::HALF_CARRY; /* These flags should be cleared. */

        this->execute_instruction({0xCB, opcode});
        ASSERT_EQ(this->cpu->reg.u8.*src, rot_val);
        ASSERT_EQ(this->cpu->reg.u8.F, 0);

        val     = 0b00110001U;
        rot_val = 0b00011000U;

        this->cpu->reg.u8.*src = val;
        this->execute_instruction({0xCB, opcode});
        ASSERT_EQ(this->cpu->reg.u8.*src, rot_val);
        ASSERT_EQ(this->cpu->reg.u8.F, CPU::Flags::CARRY);

        val     = 0b00000100U;
        rot_val = 0b10000010U;

        this->cpu->reg.u8.*src = val;
        this->execute_instruction({0xCB, opcode});
        ASSERT_EQ(this->cpu->reg.u8.*src, rot_val);
        ASSERT_EQ(this->cpu->reg.u8.F, 0);
    };

    SCOPED_TRACE("RR A");
    test_rl(0x1F);

    SCOPED_TRACE("RR B");
    test_rl(0x18);

    SCOPED_TRACE("RR C");
    test_rl(0x19);

    SCOPED_TRACE("RR D");
    test_rl(0x1A);

    SCOPED_TRACE("RR E");
    test_rl(0x1B);

    SCOPED_TRACE("RR H");
    test_rl(0x1C);

    SCOPED_TRACE("RR L");
    test_rl(0x1D);
}

TEST_F(CPUTesting, LD_R8_R8)
{
    auto test_ld_r8_r8 = [this](uint8_t opcode)
    {
        uint8_t                                i{};
        std::random_device                     rd{};
        std::mt19937                           gen{rd()};
        std::uniform_int_distribution<uint8_t> dist{0, 255};

        while (i < 8)
        {
            if (i != 6) /* Skip the MEM_HL */
            {
                auto [dest, src]       = CPU::get_register8_dest_src_from_opcode(opcode);
                this->cpu->reg.u8.*src = dist(gen);
                this->execute_instruction({opcode});
                ASSERT_EQ(this->cpu->reg.u8.*dest, this->cpu->reg.u8.*src);
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
        const auto                             val{dist(gen)};
        const auto                             dest{CPU::get_register8_dest_from_opcode(opcode)};

        this->execute_instruction({opcode, val});

        ASSERT_EQ(this->cpu->reg.u8.*dest, val);
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
        const auto                              val{dist_u8(gen)};
        const auto                              addr{dist_u16(gen)};
        const auto                              dest{CPU::get_register8_dest_from_opcode(opcode)};

        this->bus->write(addr, val);
        this->cpu->reg.u16.HL = addr;

        this->execute_instruction({opcode});

        ASSERT_EQ(this->cpu->reg.u8.*dest, val);
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

TEST_F(CPUTesting, LD_MEM_HL_R8)
{
    auto test_ld_mem_hl_r8 = [this](const uint8_t opcode)
    {
        std::random_device                      rd{};
        std::mt19937                            gen{rd()};
        std::uniform_int_distribution<uint8_t>  dist_u8{0, UINT8_MAX};
        std::uniform_int_distribution<uint16_t> dist_u16{1, Bus::MEMORY_SIZE - 1};

        const auto val  = dist_u8(gen);
        const auto addr = dist_u16(gen);
        const auto src  = CPU::get_register8_src_from_opcode(opcode);

        this->cpu->reg.u16.HL = addr;

        if (src != &CPU::Register::U8::H && src != &CPU::Register::U8::L)
        {
            this->cpu->reg.u8.*src = val;
        }

        this->execute_instruction({opcode});

        if (src != &CPU::Register::U8::H && src != &CPU::Register::U8::L)
        {
            ASSERT_EQ(this->bus->read(addr), val);
        }
        else
        {
            ASSERT_EQ(this->bus->read(addr), this->cpu->reg.u8.*src);
        }
    };

    repeat(TEST_REPEAT,
           [test_ld_mem_hl_r8]()
           {
               SCOPED_TRACE("LD [HL], A");
               test_ld_mem_hl_r8(0x77);
               SCOPED_TRACE("LD [HL], B");
               test_ld_mem_hl_r8(0x70);
               SCOPED_TRACE("LD [HL], C");
               test_ld_mem_hl_r8(0x71);
               SCOPED_TRACE("LD [HL], D");
               test_ld_mem_hl_r8(0x72);
               SCOPED_TRACE("LD [HL], E");
               test_ld_mem_hl_r8(0x73);
               SCOPED_TRACE("LD [HL], H");
               test_ld_mem_hl_r8(0x74);
               SCOPED_TRACE("LD [HL], L");
               test_ld_mem_hl_r8(0x75);
           });
}

TEST_F(CPUTesting, LD_R16_IMM16)
{
    auto test_ld_r16_imm16 = [this](const uint8_t opcode)
    {
        std::random_device                      rd{};
        std::mt19937                            gen{rd()};
        std::uniform_int_distribution<uint16_t> dist{0, UINT16_MAX};
        const auto                              val{dist(gen)};
        const auto                              dest{CPU::get_register16_dest_from_opcode(opcode)};

        this->execute_instruction(
            {opcode, static_cast<uint8_t>(val & 0xFFU), static_cast<uint8_t>((val >> 8) & 0xFFU)});

        ASSERT_EQ(this->cpu->reg.u16.*dest, val);
    };

    repeat(TEST_REPEAT,
           [test_ld_r16_imm16]()
           {
               {
                   SCOPED_TRACE("LD BC, imm16");
                   test_ld_r16_imm16(0x01);
               }
               {
                   SCOPED_TRACE("LD DE, imm16");
                   test_ld_r16_imm16(0x11);
               }
               {
                   SCOPED_TRACE("LD HL, imm16");
                   test_ld_r16_imm16(0x21);
               }
               {
                   SCOPED_TRACE("LD SP, imm16");
                   test_ld_r16_imm16(0x31);
               }
           });
}

auto TEST_JP_XX_IMM16(const CPUTesting* this_ptr, uint8_t opcode)
{
    std::random_device                      rd{};
    std::mt19937                            gen{rd()};
    std::uniform_int_distribution<uint16_t> dist{0x100, Bus::MEMORY_SIZE - 1};
    const auto                              addr = dist(gen);

    this_ptr->execute_instruction(
        {opcode, static_cast<uint8_t>(addr & 0xFF), static_cast<uint8_t>((addr & 0xFF00) >> 8)});

    return addr;
}

TEST_F(CPUTesting, JP_HL)
{
    repeat(TEST_REPEAT,
           [this]()
           {
               std::random_device                      rd{};
               std::mt19937                            gen{rd()};
               std::uniform_int_distribution<uint16_t> dist{0x100, Bus::MEMORY_SIZE - 1};
               const auto                              addr = dist(gen);

               this->cpu->reg.u16.PC = 0;
               this->cpu->reg.u16.HL = addr;
               this->execute_instruction({0xE9});
               ASSERT_EQ(this->cpu->reg.u16.PC, this->cpu->reg.u16.PC);
           });
}

TEST_F(CPUTesting, JP_IMM16)
{
    repeat(TEST_REPEAT,
           [this]()
           {
               SCOPED_TRACE("JP a16");
               ASSERT_EQ(this->cpu->reg.u16.PC, TEST_JP_XX_IMM16(this, 0xC3));
               this->cpu->reg.u16.PC = 0;
           });
}

TEST_F(CPUTesting, JP_CC_IMM16)
{
    repeat(TEST_REPEAT,
           [this]
           {
               {
                   /* Jump if flag ZERO is not set. */
                   SCOPED_TRACE("JP NZ, A16");

                   this->cpu->reg.u8.F &= ~CPU::Flags::ZERO;
                   ASSERT_EQ(this->cpu->reg.u16.PC, TEST_JP_XX_IMM16(this, 0xC2));
                   this->cpu->reg.u16.PC = 0;

                   this->cpu->reg.u8.F |= CPU::Flags::ZERO;
                   ASSERT_NE(this->cpu->reg.u16.PC, TEST_JP_XX_IMM16(this, 0xC2));
                   this->cpu->reg.u16.PC = 0;
               }
               {
                   /* Jump if flag ZERO is set. */
                   SCOPED_TRACE("JP Z, A16");

                   this->cpu->reg.u8.F |= CPU::Flags::ZERO;
                   ASSERT_EQ(this->cpu->reg.u16.PC, TEST_JP_XX_IMM16(this, 0xCA));
                   this->cpu->reg.u16.PC = 0;

                   this->cpu->reg.u8.F &= ~CPU::Flags::ZERO;
                   ASSERT_NE(this->cpu->reg.u16.PC, TEST_JP_XX_IMM16(this, 0xCA));
                   this->cpu->reg.u16.PC = 0;
               }
               {
                   /* Jump if flag CARRY is not set. */
                   SCOPED_TRACE("JP NC, A16");

                   this->cpu->reg.u8.F &= ~CPU::Flags::CARRY;
                   ASSERT_EQ(this->cpu->reg.u16.PC, TEST_JP_XX_IMM16(this, 0xD2));
                   this->cpu->reg.u16.PC = 0;

                   this->cpu->reg.u8.F |= CPU::Flags::CARRY;
                   ASSERT_NE(this->cpu->reg.u16.PC, TEST_JP_XX_IMM16(this, 0xD2));
                   this->cpu->reg.u16.PC = 0;
               }

               {
                   /* Jump if flag CARRY is set. */
                   SCOPED_TRACE("JP C, A16");

                   this->cpu->reg.u8.F |= CPU::Flags::CARRY;
                   ASSERT_EQ(this->cpu->reg.u16.PC, TEST_JP_XX_IMM16(this, 0xDA));
                   this->cpu->reg.u16.PC = 0;

                   this->cpu->reg.u8.F &= ~CPU::Flags::CARRY;
                   ASSERT_NE(this->cpu->reg.u16.PC, TEST_JP_XX_IMM16(this, 0xDA));
                   this->cpu->reg.u16.PC = 0;
               }
           });
}