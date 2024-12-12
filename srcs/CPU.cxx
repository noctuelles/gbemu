#include "CPU.hxx"

#include <iomanip>
#include <sstream>

#include "utils.hxx"

const std::array<CPU::Instruction, 0x100> CPU::inst_lookup{{
    {},                                // 0x00
    CPU::Instruction::LD_R16_IMM16(),  // 0x01
    {},                                // 0x02
    {},                                // 0x03
    {},                                // 0x04
    {},                                // 0x05
    CPU::Instruction::LD_R8_IMM8(),    // 0x06
    {},                                // 0x07
    {},                                // 0x08
    {},                                // 0x09
    {},                                // 0x0A
    {},                                // 0x0B
    {},                                // 0x0C
    {},                                // 0x0D
    CPU::Instruction::LD_R8_IMM8(),    // 0x0E
    {},                                // 0x0F
    {},                                // 0x10
    CPU::Instruction::LD_R16_IMM16(),  // 0x11
    {},                                // 0x12
    {},                                // 0x13
    {},                                // 0x14
    {},                                // 0x15
    CPU::Instruction::LD_R8_IMM8(),    // 0x16
    {},                                // 0x17
    {},                                // 0x18
    {},                                // 0x19
    {},                                // 0x1A
    {},                                // 0x1B
    {},                                // 0x1C
    {},                                // 0x1D
    CPU::Instruction::LD_R8_IMM8(),    // 0x1E
    {},                                // 0x1F
    {},                                // 0x20
    CPU::Instruction::LD_R16_IMM16(),  // 0x21
    {},                                // 0x22
    {},                                // 0x23
    {},                                // 0x24
    {},                                // 0x25
    CPU::Instruction::LD_R8_IMM8(),    // 0x26
    {},                                // 0x27
    {},                                // 0x28
    {},                                // 0x29
    {},                                // 0x2A
    {},                                // 0x2B
    {},                                // 0x2C
    {},                                // 0x2D
    CPU::Instruction::LD_R8_IMM8(),    // 0x2E
    {},                                // 0x2F
    {},                                // 0x30
    CPU::Instruction::LD_R16_IMM16(),  // 0x31
    {},                                // 0x32
    {},                                // 0x33
    {},                                // 0x34
    {},                                // 0x35
    {},                                // 0x36
    {},                                // 0x37
    {},                                // 0x38
    {},                                // 0x39
    {},                                // 0x3A
    {},                                // 0x3B
    {},                                // 0x3C
    {},                                // 0x3D
    CPU::Instruction::LD_R8_R8(),      // 0x3E
    {},                                // 0x3F
    CPU::Instruction::LD_R8_R8(),      // 0x40
    CPU::Instruction::LD_R8_R8(),      // 0x41
    CPU::Instruction::LD_R8_R8(),      // 0x42
    CPU::Instruction::LD_R8_R8(),      // 0x43
    CPU::Instruction::LD_R8_R8(),      // 0x44
    CPU::Instruction::LD_R8_R8(),      // 0x45
    CPU::Instruction::LD_R8_MEM_HL(),  // 0x46
    CPU::Instruction::LD_R8_R8(),      // 0x47
    CPU::Instruction::LD_R8_R8(),      // 0x48
    CPU::Instruction::LD_R8_R8(),      // 0x49
    CPU::Instruction::LD_R8_R8(),      // 0x4A
    CPU::Instruction::LD_R8_R8(),      // 0x4B
    CPU::Instruction::LD_R8_R8(),      // 0x4C
    CPU::Instruction::LD_R8_R8(),      // 0x4D
    CPU::Instruction::LD_R8_MEM_HL(),  // 0x4E
    CPU::Instruction::LD_R8_R8(),      // 0x4F
    CPU::Instruction::LD_R8_R8(),      // 0x50
    CPU::Instruction::LD_R8_R8(),      // 0x51
    CPU::Instruction::LD_R8_R8(),      // 0x52
    CPU::Instruction::LD_R8_R8(),      // 0x53
    CPU::Instruction::LD_R8_R8(),      // 0x54
    CPU::Instruction::LD_R8_R8(),      // 0x55
    CPU::Instruction::LD_R8_MEM_HL(),  // 0x56
    CPU::Instruction::LD_R8_R8(),      // 0x57
    CPU::Instruction::LD_R8_R8(),      // 0x58
    CPU::Instruction::LD_R8_R8(),      // 0x59
    CPU::Instruction::LD_R8_R8(),      // 0x5A
    CPU::Instruction::LD_R8_R8(),      // 0x5B
    CPU::Instruction::LD_R8_R8(),      // 0x5C
    CPU::Instruction::LD_R8_R8(),      // 0x5D
    CPU::Instruction::LD_R8_MEM_HL(),  // 0x5E
    CPU::Instruction::LD_R8_R8(),      // 0x5F
    CPU::Instruction::LD_R8_R8(),      // 0x60
    CPU::Instruction::LD_R8_R8(),      // 0x61
    CPU::Instruction::LD_R8_R8(),      // 0x62
    CPU::Instruction::LD_R8_R8(),      // 0x63
    CPU::Instruction::LD_R8_R8(),      // 0x64
    CPU::Instruction::LD_R8_R8(),      // 0x65
    CPU::Instruction::LD_R8_MEM_HL(),  // 0x66
    CPU::Instruction::LD_R8_R8(),      // 0x67
    CPU::Instruction::LD_R8_R8(),      // 0x68
    CPU::Instruction::LD_R8_R8(),      // 0x69
    CPU::Instruction::LD_R8_R8(),      // 0x6A
    CPU::Instruction::LD_R8_R8(),      // 0x6B
    CPU::Instruction::LD_R8_R8(),      // 0x6C
    CPU::Instruction::LD_R8_R8(),      // 0x6D
    CPU::Instruction::LD_R8_MEM_HL(),  // 0x6E
    CPU::Instruction::LD_R8_R8(),      // 0x6F
    CPU::Instruction::LD_MEM_HL_R8(),  // 0x70
    CPU::Instruction::LD_MEM_HL_R8(),  // 0x71
    CPU::Instruction::LD_MEM_HL_R8(),  // 0x72
    CPU::Instruction::LD_MEM_HL_R8(),  // 0x73
    CPU::Instruction::LD_MEM_HL_R8(),  // 0x74
    CPU::Instruction::LD_MEM_HL_R8(),  // 0x75
    {},                                // 0x76
    CPU::Instruction::LD_MEM_HL_R8(),  // 0x77
    CPU::Instruction::LD_R8_R8(),      // 0x78
    CPU::Instruction::LD_R8_R8(),      // 0x79
    CPU::Instruction::LD_R8_R8(),      // 0x7A
    CPU::Instruction::LD_R8_R8(),      // 0x7B
    CPU::Instruction::LD_R8_R8(),      // 0x7C
    CPU::Instruction::LD_R8_R8(),      // 0x7D
    CPU::Instruction::LD_R8_MEM_HL(),  // 0x7E
    CPU::Instruction::LD_R8_R8(),      // 0x7F
    {},                                // 0x80
    {},                                // 0x81
    {},                                // 0x82
    {},                                // 0x83
    {},                                // 0x84
    {},                                // 0x85
    {},                                // 0x86
    {},                                // 0x87
    {},                                // 0x88
    {},                                // 0x89
    {},                                // 0x8A
    {},                                // 0x8B
    {},                                // 0x8C
    {},                                // 0x8D
    {},                                // 0x8E
    {},                                // 0x8F
    {},                                // 0x90
    {},                                // 0x91
    {},                                // 0x92
    {},                                // 0x93
    {},                                // 0x94
    {},                                // 0x95
    {},                                // 0x96
    {},                                // 0x97
    {},                                // 0x98
    {},                                // 0x99
    {},                                // 0x9A
    {},                                // 0x9B
    {},                                // 0x9C
    {},                                // 0x9D
    {},                                // 0x9E
    {},                                // 0x9F
    CPU::Instruction::AND_R8(),        // 0xA0
    CPU::Instruction::AND_R8(),        // 0xA1
    CPU::Instruction::AND_R8(),        // 0xA2
    CPU::Instruction::AND_R8(),        // 0xA3
    CPU::Instruction::AND_R8(),        // 0xA4
    CPU::Instruction::AND_R8(),        // 0xA5
    {},                                // 0xA6
    CPU::Instruction::AND_R8(),        // 0xA7
    CPU::Instruction::XOR_R8(),        // 0xA8
    CPU::Instruction::XOR_R8(),        // 0xA9
    CPU::Instruction::XOR_R8(),        // 0xAA
    CPU::Instruction::XOR_R8(),        // 0xAB
    CPU::Instruction::XOR_R8(),        // 0xAC
    CPU::Instruction::XOR_R8(),        // 0xAD
    {},                                // 0xAE
    CPU::Instruction::XOR_R8(),        // 0xAF
    CPU::Instruction::OR_R8(),         // 0xB0
    CPU::Instruction::OR_R8(),         // 0xB1
    CPU::Instruction::OR_R8(),         // 0xB2
    CPU::Instruction::OR_R8(),         // 0xB3
    CPU::Instruction::OR_R8(),         // 0xB4
    CPU::Instruction::OR_R8(),         // 0xB5
    {},                                // 0xB6
    CPU::Instruction::OR_R8(),         // 0xB7
    {},                                // 0xB8
    {},                                // 0xB9
    {},                                // 0xBA
    {},                                // 0xBB
    {},                                // 0xBC
    {},                                // 0xBD
    {},                                // 0xBE
    {},                                // 0xBF
    {},                                // 0xC0
    {},                                // 0xC1
    {},                                // 0xC2
    {},                                // 0xC3
    {},                                // 0xC4
    {},                                // 0xC5
    {},                                // 0xC6
    {},                                // 0xC7
    {},                                // 0xC8
    {},                                // 0xC9
    {},                                // 0xCA
    {},                                // 0xCB
    {},                                // 0xCC
    {},                                // 0xCD
    {},                                // 0xCE
    {},                                // 0xCF
    {},                                // 0xD0
    {},                                // 0xD1
    {},                                // 0xD2
    CPU::Instruction::ILL(),           // 0xD3
    {},                                // 0xD4
    {},                                // 0xD5
    {},                                // 0xD6
    {},                                // 0xD7
    {},                                // 0xD8
    {},                                // 0xD9
    {},                                // 0xDA
    CPU::Instruction::ILL(),           // 0xDB
    {},                                // 0xDC
    CPU::Instruction::ILL(),           // 0xDD
    {},                                // 0xDE
    {},                                // 0xDF
    {},                                // 0xE0
    {},                                // 0xE1
    {},                                // 0xE2
    CPU::Instruction::ILL(),           // 0xE3
    CPU::Instruction::ILL(),           // 0xE4
    {},                                // 0xE5
    {},                                // 0xE6
    {},                                // 0xE7
    {},                                // 0xE8
    {},                                // 0xE9
    {},                                // 0xEA
    CPU::Instruction::ILL(),           // 0xEB
    CPU::Instruction::ILL(),           // 0xEC
    CPU::Instruction::ILL(),           // 0xED
    {},                                // 0xEE
    {},                                // 0xEF
    {},                                // 0xF0
    {},                                // 0xF1
    {},                                // 0xF2
    {},                                // 0xF3
    CPU::Instruction::ILL(),           // 0xF4
    {},                                // 0xF5
    {},                                // 0xF6
    {},                                // 0xF7
    {},                                // 0xF8
    {},                                // 0xF9
    {},                                // 0xFA
    {},                                // 0xFB
    CPU::Instruction::ILL(),           // 0xFC
    CPU::Instruction::ILL(),           // 0xFD
    {},                                // 0xFE
    {},                                // 0xFF
}};

