//
// Created by plouvel on 12/29/24.
//

#include <format>

#include "../../../../includes/tests/hardware/core/SM83.hxx"
#include "Utils.hxx"

struct LD_R8_R8_Params : TestInstructionParam
{
    SM83::Register8 dst;
    SM83::Register8 src;
};

class LD_R8_R8 : public SM83Tester, public ::testing::WithParamInterface<LD_R8_R8_Params>
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

INSTANTIATE_TEST_SUITE_P(Main, LD_R8_R8,
                         ::testing::Values(LD_R8_R8_Params{0x40, &SM83::Register::U8::B, &SM83::Register::U8::B},
                                           LD_R8_R8_Params{0x41, &SM83::Register::U8::B, &SM83::Register::U8::C},
                                           LD_R8_R8_Params{0x42, &SM83::Register::U8::B, &SM83::Register::U8::D},
                                           LD_R8_R8_Params{0x43, &SM83::Register::U8::B, &SM83::Register::U8::E},
                                           LD_R8_R8_Params{0x44, &SM83::Register::U8::B, &SM83::Register::U8::H},
                                           LD_R8_R8_Params{0x45, &SM83::Register::U8::B, &SM83::Register::U8::L},
                                           LD_R8_R8_Params{0x47, &SM83::Register::U8::B, &SM83::Register::U8::A},

                                           LD_R8_R8_Params{0x48, &SM83::Register::U8::C, &SM83::Register::U8::B},
                                           LD_R8_R8_Params{0x49, &SM83::Register::U8::C, &SM83::Register::U8::C},
                                           LD_R8_R8_Params{0x4A, &SM83::Register::U8::C, &SM83::Register::U8::D},
                                           LD_R8_R8_Params{0x4B, &SM83::Register::U8::C, &SM83::Register::U8::E},
                                           LD_R8_R8_Params{0x4C, &SM83::Register::U8::C, &SM83::Register::U8::H},
                                           LD_R8_R8_Params{0x4D, &SM83::Register::U8::C, &SM83::Register::U8::L},
                                           LD_R8_R8_Params{0x4F, &SM83::Register::U8::C, &SM83::Register::U8::A},

                                           LD_R8_R8_Params{0x50, &SM83::Register::U8::D, &SM83::Register::U8::B},
                                           LD_R8_R8_Params{0x51, &SM83::Register::U8::D, &SM83::Register::U8::C},
                                           LD_R8_R8_Params{0x52, &SM83::Register::U8::D, &SM83::Register::U8::D},
                                           LD_R8_R8_Params{0x53, &SM83::Register::U8::D, &SM83::Register::U8::E},
                                           LD_R8_R8_Params{0x54, &SM83::Register::U8::D, &SM83::Register::U8::H},
                                           LD_R8_R8_Params{0x55, &SM83::Register::U8::D, &SM83::Register::U8::L},
                                           LD_R8_R8_Params{0x57, &SM83::Register::U8::D, &SM83::Register::U8::A},

                                           LD_R8_R8_Params{0x58, &SM83::Register::U8::E, &SM83::Register::U8::B},
                                           LD_R8_R8_Params{0x59, &SM83::Register::U8::E, &SM83::Register::U8::C},
                                           LD_R8_R8_Params{0x5A, &SM83::Register::U8::E, &SM83::Register::U8::D},
                                           LD_R8_R8_Params{0x5B, &SM83::Register::U8::E, &SM83::Register::U8::E},
                                           LD_R8_R8_Params{0x5C, &SM83::Register::U8::E, &SM83::Register::U8::H},
                                           LD_R8_R8_Params{0x5D, &SM83::Register::U8::E, &SM83::Register::U8::L},
                                           LD_R8_R8_Params{0x5F, &SM83::Register::U8::E, &SM83::Register::U8::A},

                                           LD_R8_R8_Params{0x60, &SM83::Register::U8::H, &SM83::Register::U8::B},
                                           LD_R8_R8_Params{0x61, &SM83::Register::U8::H, &SM83::Register::U8::C},
                                           LD_R8_R8_Params{0x62, &SM83::Register::U8::H, &SM83::Register::U8::D},
                                           LD_R8_R8_Params{0x63, &SM83::Register::U8::H, &SM83::Register::U8::E},
                                           LD_R8_R8_Params{0x64, &SM83::Register::U8::H, &SM83::Register::U8::H},
                                           LD_R8_R8_Params{0x65, &SM83::Register::U8::H, &SM83::Register::U8::L},
                                           LD_R8_R8_Params{0x67, &SM83::Register::U8::H, &SM83::Register::U8::A},

                                           LD_R8_R8_Params{0x68, &SM83::Register::U8::L, &SM83::Register::U8::B},
                                           LD_R8_R8_Params{0x69, &SM83::Register::U8::L, &SM83::Register::U8::C},
                                           LD_R8_R8_Params{0x6A, &SM83::Register::U8::L, &SM83::Register::U8::D},
                                           LD_R8_R8_Params{0x6B, &SM83::Register::U8::L, &SM83::Register::U8::E},
                                           LD_R8_R8_Params{0x6C, &SM83::Register::U8::L, &SM83::Register::U8::H},
                                           LD_R8_R8_Params{0x6D, &SM83::Register::U8::L, &SM83::Register::U8::L},
                                           LD_R8_R8_Params{0x6F, &SM83::Register::U8::L, &SM83::Register::U8::A},

                                           LD_R8_R8_Params{0x78, &SM83::Register::U8::A, &SM83::Register::U8::B},
                                           LD_R8_R8_Params{0x79, &SM83::Register::U8::A, &SM83::Register::U8::C},
                                           LD_R8_R8_Params{0x7A, &SM83::Register::U8::A, &SM83::Register::U8::D},
                                           LD_R8_R8_Params{0x7B, &SM83::Register::U8::A, &SM83::Register::U8::E},
                                           LD_R8_R8_Params{0x7C, &SM83::Register::U8::A, &SM83::Register::U8::H},
                                           LD_R8_R8_Params{0x7D, &SM83::Register::U8::A, &SM83::Register::U8::L},
                                           LD_R8_R8_Params{0x7F, &SM83::Register::U8::A, &SM83::Register::U8::A}

                                           ),
                         [](const testing::TestParamInfo<LD_R8_R8::ParamType>& info)
                         {
                             return std::format("LD_{:s}_{:s}", SM83::get_register8_name(info.param.dst),
                                                SM83::get_register8_name(info.param.src));
                         });

