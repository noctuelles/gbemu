//
// Created by plouvel on 12/30/24.
//

#include "tests/CPU.hxx"

struct RotateParams : TestInstructionParam
{
    std::string_view name;
    SM83::Register8   operand;
};

class RR_R8 : public CPUTesting, public ::testing::WithParamInterface<RotateParams>
{
};

class RRC_R8 : public CPUTesting, public ::testing::WithParamInterface<RotateParams>
{
};

class RL_R8 : public CPUTesting, public ::testing::WithParamInterface<RotateParams>
{
};

class RLC_R8 : public CPUTesting, public ::testing::WithParamInterface<RotateParams>
{
};

TEST_P(RR_R8, Value)
{
    const auto&       param{GetParam()};
    constexpr uint8_t byte{0b01011110};

    this->cpu->reg.u16.PC = 0;

    this->cpu->reg.u8.*param.operand = byte;
    this->cpu->reg.u8.F              = 0;

    this->execute_instruction({0xCB, param.opcode}, 8);

    ASSERT_EQ(this->cpu->reg.u8.*param.operand, 0b00101111);
    ASSERT_EQ(this->cpu->reg.u8.F, 0);

    this->execute_instruction({0xCB, param.opcode}, 8);

    ASSERT_EQ(this->cpu->reg.u8.*param.operand, 0b00010111);
    ASSERT_EQ(this->cpu->carry(), true);
    ASSERT_EQ(this->cpu->zero(), false);
    ASSERT_EQ(this->cpu->subtract(), false);
    ASSERT_EQ(this->cpu->half_carry(), false);

    this->execute_instruction({0xCB, param.opcode}, 8);

    ASSERT_EQ(this->cpu->reg.u8.*param.operand, 0b10001011);
    ASSERT_EQ(this->cpu->carry(), true);
    ASSERT_EQ(this->cpu->subtract(), false);
    ASSERT_EQ(this->cpu->half_carry(), false);
    ASSERT_EQ(this->cpu->zero(), false);

    this->execute_instruction({0xCB, param.opcode}, 8);

    ASSERT_EQ(this->cpu->reg.u8.*param.operand, 0b11000101);
    ASSERT_EQ(this->cpu->carry(), true);
    ASSERT_EQ(this->cpu->zero(), false);
    ASSERT_EQ(this->cpu->subtract(), false);
    ASSERT_EQ(this->cpu->half_carry(), false);

    this->execute_instruction({0xCB, param.opcode}, 8);

    ASSERT_EQ(this->cpu->reg.u8.*param.operand, 0b11100010);
    ASSERT_EQ(this->cpu->carry(), true);
    ASSERT_EQ(this->cpu->zero(), false);
    ASSERT_EQ(this->cpu->subtract(), false);
    ASSERT_EQ(this->cpu->half_carry(), false);

    this->execute_instruction({0xCB, param.opcode}, 8);

    ASSERT_EQ(this->cpu->reg.u8.*param.operand, 0b11110001);
    ASSERT_EQ(this->cpu->carry(), false);
    ASSERT_EQ(this->cpu->zero(), false);
    ASSERT_EQ(this->cpu->subtract(), false);
    ASSERT_EQ(this->cpu->half_carry(), false);

    this->execute_instruction({0xCB, param.opcode}, 8);

    ASSERT_EQ(this->cpu->reg.u8.*param.operand, 0b01111000);
    ASSERT_EQ(this->cpu->carry(), true);
    ASSERT_EQ(this->cpu->zero(), false);
    ASSERT_EQ(this->cpu->subtract(), false);
    ASSERT_EQ(this->cpu->half_carry(), false);
}