constexpr CPU::Instruction CPU::Instruction::LD_R8_R8()
{
    return {.name = "LD", .cycles = 8, .op = &CPU::LD_R8_R8};
}

constexpr CPU::Instruction CPU::Instruction::LD_R8_IMM8()
{
    return {.name = "LD", .cycles = 8, .op = &CPU::LD_R8_IMM8};
}

constexpr CPU::Instruction CPU::Instruction::LD_R16_IMM16()
{
    return {.name = "LD", .cycles = 12, .op = &CPU::LD_R16_IMM16};
}

constexpr CPU::Instruction CPU::Instruction::LD_R8_MEM_HL()
{
    return {.name = "LD", .cycles = 8, .op = &CPU::LD_R8_MEM_HL};
}

constexpr CPU::Instruction CPU::Instruction::LD_MEM_HL_R8()
{
    return {.name = "LD", .cycles = 8, .op = &CPU::LD_MEM_HL_R8};
}

constexpr CPU::Instruction CPU::Instruction::AND_R8()
{
    return {.name = "AND", .cycles = 4, .op = &CPU::AND_R8};
}

constexpr CPU::Instruction CPU::Instruction::XOR_R8()
{
    return {.name = "XOR", .cycles = 4, .op = &CPU::XOR_R8};
}