struct LD_MEM_HL_R8_Params : TestInstructionParam
{
    SM83::Register8 src;
};

class LD_MEM_HL_R8 : public SM83Tester, public ::testing::WithParamInterface<LD_MEM_HL_R8_Params>
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
                         ::testing::Values(LD_MEM_HL_R8_Params{0x70, &SM83::Register::U8::B},
                                           LD_MEM_HL_R8_Params{0x71, &SM83::Register::U8::C},
                                           LD_MEM_HL_R8_Params{0x72, &SM83::Register::U8::D},
                                           LD_MEM_HL_R8_Params{0x73, &SM83::Register::U8::E},
                                           LD_MEM_HL_R8_Params{0x74, &SM83::Register::U8::H},
                                           LD_MEM_HL_R8_Params{0x75, &SM83::Register::U8::L},
                                           LD_MEM_HL_R8_Params{0x77, &SM83::Register::U8::A}),
                         [](const testing::TestParamInfo<LD_MEM_HL_R8::ParamType>& info)
                         { return std::format("LD_MEM_HL_{:s}", SM83::get_register8_name(info.param.src)); });

struct LD_R8_MEM_HL_Params : TestInstructionParam
{
    SM83::Register8 dest;
};

class LD_R8_MEM_HL : public SM83Tester, public ::testing::WithParamInterface<LD_R8_MEM_HL_Params>
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
                         ::testing::Values(LD_R8_MEM_HL_Params{0x46, &SM83::Register::U8::B},
                                           LD_R8_MEM_HL_Params{0x4E, &SM83::Register::U8::C},
                                           LD_R8_MEM_HL_Params{0x56, &SM83::Register::U8::D},
                                           LD_R8_MEM_HL_Params{0x5E, &SM83::Register::U8::E},
                                           LD_R8_MEM_HL_Params{0x66, &SM83::Register::U8::H},
                                           LD_R8_MEM_HL_Params{0x6E, &SM83::Register::U8::L},
                                           LD_R8_MEM_HL_Params{0x7E, &SM83::Register::U8::A}),
                         [](const testing::TestParamInfo<LD_R8_MEM_HL::ParamType>& info)
                         { return std::format("LD_{:s}_MEM_HL", SM83::get_register8_name(info.param.dest)); });

struct LD_R8_IMM8_Params : TestInstructionParam
{
    SM83::Register8 dest;
};