TEST_P(RRC_R8, Value)
{
    const auto&       param{GetParam()};
    constexpr uint8_t byte{0b01011110};

    this->cpu->reg.u16.PC = 0;

    this->cpu->reg.u8.*param.operand = byte;
    this->cpu->reg.u8.F              = 0;

    this->execute_instruction({0xCB, param.opcode}, 8);

    ASSERT_EQ(this->cpu->reg.u8.*param.operand, 0b00101111);
    ASSERT_EQ(this->cpu->reg.u8.F, 0);

    this->execute_instruction({0xCB, param.opcode}, 8);

    ASSERT_EQ(this->cpu->reg.u8.*param.operand, 0b00010111);
    ASSERT_EQ(this->cpu->carry(), true);
    ASSERT_EQ(this->cpu->zero(), false);
    ASSERT_EQ(this->cpu->subtract(), false);
    ASSERT_EQ(this->cpu->half_carry(), false);

    this->execute_instruction({0xCB, param.opcode}, 8);

    ASSERT_EQ(this->cpu->reg.u8.*param.operand, 0b00001011);
    ASSERT_EQ(this->cpu->carry(), true);
    ASSERT_EQ(this->cpu->subtract(), false);
    ASSERT_EQ(this->cpu->half_carry(), false);
    ASSERT_EQ(this->cpu->zero(), false);

    this->execute_instruction({0xCB, param.opcode}, 8);

    ASSERT_EQ(this->cpu->reg.u8.*param.operand, 0b00000101);
    ASSERT_EQ(this->cpu->carry(), true);
    ASSERT_EQ(this->cpu->zero(), false);
    ASSERT_EQ(this->cpu->subtract(), false);
    ASSERT_EQ(this->cpu->half_carry(), false);

    this->execute_instruction({0xCB, param.opcode}, 8);

    ASSERT_EQ(this->cpu->reg.u8.*param.operand, 0b00000010);
    ASSERT_EQ(this->cpu->carry(), true);
    ASSERT_EQ(this->cpu->zero(), false);
    ASSERT_EQ(this->cpu->subtract(), false);
    ASSERT_EQ(this->cpu->half_carry(), false);

    this->execute_instruction({0xCB, param.opcode}, 8);

    ASSERT_EQ(this->cpu->reg.u8.*param.operand, 0b00000001);
    ASSERT_EQ(this->cpu->carry(), false);
    ASSERT_EQ(this->cpu->zero(), false);
    ASSERT_EQ(this->cpu->subtract(), false);
    ASSERT_EQ(this->cpu->half_carry(), false);

    this->execute_instruction({0xCB, param.opcode}, 8);

    ASSERT_EQ(this->cpu->reg.u8.*param.operand, 0b00000000);
    ASSERT_EQ(this->cpu->carry(), true);
    ASSERT_EQ(this->cpu->zero(), true);
    ASSERT_EQ(this->cpu->subtract(), false);
    ASSERT_EQ(this->cpu->half_carry(), false);
}

