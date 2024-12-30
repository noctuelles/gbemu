//
// Created by plouvel on 12/29/24.
//

#include <format>

#include "tests/CPU.hxx"
#include "Utils.hxx"

struct LD_R8_R8_Params : TestInstructionParam
{
    CPU::Register8 dst;
    CPU::Register8 src;
};

class LD_R8_R8 : public CPUTesting, public ::testing::WithParamInterface<LD_R8_R8_Params>
{
};

TEST_P(LD_R8_R8, Value)
{
    const auto& param              = GetParam();
    this->cpu->reg.u16.PC          = 0;
    this->cpu->reg.u8.*(param.src) = generate_byte();
    this->cpu->reg.u8.*(param.dst) = 0x42;
    this->execute_instruction({param.opcode}, 4);
    ASSERT_EQ(this->cpu->reg.u8.*param.dst, this->cpu->reg.u8.*param.src);
}

TEST_P(LD_R8_R8, Flags)
{
    const auto& param{GetParam()};
    const auto  byte{generate_byte()};
    this->cpu->reg.u8.F = byte;
    this->execute_instruction({param.opcode}, 4);
    ASSERT_EQ(this->cpu->reg.u8.F, byte);
}

INSTANTIATE_TEST_SUITE_P(Main, LD_R8_R8,
                         ::testing::Values(LD_R8_R8_Params{0x40, &CPU::Register::U8::B, &CPU::Register::U8::B},
                                           LD_R8_R8_Params{0x41, &CPU::Register::U8::B, &CPU::Register::U8::C},
                                           LD_R8_R8_Params{0x42, &CPU::Register::U8::B, &CPU::Register::U8::D},
                                           LD_R8_R8_Params{0x43, &CPU::Register::U8::B, &CPU::Register::U8::E},
                                           LD_R8_R8_Params{0x44, &CPU::Register::U8::B, &CPU::Register::U8::H},
                                           LD_R8_R8_Params{0x45, &CPU::Register::U8::B, &CPU::Register::U8::L},
                                           LD_R8_R8_Params{0x47, &CPU::Register::U8::B, &CPU::Register::U8::A},

                                           LD_R8_R8_Params{0x48, &CPU::Register::U8::C, &CPU::Register::U8::B},
                                           LD_R8_R8_Params{0x49, &CPU::Register::U8::C, &CPU::Register::U8::C},
                                           LD_R8_R8_Params{0x4A, &CPU::Register::U8::C, &CPU::Register::U8::D},
                                           LD_R8_R8_Params{0x4B, &CPU::Register::U8::C, &CPU::Register::U8::E},
                                           LD_R8_R8_Params{0x4C, &CPU::Register::U8::C, &CPU::Register::U8::H},
                                           LD_R8_R8_Params{0x4D, &CPU::Register::U8::C, &CPU::Register::U8::L},
                                           LD_R8_R8_Params{0x4F, &CPU::Register::U8::C, &CPU::Register::U8::A},

                                           LD_R8_R8_Params{0x50, &CPU::Register::U8::D, &CPU::Register::U8::B},
                                           LD_R8_R8_Params{0x51, &CPU::Register::U8::D, &CPU::Register::U8::C},
                                           LD_R8_R8_Params{0x52, &CPU::Register::U8::D, &CPU::Register::U8::D},
                                           LD_R8_R8_Params{0x53, &CPU::Register::U8::D, &CPU::Register::U8::E},
                                           LD_R8_R8_Params{0x54, &CPU::Register::U8::D, &CPU::Register::U8::H},
                                           LD_R8_R8_Params{0x55, &CPU::Register::U8::D, &CPU::Register::U8::L},
                                           LD_R8_R8_Params{0x57, &CPU::Register::U8::D, &CPU::Register::U8::A},

                                           LD_R8_R8_Params{0x58, &CPU::Register::U8::E, &CPU::Register::U8::B},
                                           LD_R8_R8_Params{0x59, &CPU::Register::U8::E, &CPU::Register::U8::C},
                                           LD_R8_R8_Params{0x5A, &CPU::Register::U8::E, &CPU::Register::U8::D},
                                           LD_R8_R8_Params{0x5B, &CPU::Register::U8::E, &CPU::Register::U8::E},
                                           LD_R8_R8_Params{0x5C, &CPU::Register::U8::E, &CPU::Register::U8::H},
                                           LD_R8_R8_Params{0x5D, &CPU::Register::U8::E, &CPU::Register::U8::L},
                                           LD_R8_R8_Params{0x5F, &CPU::Register::U8::E, &CPU::Register::U8::A},

                                           LD_R8_R8_Params{0x60, &CPU::Register::U8::H, &CPU::Register::U8::B},
                                           LD_R8_R8_Params{0x61, &CPU::Register::U8::H, &CPU::Register::U8::C},
                                           LD_R8_R8_Params{0x62, &CPU::Register::U8::H, &CPU::Register::U8::D},
                                           LD_R8_R8_Params{0x63, &CPU::Register::U8::H, &CPU::Register::U8::E},
                                           LD_R8_R8_Params{0x64, &CPU::Register::U8::H, &CPU::Register::U8::H},
                                           LD_R8_R8_Params{0x65, &CPU::Register::U8::H, &CPU::Register::U8::L},
                                           LD_R8_R8_Params{0x67, &CPU::Register::U8::H, &CPU::Register::U8::A},

                                           LD_R8_R8_Params{0x68, &CPU::Register::U8::L, &CPU::Register::U8::B},
                                           LD_R8_R8_Params{0x69, &CPU::Register::U8::L, &CPU::Register::U8::C},
                                           LD_R8_R8_Params{0x6A, &CPU::Register::U8::L, &CPU::Register::U8::D},
                                           LD_R8_R8_Params{0x6B, &CPU::Register::U8::L, &CPU::Register::U8::E},
                                           LD_R8_R8_Params{0x6C, &CPU::Register::U8::L, &CPU::Register::U8::H},
                                           LD_R8_R8_Params{0x6D, &CPU::Register::U8::L, &CPU::Register::U8::L},
                                           LD_R8_R8_Params{0x6F, &CPU::Register::U8::L, &CPU::Register::U8::A},

                                           LD_R8_R8_Params{0x78, &CPU::Register::U8::A, &CPU::Register::U8::B},
                                           LD_R8_R8_Params{0x79, &CPU::Register::U8::A, &CPU::Register::U8::C},
                                           LD_R8_R8_Params{0x7A, &CPU::Register::U8::A, &CPU::Register::U8::D},
                                           LD_R8_R8_Params{0x7B, &CPU::Register::U8::A, &CPU::Register::U8::E},
                                           LD_R8_R8_Params{0x7C, &CPU::Register::U8::A, &CPU::Register::U8::H},
                                           LD_R8_R8_Params{0x7D, &CPU::Register::U8::A, &CPU::Register::U8::L},
                                           LD_R8_R8_Params{0x7F, &CPU::Register::U8::A, &CPU::Register::U8::A}

                                           ),
                         [](const testing::TestParamInfo<LD_R8_R8::ParamType>& info)
                         {
                             return std::format("LD_{:s}_{:s}", CPU::get_register8_name(info.param.dst),
                                                CPU::get_register8_name(info.param.src));
                         });