class LD_R8_IMM8 : public SM83Tester, public ::testing::WithParamInterface<LD_R8_IMM8_Params>
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
    ::testing::Values(LD_R8_IMM8_Params{0x06, &SM83::Register::U8::B}, LD_R8_IMM8_Params{0x0E, &SM83::Register::U8::C},
                      LD_R8_IMM8_Params{0x16, &SM83::Register::U8::D}, LD_R8_IMM8_Params{0x1E, &SM83::Register::U8::E},
                      LD_R8_IMM8_Params{0x26, &SM83::Register::U8::H}, LD_R8_IMM8_Params{0x2E, &SM83::Register::U8::L},
                      LD_R8_IMM8_Params{0x3E, &SM83::Register::U8::A}),
    [](const testing::TestParamInfo<LD_R8_IMM8::ParamType>& info)
    { return std::format("LD_{:s}_IMM8", SM83::get_register8_name(info.param.dest)); });

class LD_MEM_HL_IMM8 : public SM83Tester
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

class LD_MEM_16_SP : public SM83Tester
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

class LD_MEM_16_A : public SM83Tester
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

class LD_A_MEM_16 : public SM83Tester
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
    std::string_view name;
    SM83::Register16  dest;
};

class LD_R16_IMM16 : public SM83Tester, public ::testing::WithParamInterface<LD_R16_IMM16_Params>
{
};

TEST_P(LD_R16_IMM16, Value)
{
    const auto  word{u16(generate_byte(), generate_byte())};
    const auto& param{GetParam()};

    this->cpu->reg.u16.PC = 0;

    this->execute_instruction({param.opcode, u16_lsb(word), u16_msb(word)}, 16);

    ASSERT_EQ(this->cpu->reg.u16.*param.dest, word);
};

INSTANTIATE_TEST_SUITE_P(LoadRegister16FromImmediate16, LD_R16_IMM16,
                         ::testing::Values(LD_R16_IMM16_Params{0x01, "LD_BC_IMM16", &SM83::Register::U16::BC},
                                           LD_R16_IMM16_Params{0x11, "LD_DE_IMM16", &SM83::Register::U16::DE},
                                           LD_R16_IMM16_Params{0x21, "LD_HL_IMM16", &SM83::Register::U16::HL},
                                           LD_R16_IMM16_Params{0x31, "LD_SP_IMM16", &SM83::Register::U16::SP}),
                         [](const testing::TestParamInfo<LD_R16_IMM16::ParamType>& info)
                         { return std::string(info.param.name); });

class LDH_IMM8_A : public SM83Tester {};

TEST_F(LDH_IMM8_A, Value)
{
    const auto addr_lsb{generate_byte()};
    const auto byte{generate_byte()};

    this->cpu->reg.u8.A = byte;

    this->execute_instruction({0xE0, addr_lsb}, 12);

    ASSERT_EQ(this->bus->read(u16(0xFF, addr_lsb)), this->cpu->reg.u8.A);
};

class LDH_A_IMM8 : public SM83Tester {};

TEST_F(LDH_A_IMM8, Value)
{
    const auto addr_lsb{generate_byte()};
    const auto byte{generate_byte()};

    this->bus->write(u16(0xFF, addr_lsb), byte);

    this->execute_instruction({0xF0, addr_lsb}, 12);

    ASSERT_EQ(this->cpu->reg.u8.A, this->bus->read(u16(0xFF, addr_lsb)));
};

class LDH_MEM_C_A : public SM83Tester {};

TEST_F(LDH_MEM_C_A, Value)
{
    const auto addr_lsb{generate_byte()};
    const auto byte{generate_byte()};

    this->cpu->reg.u8.C = addr_lsb;
    this->cpu->reg.u8.A = byte;

    this->execute_instruction({0xE2, addr_lsb}, 8);

    ASSERT_EQ(this->bus->read(u16(0xFF, this->cpu->reg.u8.C)), this->cpu->reg.u8.A);
}


class LDH_A_MEM_C : public SM83Tester {};

TEST_F(LDH_A_MEM_C, Value)
{
    const auto addr_lsb{generate_byte()};
    const auto byte{generate_byte()};

    this->cpu->reg.u8.C = addr_lsb;
    this->bus->write(u16(0xFF, this->cpu->reg.u8.C), byte);

    this->execute_instruction({0xE2, addr_lsb}, 8);

    ASSERT_EQ(this->cpu->reg.u8.A, this->bus->read(u16(0xFF, this->cpu->reg.u8.C)));
}