constexpr CPU::Instruction CPU::Instruction::OR_R8()
{
    return {.name = "OR", .cycles = 4, .op = &CPU::OR_R8};
}

constexpr CPU::Instruction CPU::Instruction::ILL()
{
    return {.name = "???", .cycles = 0, .op = &CPU::ILL};
}

CPU::CPU(Bus& bus)
    : A(0),
      F(0),
      B(0),
      C(0),
      D(0),
      E(0),
      H(0),
      L(0),
      PC(0),
      SP(0),
      cycles(0),
      opcode(0),
      inst(inst_lookup[0]),
      bus(bus)
{
}

CPU::~CPU() = default;

void CPU::NOP() {}

void CPU::LD_R8_R8()
{
    const auto r_dest = static_cast<Register8>((this->opcode >> 3) & 0b00000111);
    const auto r_src  = static_cast<uint8_t>(this->opcode & 0b00000111);

    this->set_register_r8_imm8(r_dest, r_src);
}

void CPU::LD_R8_IMM8()
{
    const auto r_dest = static_cast<Register8>((this->opcode >> 3) & 0b00000111);
    const auto r_imm{this->bus.read(this->PC++)};

    this->set_register_r8_imm8(r_dest, r_imm);
}

void CPU::LD_R16_IMM16()
{
    const auto r_dest = static_cast<Register16>((this->opcode >> 4) & 0b00000011U);
    const auto imm_lsb{this->bus.read(this->PC++)};
    const auto imm_msb{this->bus.read(this->PC++)};
    const auto imm_val = static_cast<uint16_t>(imm_lsb << 8 | imm_msb);

    this->set_register_r16_imm16(r_dest, imm_val);
}

