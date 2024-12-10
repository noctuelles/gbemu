#include "CPU.hxx"

#include <iomanip>
#include <sstream>

#include "utils.hxx"

const std::array<CPU::RegisterDescription<uint8_t>, 0x08> CPU::r8_lookup{{
    {"A", &CPU::B},       /* 0b000 */
    {"C", &CPU::C},       /* 0b001 */
    {"D", &CPU::D},       /* 0b010 */
    {"E", &CPU::E},       /* 0b011 */
    {"H", &CPU::H},       /* 0b100 */
    {"L", &CPU::L},       /* 0b101 */
    {"INVALID", nullptr}, /* 0b110 */
    {"A", &CPU::A},       /* 0b111 */
}};

const std::array<CPU::Instruction, 0x100> CPU::inst_lookup{{
    {},                              // 0x00
    {},                              // 0x01
    {},                              // 0x02
    {},                              // 0x03
    {},                              // 0x04
    {},                              // 0x05
    CPU::Instruction::LD_R8_IMM8(),  // 0x06
    {},                              // 0x07
    {},                              // 0x08
    {},                              // 0x09
    {},                              // 0x0A
    {},                              // 0x0B
    {},                              // 0x0C
    {},                              // 0x0D
    CPU::Instruction::LD_R8_IMM8(),  // 0x0E
    {},                              // 0x0F
    {},                              // 0x10
    {},                              // 0x11
    {},                              // 0x12
    {},                              // 0x13
    {},                              // 0x14
    {},                              // 0x15
    CPU::Instruction::LD_R8_IMM8(),  // 0x16
    {},                              // 0x17
    {},                              // 0x18
    {},                              // 0x19
    {},                              // 0x1A
    {},                              // 0x1B
    {},                              // 0x1C
    {},                              // 0x1D
    CPU::Instruction::LD_R8_IMM8(),  // 0x1E
    {},                              // 0x1F
    {},                              // 0x20
    {},                              // 0x21
    {},                              // 0x22
    {},                              // 0x23
    {},                              // 0x24
    {},                              // 0x25
    CPU::Instruction::LD_R8_IMM8(),  // 0x26
    {},                              // 0x27
    {},                              // 0x28
    {},                              // 0x29
    {},                              // 0x2A
    {},                              // 0x2B
    {},                              // 0x2C
    {},                              // 0x2D
    CPU::Instruction::LD_R8_IMM8(),  // 0x2E
    {},                              // 0x2F
    {},                              // 0x30
    {},                              // 0x31
    {},                              // 0x32
    {},                              // 0x33
    {},                              // 0x34
    {},                              // 0x35
    {},                              // 0x36
    {},                              // 0x37
    {},                              // 0x38
    {},                              // 0x39
    {},                              // 0x3A
    {},                              // 0x3B
    {},                              // 0x3C
    {},                              // 0x3D
    CPU::Instruction::LD_R8_R8(),    // 0x3E
    {},                              // 0x3F
    CPU::Instruction::LD_R8_R8(),    // 0x40
    CPU::Instruction::LD_R8_R8(),    // 0x41
    CPU::Instruction::LD_R8_R8(),    // 0x42
    CPU::Instruction::LD_R8_R8(),    // 0x43
    CPU::Instruction::LD_R8_R8(),    // 0x44
    CPU::Instruction::LD_R8_R8(),    // 0x45
    {},                              // 0x46
    CPU::Instruction::LD_R8_R8(),    // 0x47
    CPU::Instruction::LD_R8_R8(),    // 0x48
    CPU::Instruction::LD_R8_R8(),    // 0x49
    CPU::Instruction::LD_R8_R8(),    // 0x4A
    CPU::Instruction::LD_R8_R8(),    // 0x4B
    CPU::Instruction::LD_R8_R8(),    // 0x4C
    CPU::Instruction::LD_R8_R8(),    // 0x4D
    {},                              // 0x4E
    CPU::Instruction::LD_R8_R8(),    // 0x4F
    CPU::Instruction::LD_R8_R8(),    // 0x50
    CPU::Instruction::LD_R8_R8(),    // 0x51
    CPU::Instruction::LD_R8_R8(),    // 0x52
    CPU::Instruction::LD_R8_R8(),    // 0x53
    CPU::Instruction::LD_R8_R8(),    // 0x54
    CPU::Instruction::LD_R8_R8(),    // 0x55
    {},                              // 0x56
    CPU::Instruction::LD_R8_R8(),    // 0x57
    CPU::Instruction::LD_R8_R8(),    // 0x58
    CPU::Instruction::LD_R8_R8(),    // 0x59
    CPU::Instruction::LD_R8_R8(),    // 0x5A
    CPU::Instruction::LD_R8_R8(),    // 0x5B
    CPU::Instruction::LD_R8_R8(),    // 0x5C
    CPU::Instruction::LD_R8_R8(),    // 0x5D
    {},                              // 0x5E
    CPU::Instruction::LD_R8_R8(),    // 0x5F
    CPU::Instruction::LD_R8_R8(),    // 0x60
    CPU::Instruction::LD_R8_R8(),    // 0x61
    CPU::Instruction::LD_R8_R8(),    // 0x62
    CPU::Instruction::LD_R8_R8(),    // 0x63
    CPU::Instruction::LD_R8_R8(),    // 0x64
    CPU::Instruction::LD_R8_R8(),    // 0x65
    {},                              // 0x66
    CPU::Instruction::LD_R8_R8(),    // 0x67
    CPU::Instruction::LD_R8_R8(),    // 0x68
    CPU::Instruction::LD_R8_R8(),    // 0x69
    CPU::Instruction::LD_R8_R8(),    // 0x6A
    CPU::Instruction::LD_R8_R8(),    // 0x6B
    CPU::Instruction::LD_R8_R8(),    // 0x6C
    CPU::Instruction::LD_R8_R8(),    // 0x6D
    {},                              // 0x6E
    CPU::Instruction::LD_R8_R8(),    // 0x6F
    {},                              // 0x70
    {},                              // 0x71
    {},                              // 0x72
    {},                              // 0x73
    {},                              // 0x74
    {},                              // 0x75
    {},                              // 0x76
    {},                              // 0x77
    CPU::Instruction::LD_R8_R8(),    // 0x78
    CPU::Instruction::LD_R8_R8(),    // 0x79
    CPU::Instruction::LD_R8_R8(),    // 0x7A
    CPU::Instruction::LD_R8_R8(),    // 0x7B
    CPU::Instruction::LD_R8_R8(),    // 0x7C
    CPU::Instruction::LD_R8_R8(),    // 0x7D
    {},                              // 0x7E
    CPU::Instruction::LD_R8_R8(),    // 0x7F
    {},                              // 0x80
    {},                              // 0x81
    {},                              // 0x82
    {},                              // 0x83
    {},                              // 0x84
    {},                              // 0x85
    {},                              // 0x86
    {},                              // 0x87
    {},                              // 0x88
    {},                              // 0x89
    {},                              // 0x8A
    {},                              // 0x8B
    {},                              // 0x8C
    {},                              // 0x8D
    {},                              // 0x8E
    {},                              // 0x8F
    {},                              // 0x90
    {},                              // 0x91
    {},                              // 0x92
    {},                              // 0x93
    {},                              // 0x94
    {},                              // 0x95
    {},                              // 0x96
    {},                              // 0x97
    {},                              // 0x98
    {},                              // 0x99
    {},                              // 0x9A
    {},                              // 0x9B
    {},                              // 0x9C
    {},                              // 0x9D
    {},                              // 0x9E
    {},                              // 0x9F
    CPU::Instruction::AND_R8(),      // 0xA0
    CPU::Instruction::AND_R8(),      // 0xA1
    CPU::Instruction::AND_R8(),      // 0xA2
    CPU::Instruction::AND_R8(),      // 0xA3
    CPU::Instruction::AND_R8(),      // 0xA4
    CPU::Instruction::AND_R8(),      // 0xA5
    {},                              // 0xA6
    CPU::Instruction::AND_R8(),      // 0xA7
    CPU::Instruction::XOR_R8(),      // 0xA8
    CPU::Instruction::XOR_R8(),      // 0xA9
    CPU::Instruction::XOR_R8(),      // 0xAA
    CPU::Instruction::XOR_R8(),      // 0xAB
    CPU::Instruction::XOR_R8(),      // 0xAC
    CPU::Instruction::XOR_R8(),      // 0xAD
    {},                              // 0xAE
    CPU::Instruction::XOR_R8(),      // 0xAF
    CPU::Instruction::OR_R8(),       // 0xB0
    CPU::Instruction::OR_R8(),       // 0xB1
    CPU::Instruction::OR_R8(),       // 0xB2
    CPU::Instruction::OR_R8(),       // 0xB3
    CPU::Instruction::OR_R8(),       // 0xB4
    CPU::Instruction::OR_R8(),       // 0xB5
    {},                              // 0xB6
    CPU::Instruction::OR_R8(),       // 0xB7
    {},                              // 0xB8
    {},                              // 0xB9
    {},                              // 0xBA
    {},                              // 0xBB
    {},                              // 0xBC
    {},                              // 0xBD
    {},                              // 0xBE
    {},                              // 0xBF
    {},                              // 0xC0
    {},                              // 0xC1
    {},                              // 0xC2
    {},                              // 0xC3
    {},                              // 0xC4
    {},                              // 0xC5
    {},                              // 0xC6
    {},                              // 0xC7
    {},                              // 0xC8
    {},                              // 0xC9
    {},                              // 0xCA
    {},                              // 0xCB
    {},                              // 0xCC
    {},                              // 0xCD
    {},                              // 0xCE
    {},                              // 0xCF
    {},                              // 0xD0
    {},                              // 0xD1
    {},                              // 0xD2
    CPU::Instruction::ILL(),         // 0xD3
    {},                              // 0xD4
    {},                              // 0xD5
    {},                              // 0xD6
    {},                              // 0xD7
    {},                              // 0xD8
    {},                              // 0xD9
    {},                              // 0xDA
    CPU::Instruction::ILL(),         // 0xDB
    {},                              // 0xDC
    CPU::Instruction::ILL(),         // 0xDD
    {},                              // 0xDE
    {},                              // 0xDF
    {},                              // 0xE0
    {},                              // 0xE1
    {},                              // 0xE2
    CPU::Instruction::ILL(),         // 0xE3
    CPU::Instruction::ILL(),         // 0xE4
    {},                              // 0xE5
    {},                              // 0xE6
    {},                              // 0xE7
    {},                              // 0xE8
    {},                              // 0xE9
    {},                              // 0xEA
    CPU::Instruction::ILL(),         // 0xEB
    CPU::Instruction::ILL(),         // 0xEC
    CPU::Instruction::ILL(),         // 0xED
    {},                              // 0xEE
    {},                              // 0xEF
    {},                              // 0xF0
    {},                              // 0xF1
    {},                              // 0xF2
    {},                              // 0xF3
    CPU::Instruction::ILL(),         // 0xF4
    {},                              // 0xF5
    {},                              // 0xF6
    {},                              // 0xF7
    {},                              // 0xF8
    {},                              // 0xF9
    {},                              // 0xFA
    {},                              // 0xFB
    CPU::Instruction::ILL(),         // 0xFC
    CPU::Instruction::ILL(),         // 0xFD
    {},                              // 0xFE
    {},                              // 0xFF
}};