TEST_P(RL_R8, Value)
{
    const auto&       param{GetParam()};
    constexpr uint8_t byte{0b01011110};

    this->cpu->reg.u16.PC = 0;

    this->cpu->reg.u8.*param.operand = byte;
    this->cpu->reg.u8.F              = 0;

    this->execute_instruction({0xCB, param.opcode}, 8);

    ASSERT_EQ(this->cpu->reg.u8.*param.operand, 0b10111100);
    ASSERT_EQ(this->cpu->reg.u8.F, 0);

    this->execute_instruction({0xCB, param.opcode}, 8);

    ASSERT_EQ(this->cpu->reg.u8.*param.operand, 0b01111000);
    ASSERT_EQ(this->cpu->carry(), true);
    ASSERT_EQ(this->cpu->zero(), false);
    ASSERT_EQ(this->cpu->subtract(), false);
    ASSERT_EQ(this->cpu->half_carry(), false);

    this->execute_instruction({0xCB, param.opcode}, 8);

    ASSERT_EQ(this->cpu->reg.u8.*param.operand, 0b11110001);
    ASSERT_EQ(this->cpu->carry(), false);
    ASSERT_EQ(this->cpu->subtract(), false);
    ASSERT_EQ(this->cpu->half_carry(), false);
    ASSERT_EQ(this->cpu->zero(), false);

    this->execute_instruction({0xCB, param.opcode}, 8);

    ASSERT_EQ(this->cpu->reg.u8.*param.operand, 0b11100010);
    ASSERT_EQ(this->cpu->carry(), true);
    ASSERT_EQ(this->cpu->zero(), false);
    ASSERT_EQ(this->cpu->subtract(), false);
    ASSERT_EQ(this->cpu->half_carry(), false);

    this->execute_instruction({0xCB, param.opcode}, 8);

    ASSERT_EQ(this->cpu->reg.u8.*param.operand, 0b11000101);
    ASSERT_EQ(this->cpu->carry(), true);
    ASSERT_EQ(this->cpu->zero(), false);
    ASSERT_EQ(this->cpu->subtract(), false);
    ASSERT_EQ(this->cpu->half_carry(), false);

    this->execute_instruction({0xCB, param.opcode}, 8);

    ASSERT_EQ(this->cpu->reg.u8.*param.operand, 0b10001011);
    ASSERT_EQ(this->cpu->carry(), true);
    ASSERT_EQ(this->cpu->zero(), false);
    ASSERT_EQ(this->cpu->subtract(), false);
    ASSERT_EQ(this->cpu->half_carry(), false);

    this->execute_instruction({0xCB, param.opcode}, 8);

    ASSERT_EQ(this->cpu->reg.u8.*param.operand, 0b00010111);
    ASSERT_EQ(this->cpu->carry(), true);
    ASSERT_EQ(this->cpu->zero(), false);
    ASSERT_EQ(this->cpu->subtract(), false);
    ASSERT_EQ(this->cpu->half_carry(), false);
}

TEST_P(RLC_R8, Value)
{
    const auto&       param{GetParam()};
    constexpr uint8_t byte{0b01011110};

    this->cpu->reg.u16.PC = 0;

    this->cpu->reg.u8.*param.operand = byte;
    this->cpu->reg.u8.F              = 0;

    this->execute_instruction({0xCB, param.opcode}, 8);

    ASSERT_EQ(this->cpu->reg.u8.*param.operand, 0b10111100);
    ASSERT_EQ(this->cpu->reg.u8.F, 0);

    this->execute_instruction({0xCB, param.opcode}, 8);

    ASSERT_EQ(this->cpu->reg.u8.*param.operand, 0b01111000);
    ASSERT_EQ(this->cpu->carry(), true);
    ASSERT_EQ(this->cpu->zero(), false);
    ASSERT_EQ(this->cpu->subtract(), false);
    ASSERT_EQ(this->cpu->half_carry(), false);

    this->execute_instruction({0xCB, param.opcode}, 8);

    ASSERT_EQ(this->cpu->reg.u8.*param.operand, 0b11110000);
    ASSERT_EQ(this->cpu->carry(), false);
    ASSERT_EQ(this->cpu->subtract(), false);
    ASSERT_EQ(this->cpu->half_carry(), false);
    ASSERT_EQ(this->cpu->zero(), false);

    this->execute_instruction({0xCB, param.opcode}, 8);

    ASSERT_EQ(this->cpu->reg.u8.*param.operand, 0b11100000);
    ASSERT_EQ(this->cpu->carry(), true);
    ASSERT_EQ(this->cpu->zero(), false);
    ASSERT_EQ(this->cpu->subtract(), false);
    ASSERT_EQ(this->cpu->half_carry(), false);

    this->execute_instruction({0xCB, param.opcode}, 8);

    ASSERT_EQ(this->cpu->reg.u8.*param.operand, 0b11000000);
    ASSERT_EQ(this->cpu->carry(), true);
    ASSERT_EQ(this->cpu->zero(), false);
    ASSERT_EQ(this->cpu->subtract(), false);
    ASSERT_EQ(this->cpu->half_carry(), false);

    this->execute_instruction({0xCB, param.opcode}, 8);

    ASSERT_EQ(this->cpu->reg.u8.*param.operand, 0b10000000);
    ASSERT_EQ(this->cpu->carry(), true);
    ASSERT_EQ(this->cpu->zero(), false);
    ASSERT_EQ(this->cpu->subtract(), false);
    ASSERT_EQ(this->cpu->half_carry(), false);

    this->execute_instruction({0xCB, param.opcode}, 8);

    ASSERT_EQ(this->cpu->reg.u8.*param.operand, 0b00000000);
    ASSERT_EQ(this->cpu->carry(), true);
    ASSERT_EQ(this->cpu->zero(), true);
    ASSERT_EQ(this->cpu->subtract(), false);
    ASSERT_EQ(this->cpu->half_carry(), false);
}