void CPU::LD_R8_MEM_HL()
{
    const auto r_dest  = static_cast<Register8>((this->opcode >> 3) & 0b00000111);
    const auto mem_val = this->bus.read(REG16_PAIR_GET(this->H, this->L));

    this->set_register_r8_imm8(r_dest, mem_val);
}

void CPU::LD_MEM_HL_R8() const
{
    const auto r_dest = get_register8(static_cast<Register8>(this->opcode & 0b00000111));
    this->bus.write(REG16_PAIR_GET(this->H, this->L), this->*r_dest);
}

void CPU::AND_R8()
{
    const auto r_src = get_register8(static_cast<Register8>(this->opcode & 0b00000111));

    this->A &= this->*r_src;
    if (this->A == 0)
    {
        this->F |= Flags::ZERO;
    }
    else
    {
        this->F &= ~Flags::ZERO;
    }
    this->F |= Flags::HALF_CARRY;
    this->F &= ~Flags::SUBTRACT;
    this->F &= ~Flags::CARRY;
}

void CPU::OR_R8()
{
    const auto r_src = get_register8(static_cast<Register8>(this->opcode & 0b00000111));

    this->A |= this->*r_src;
    if (this->A == 0)
    {
        this->F |= Flags::ZERO;
    }
    else
    {
        this->F &= ~Flags::ZERO;
    }
    this->F &= ~Flags::HALF_CARRY;
    this->F &= ~Flags::SUBTRACT;
    this->F &= ~Flags::CARRY;
}