constexpr CPU::Instruction CPU::Instruction::LD_R8_R8()
{
    return {.name = "LD", .cycles = 8, .op = &CPU::LD_R8_R8};
}

constexpr CPU::Instruction CPU::Instruction::LD_R8_IMM8()
{
    return {.name = "LD", .cycles = 8, .op = &CPU::LD_R8_IMM8};
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

CPU::CPU()
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
      opcode(),
      memory(),
      inst(inst_lookup[0])
{
}

void CPU::NOP() {}

void CPU::LD_R8_R8()
{
    const auto& r_dest = r8_lookup[(this->opcode >> 3) & 0b00000111];
    const auto& r_src  = r8_lookup[this->opcode & 0b00000111];

    if (r_dest.reg == nullptr || r_src.reg == nullptr)
    {
        throw BadRegisterException();
    }

    this->build_str_inst_r8_xxx(r_dest.name, r_src.name);
    this->*r_dest.reg = this->*r_src.reg;
}

void CPU::LD_R8_IMM8()
{
    const auto& r_dest{r8_lookup[(this->opcode >> 3) & 0b00000111U]};
    const auto  r_imm{this->memory.read(this->PC++)};

    if (r_dest.reg == nullptr)
    {
        throw BadRegisterException();
    }

    this->build_str_inst_r8_xxx(r_dest.name, int_to_hex(r_imm));
    this->*r_dest.reg = r_imm;
}

void CPU::LD_R8_MEM_HL()
{
    const auto& r_dest  = r8_lookup[(this->opcode >> 3) & 0b00000111U];
    const auto  mem_val = this->memory.read(REG16_PAIR_GET(this->H, this->L));

    if (r_dest.reg == nullptr)
    {
        throw BadRegisterException();
    }

    this->build_str_inst_r8_xxx(r_dest.name, "HL");
    this->*r_dest.reg = mem_val;
}

void CPU::AND_R8()
{
    const auto& r_src = r8_lookup[this->opcode & 0b00000111];

    if (r_src.reg == nullptr)
    {
        throw BadRegisterException();
    }

    this->build_str_inst_r8(r_src.name);
    this->A &= this->*r_src.reg;
    if (this->A == 0)
    {
        this->F |= ZERO;
    }
    else
    {
        this->F &= ~ZERO;
    }
    this->F |= HALF_CARRY;
    this->F &= ~SUBTRACT;
    this->F &= ~CARRY;
}

void CPU::OR_R8()
{
    const auto& r_src = r8_lookup[this->opcode & 0b00000111];

    if (r_src.reg == nullptr)
    {
        throw BadRegisterException();
    }

    this->build_str_inst_r8(r_src.name);
    this->A |= this->*r_src.reg;
    if (this->A == 0)
    {
        this->F |= ZERO;
    }
    else
    {
        this->F &= ~ZERO;
    }
    this->F &= ~HALF_CARRY;
    this->F &= ~SUBTRACT;
    this->F &= ~CARRY;
}

void CPU::XOR_R8()
{
    const auto& r_src = r8_lookup[this->opcode & 0b00000111];

    if (r_src.reg == nullptr)
    {
        throw BadRegisterException();
    }

    this->build_str_inst_r8(r_src.name);
    this->A ^= this->*r_src.reg;
    if (this->A == 0)
    {
        this->F |= ZERO;
    }
    else
    {
        this->F &= ~ZERO;
    }
    this->F &= ~HALF_CARRY;
    this->F &= ~SUBTRACT;
    this->F &= ~CARRY;
}

void CPU::ADD_R8()
{
    const auto& r_src      = r8_lookup[this->opcode & 0b00000111];
    uint8_t     half_carry = 0;

    if (r_src.reg == nullptr)
    {
        throw BadRegisterException();
    }

    this->build_str_inst_r8(r_src.name);
    half_carry = ((this->A & 0x0F) + (this->*r_src.reg & 0x0F)) & 0x10;
    if (half_carry)
    {
        this->F |= HALF_CARRY;
    }
    else
    {
        this->F &= ~HALF_CARRY;
    }
    if (__builtin_add_overflow(this->A, this->*r_src.reg, &this->A))
    {
        this->F |= CARRY;
    }
    else
    {
        this->F &= ~CARRY;
    }
    if (this->A == 0)
    {
        this->F |= ZERO;
    }
    else
    {
        this->F &= ~ZERO;
    }
    this->F &= ~SUBTRACT;
}

void CPU::INC_R8()
{
    const auto& r_dest = r8_lookup[(this->opcode >> 3) & 0b00000111U];
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
        this->opcode = this->memory.read(this->PC);
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