INSTANTIATE_TEST_SUITE_P(RotateRight, RR_R8,
                         ::testing::Values(RotateParams{0x18, "RR_B", &SM83::Register::U8::B},
                                           RotateParams{0x19, "RR_C", &SM83::Register::U8::C},
                                           RotateParams{0x1A, "RR_D", &SM83::Register::U8::D},
                                           RotateParams{0x1B, "RR_E", &SM83::Register::U8::E},
                                           RotateParams{0x1C, "RR_H", &SM83::Register::U8::H},
                                           RotateParams{0x1D, "RR_L", &SM83::Register::U8::L},
                                           RotateParams{0x1F, "RR_A", &SM83::Register::U8::A}),
                         [](const testing::TestParamInfo<RR_R8::ParamType>& info)
                         { return std::string(info.param.name); });

INSTANTIATE_TEST_SUITE_P(RotateRightCircular, RRC_R8,
                         ::testing::Values(RotateParams{0x08, "RRC_B", &SM83::Register::U8::B},
                                           RotateParams{0x09, "RRC_C", &SM83::Register::U8::C},
                                           RotateParams{0x0A, "RRC_D", &SM83::Register::U8::D},
                                           RotateParams{0x0B, "RRC_E", &SM83::Register::U8::E},
                                           RotateParams{0x0C, "RRC_H", &SM83::Register::U8::H},
                                           RotateParams{0x0D, "RRC_L", &SM83::Register::U8::L},
                                           RotateParams{0x0F, "RRC_A", &SM83::Register::U8::A}),
                         [](const testing::TestParamInfo<RRC_R8::ParamType>& info)
                         { return std::string(info.param.name); });

INSTANTIATE_TEST_SUITE_P(RotateLeft, RL_R8,
                         ::testing::Values(RotateParams{0x10, "RL_B", &SM83::Register::U8::B},
                                           RotateParams{0x11, "RL_C", &SM83::Register::U8::C},
                                           RotateParams{0x12, "RL_D", &SM83::Register::U8::D},
                                           RotateParams{0x13, "RL_E", &SM83::Register::U8::E},
                                           RotateParams{0x14, "RL_H", &SM83::Register::U8::H},
                                           RotateParams{0x15, "RL_L", &SM83::Register::U8::L},
                                           RotateParams{0x17, "RL_A", &SM83::Register::U8::A}),
                         [](const testing::TestParamInfo<RL_R8::ParamType>& info)
                         { return std::string(info.param.name); });


INSTANTIATE_TEST_SUITE_P(RotateLeftCircular, RLC_R8,
                         ::testing::Values(RotateParams{0x00, "RLC_B", &SM83::Register::U8::B},
                                           RotateParams{0x01, "RLC_C", &SM83::Register::U8::C},
                                           RotateParams{0x02, "RLC_D", &SM83::Register::U8::D},
                                           RotateParams{0x03, "RLC_E", &SM83::Register::U8::E},
                                           RotateParams{0x04, "RLC_H", &SM83::Register::U8::H},
                                           RotateParams{0x05, "RLC_L", &SM83::Register::U8::L},
                                           RotateParams{0x07, "RLC_A", &SM83::Register::U8::A}),
                         [](const testing::TestParamInfo<RL_R8::ParamType>& info)
                         { return std::string(info.param.name); });