void CPU::XOR_R8()
{
    const auto r_src = get_register8(static_cast<Register8>(this->opcode & 0b00000111));

    this->A ^= this->*r_src;
    if (this->A == 0)
    {
        this->F |= Flags::ZERO;
    }
    else
    {
        this->F &= ~Flags::ZERO;
    }
    this->F &= ~Flags::HALF_CARRY;
    this->F &= ~Flags::SUBTRACT;
    this->F &= ~Flags::CARRY;
}

void CPU::ADD_R8()
{
    const auto r_src      = get_register8(static_cast<Register8>(this->opcode & 0b00000111));
    uint8_t    half_carry = 0;

    half_carry = ((this->A & 0x0F) + (this->*r_src & 0x0F)) & 0x10;
    if (half_carry)
    {
        this->F |= Flags::HALF_CARRY;
    }
    else
    {
        this->F &= ~Flags::HALF_CARRY;
    }
    if (__builtin_add_overflow(this->A, this->*r_src, &this->A))
    {
        this->F |= Flags::CARRY;
    }
    else
    {
        this->F &= ~Flags::CARRY;
    }
    if (this->A == 0)
    {
        this->F |= Flags::ZERO;
    }
    else
    {
        this->F &= ~Flags::ZERO;
    }
    this->F &= ~Flags::SUBTRACT;
}

void CPU::INC_R8()
{
    const auto r_dest = get_register8(static_cast<Register8>((this->opcode >> 4) & 0b00000011U));
    this->*r_dest += 1;
}

void CPU::ILL()
{
    throw IllegalInstructionException();
}

void CPU::cycle()
{
    if (this->cycles == 0)
    {
        // TODO: CB Prefix
        this->opcode = this->bus.read(this->PC);
        this->inst   = inst_lookup.at(opcode);
        this->cycles = this->inst.cycles;
        this->PC++;
        (this->*inst.op)();
    }
    else
    {
        this->cycles--;
    }
}
const std::string& CPU::get_str_inst() const
{
    return this->str_inst;
}

void CPU::build_str_inst_r8(const std::string& reg)
{
    this->str_inst = this->inst.name + " " + reg;
}

void CPU::build_str_inst_r8_xxx(const std::string& reg_dest, const std::string& val)
{
    this->str_inst = this->inst.name + " " + reg_dest + ", " + val;
}

void CPU::set_register_r16_imm16(const Register16 reg, const uint16_t value)
{
    switch (reg)
    {
        case Register16::R16_BC:
            this->B = value & 0xFF;
            this->C = value >> 8;
            break;
        case Register16::R16_DE:
            this->D = value & 0xFF;
            this->E = value >> 8;
            break;
        case Register16::R16_HL:
            this->H = value & 0xFF;
            this->L = value >> 8;
            break;
        case Register16::R16_SP:
            this->SP = value;
            break;
        default:
            throw BadRegisterException();
    }
}

void CPU::set_register_r8_imm8(const Register8 reg, const uint8_t value)
{
    const auto r_dest = get_register8(reg);
    this->*r_dest     = value;
}

void CPU::set_register_r8_r8(const Register8 reg_dst, const Register8 reg_src)
{
    const auto r_dest = get_register8(reg_dst);
    const auto r_src  = get_register8(reg_src);

    this->*r_dest = this->*r_src;
}

CPU::Reg CPU::get_register8(Register8 reg)
{
    switch (reg)
    {
        case Register8::R8_A:
            return &CPU::A;
        case Register8::R8_B:
            return &CPU::B;
        case Register8::R8_C:
            return &CPU::C;
        case Register8::R8_D:
            return &CPU::D;
        case Register8::R8_E:
            return &CPU::E;
        case Register8::R8_H:
            return &CPU::H;
        case Register8::R8_L:
            return &CPU::L;
        default:
            throw BadRegisterException();
    }
}