struct LD_MEM_HL_R8_Params : TestInstructionParam
{
    CPU::Register8 src;
};

class LD_MEM_HL_R8 : public CPUTesting, public ::testing::WithParamInterface<LD_MEM_HL_R8_Params>
{
};

TEST_P(LD_MEM_HL_R8, Value)
{
    const auto& param{GetParam()};
    this->cpu->reg.u16.PC = 0;
    this->cpu->reg.u16.HL = generate_address({0x1000, 0x2000});
    this->bus->write(this->cpu->reg.u16.HL, 0x42);
    this->cpu->reg.u8.*param.src = generate_byte();
    this->execute_instruction({param.opcode}, 8);

    ASSERT_EQ(this->bus->read(this->cpu->reg.u16.HL), this->cpu->reg.u8.*param.src);
}

INSTANTIATE_TEST_SUITE_P(Main, LD_MEM_HL_R8,
                         ::testing::Values(LD_MEM_HL_R8_Params{0x70, &CPU::Register::U8::B},
                                           LD_MEM_HL_R8_Params{0x71, &CPU::Register::U8::C},
                                           LD_MEM_HL_R8_Params{0x72, &CPU::Register::U8::D},
                                           LD_MEM_HL_R8_Params{0x73, &CPU::Register::U8::E},
                                           LD_MEM_HL_R8_Params{0x74, &CPU::Register::U8::H},
                                           LD_MEM_HL_R8_Params{0x75, &CPU::Register::U8::L},
                                           LD_MEM_HL_R8_Params{0x77, &CPU::Register::U8::A}),
                         [](const testing::TestParamInfo<LD_MEM_HL_R8::ParamType>& info)
                         { return std::format("LD_MEM_HL_{:s}", CPU::get_register8_name(info.param.src)); });

struct LD_R8_MEM_HL_Params : TestInstructionParam
{
    CPU::Register8 dest;
};

class LD_R8_MEM_HL : public CPUTesting, public ::testing::WithParamInterface<LD_R8_MEM_HL_Params>
{
};

TEST_P(LD_R8_MEM_HL, Value)
{
    const auto& param{GetParam()};
    const auto  byte{generate_byte()};

    this->cpu->reg.u16.PC = 0;
    this->cpu->reg.u16.HL = generate_address({0x1000, 0x2000});
    this->bus->write(this->cpu->reg.u16.HL, byte);

    this->execute_instruction({param.opcode}, 8);

    ASSERT_EQ(this->cpu->reg.u8.*param.dest, byte);
}

INSTANTIATE_TEST_SUITE_P(Main, LD_R8_MEM_HL,
                         ::testing::Values(LD_R8_MEM_HL_Params{0x46, &CPU::Register::U8::B},
                                           LD_R8_MEM_HL_Params{0x4E, &CPU::Register::U8::C},
                                           LD_R8_MEM_HL_Params{0x56, &CPU::Register::U8::D},
                                           LD_R8_MEM_HL_Params{0x5E, &CPU::Register::U8::E},
                                           LD_R8_MEM_HL_Params{0x66, &CPU::Register::U8::H},
                                           LD_R8_MEM_HL_Params{0x6E, &CPU::Register::U8::L},
                                           LD_R8_MEM_HL_Params{0x7E, &CPU::Register::U8::A}),
                         [](const testing::TestParamInfo<LD_R8_MEM_HL::ParamType>& info)
                         { return std::format("LD_{:s}_MEM_HL", CPU::get_register8_name(info.param.dest)); });

struct LD_R8_IMM8_Params : TestInstructionParam
{
    CPU::Register8 dest;
};

class LD_R8_IMM8 : public CPUTesting, public ::testing::WithParamInterface<LD_R8_IMM8_Params>
{
};

TEST_P(LD_R8_IMM8, Value)
{
    const auto& param{GetParam()};
    const auto  byte{generate_byte()};

    this->cpu->reg.u16.PC = 0;
    this->execute_instruction({param.opcode, byte}, 8);

    ASSERT_EQ(this->cpu->reg.u8.*param.dest, byte);
}

INSTANTIATE_TEST_SUITE_P(
    Main, LD_R8_IMM8,
    ::testing::Values(LD_R8_IMM8_Params{0x06, &CPU::Register::U8::B}, LD_R8_IMM8_Params{0x0E, &CPU::Register::U8::C},
                      LD_R8_IMM8_Params{0x16, &CPU::Register::U8::D}, LD_R8_IMM8_Params{0x1E, &CPU::Register::U8::E},
                      LD_R8_IMM8_Params{0x26, &CPU::Register::U8::H}, LD_R8_IMM8_Params{0x2E, &CPU::Register::U8::L},
                      LD_R8_IMM8_Params{0x3E, &CPU::Register::U8::A}),
    [](const testing::TestParamInfo<LD_R8_IMM8::ParamType>& info)
    { return std::format("LD_{:s}_IMM8", CPU::get_register8_name(info.param.dest)); });

class LD_MEM_HL_IMM8 : public CPUTesting
{
};

TEST_F(LD_MEM_HL_IMM8, Value)
{
    const auto byte{generate_byte()};

    this->cpu->reg.u16.PC = 0;
    this->cpu->reg.u16.HL = generate_address({0x1000, 0x2000});
    this->execute_instruction({0x36, byte}, 12);

    ASSERT_EQ(this->bus->read(this->cpu->reg.u16.HL), byte);
}

class LD_MEM_16_SP : public CPUTesting
{
};

TEST_F(LD_MEM_16_SP, Value)
{
    const auto addr{generate_address({0x1000, 0x2000})};

    this->cpu->reg.u16.PC = 0;
    this->cpu->reg.u16.SP = u16(generate_byte(), generate_byte());

    this->execute_instruction({0x08, u16_lsb(addr), u16_msb(addr)}, 20);

    ASSERT_EQ(this->bus->read(addr), u16_lsb(this->cpu->reg.u16.SP));
    ASSERT_EQ(this->bus->read(addr + 1), u16_msb(this->cpu->reg.u16.SP));
}

class LD_MEM_16_A : public CPUTesting
{
};

TEST_F(LD_MEM_16_A, Value)
{
    const auto addr{generate_address({0x1000, 0x2000})};

    this->cpu->reg.u16.PC = 0;
    this->cpu->reg.u8.A   = generate_byte();

    this->execute_instruction({0xEA, u16_lsb(addr), u16_msb(addr)}, 16);

    ASSERT_EQ(this->bus->read(addr), this->cpu->reg.u8.A);
}


class LD_A_MEM_16 : public CPUTesting
{
};

TEST_F(LD_A_MEM_16, Value)
{
    const auto addr{generate_address({0x1000, 0x2000})};
    const auto byte{generate_byte()};

    this->cpu->reg.u16.PC = 0;
    this->bus->write(addr, byte);

    this->execute_instruction({0xFA, u16_lsb(addr), u16_msb(addr)}, 16);

    ASSERT_EQ(this->cpu->reg.u8.A, byte);
}

struct LD_R16_IMM16_Params : TestInstructionParam
{
    CPU::Register8 dest;
    std::string    name;
};

class LD_R16_IMM16 : public CPUTesting, public ::testing::WithParamInterface<LD_R16_IMM16_Params>
{
};
