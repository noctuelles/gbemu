#include "CPU.hxx"

const std::array<CPU::RegisterDescription<uint8_t>, 0x08> CPU::r8_lookup = {
    {
        {"A",&CPU::B}, /* 0b000 */
        {"C",&CPU::C}, /* 0b001 */
        {"D", &CPU::D}, /* 0b010 */
        {"E", &CPU::E}, /* 0b011 */
        {"H", &CPU::H}, /* 0b100 */
        {"L", &CPU::L}, /* 0b101 */
        {"INVALID", nullptr}, /* 0b110 */
        {"A", &CPU::A}, /* 0b111 */
    }};

const std::array<CPU::Instruction, 0x100> CPU::inst_lookup = {
{
    {}, // 0x00
    {}, // 0x01
    {}, // 0x02
    {}, // 0x03
    {}, // 0x04
    {}, // 0x05
    {.name = "LD", .cycles = 8, .op = &CPU::LD_R8_IMM8}, // 0x06
    {}, // 0x07
    {}, // 0x08
    {}, // 0x09
    {}, // 0x0A
    {}, // 0x0B
    {}, // 0x0C
    {}, // 0x0D
    {.name = "LD", .cycles = 8, .op = &CPU::LD_R8_IMM8}, // 0x0E
    {}, // 0x0F
    {}, // 0x10
    {}, // 0x11
    {}, // 0x12
    {}, // 0x13
    {}, // 0x14
    {}, // 0x15
    {.name = "LD", .cycles = 8, .op = &CPU::LD_R8_IMM8}, // 0x16
    {}, // 0x17
    {}, // 0x18
    {}, // 0x19
    {}, // 0x1A
    {}, // 0x1B
    {}, // 0x1C
    {}, // 0x1D
    {.name = "LD", .cycles = 8, .op = &CPU::LD_R8_IMM8}, // 0x1E
    {}, // 0x1F
    {}, // 0x20
    {}, // 0x21
    {}, // 0x22
    {}, // 0x23
    {}, // 0x24
    {}, // 0x25
    {.name = "LD", .cycles = 8, .op = &CPU::LD_R8_IMM8}, // 0x26
    {}, // 0x27
    {}, // 0x28
    {}, // 0x29
    {}, // 0x2A
    {}, // 0x2B
    {}, // 0x2C
    {}, // 0x2D
    {.name = "LD", .cycles = 8, .op = &CPU::LD_R8_IMM8}, // 0x2E
    {}, // 0x2F
    {}, // 0x30
    {}, // 0x31
    {}, // 0x32
    {}, // 0x33
    {}, // 0x34
    {}, // 0x35
    {}, // 0x36
    {}, // 0x37
    {}, // 0x38
    {}, // 0x39
    {}, // 0x3A
    {}, // 0x3B
    {}, // 0x3C
    {}, // 0x3D
    {.name = "LD", .cycles = 8, .op = &CPU::LD_R8_IMM8}, // 0x3E
    {}, // 0x3F
    {.name = "LD", .cycles = 4, .op = &CPU::LD_R8_R8}, // 0x40
    {.name = "LD", .cycles = 4, .op = &CPU::LD_R8_R8}, // 0x41
    {.name = "LD", .cycles = 4, .op = &CPU::LD_R8_R8}, // 0x42
    {.name = "LD", .cycles = 4, .op = &CPU::LD_R8_R8}, // 0x43
    {.name = "LD", .cycles = 4, .op = &CPU::LD_R8_R8}, // 0x44
    {.name = "LD", .cycles = 4, .op = &CPU::LD_R8_R8}, // 0x45
    {}, // 0x46
    {.name = "LD", .cycles = 4, .op = &CPU::LD_R8_R8}, // 0x47
    {.name = "LD", .cycles = 4, .op = &CPU::LD_R8_R8}, // 0x48
    {.name = "LD", .cycles = 4, .op = &CPU::LD_R8_R8}, // 0x49
    {.name = "LD", .cycles = 4, .op = &CPU::LD_R8_R8}, // 0x4A
    {.name = "LD", .cycles = 4, .op = &CPU::LD_R8_R8}, // 0x4B
    {.name = "LD", .cycles = 4, .op = &CPU::LD_R8_R8}, // 0x4C
    {.name = "LD", .cycles = 4, .op = &CPU::LD_R8_R8}, // 0x4D
    {}, // 0x4E
    {.name = "LD", .cycles = 4, .op = &CPU::LD_R8_R8}, // 0x4F
    {.name = "LD", .cycles = 4, .op = &CPU::LD_R8_R8}, // 0x50
    {.name = "LD", .cycles = 4, .op = &CPU::LD_R8_R8}, // 0x51
    {.name = "LD", .cycles = 4, .op = &CPU::LD_R8_R8}, // 0x52
    {.name = "LD", .cycles = 4, .op = &CPU::LD_R8_R8}, // 0x53
    {.name = "LD", .cycles = 4, .op = &CPU::LD_R8_R8}, // 0x54
    {.name = "LD", .cycles = 4, .op = &CPU::LD_R8_R8}, // 0x55
    {}, // 0x56
    {.name = "LD", .cycles = 4, .op = &CPU::LD_R8_R8}, // 0x57
    {.name = "LD", .cycles = 4, .op = &CPU::LD_R8_R8}, // 0x58
    {.name = "LD", .cycles = 4, .op = &CPU::LD_R8_R8}, // 0x59
    {.name = "LD", .cycles = 4, .op = &CPU::LD_R8_R8}, // 0x5A
    {.name = "LD", .cycles = 4, .op = &CPU::LD_R8_R8}, // 0x5B
    {.name = "LD", .cycles = 4, .op = &CPU::LD_R8_R8}, // 0x5C
    {.name = "LD", .cycles = 4, .op = &CPU::LD_R8_R8}, // 0x5D
    {}, // 0x5E
    {.name = "LD", .cycles = 4, .op = &CPU::LD_R8_R8}, // 0x5F
    {.name = "LD", .cycles = 4, .op = &CPU::LD_R8_R8}, // 0x60
    {.name = "LD", .cycles = 4, .op = &CPU::LD_R8_R8}, // 0x61
    {.name = "LD", .cycles = 4, .op = &CPU::LD_R8_R8}, // 0x62
    {.name = "LD", .cycles = 4, .op = &CPU::LD_R8_R8}, // 0x63
    {.name = "LD", .cycles = 4, .op = &CPU::LD_R8_R8}, // 0x64
    {.name = "LD", .cycles = 4, .op = &CPU::LD_R8_R8}, // 0x65
    {}, // 0x66
    {.name = "LD", .cycles = 4, .op = &CPU::LD_R8_R8}, // 0x67
    {.name = "LD", .cycles = 4, .op = &CPU::LD_R8_R8}, // 0x68
    {.name = "LD", .cycles = 4, .op = &CPU::LD_R8_R8}, // 0x69
    {.name = "LD", .cycles = 4, .op = &CPU::LD_R8_R8}, // 0x6A
    {.name = "LD", .cycles = 4, .op = &CPU::LD_R8_R8}, // 0x6B
    {.name = "LD", .cycles = 4, .op = &CPU::LD_R8_R8}, // 0x6C
    {.name = "LD", .cycles = 4, .op = &CPU::LD_R8_R8}, // 0x6D
    {}, // 0x6E
    {.name = "LD", .cycles = 4, .op = &CPU::LD_R8_R8}, // 0x6F
    {}, // 0x70
    {}, // 0x71
    {}, // 0x72
    {}, // 0x73
    {}, // 0x74
    {}, // 0x75
    {}, // 0x76
    {}, // 0x77
    {.name = "LD", .cycles = 4, .op = &CPU::LD_R8_R8}, // 0x78
    {.name = "LD", .cycles = 4, .op = &CPU::LD_R8_R8}, // 0x79
    {.name = "LD", .cycles = 4, .op = &CPU::LD_R8_R8}, // 0x7A
    {.name = "LD", .cycles = 4, .op = &CPU::LD_R8_R8}, // 0x7B
    {.name = "LD", .cycles = 4, .op = &CPU::LD_R8_R8}, // 0x7C
    {.name = "LD", .cycles = 4, .op = &CPU::LD_R8_R8}, // 0x7D
    {}, // 0x7E
    {.name = "LD", .cycles = 4, .op = &CPU::LD_R8_R8}, // 0x7F
    {}, // 0x80
    {}, // 0x81
    {}, // 0x82
    {}, // 0x83
    {}, // 0x84
    {}, // 0x85
    {}, // 0x86
    {}, // 0x87
    {}, // 0x88
    {}, // 0x89
    {}, // 0x8A
    {}, // 0x8B
    {}, // 0x8C
    {}, // 0x8D
    {}, // 0x8E
    {}, // 0x8F
    {}, // 0x90
    {}, // 0x91
    {}, // 0x92
    {}, // 0x93
    {}, // 0x94
    {}, // 0x95
    {}, // 0x96
    {}, // 0x97
    {}, // 0x98
    {}, // 0x99
    {}, // 0x9A
    {}, // 0x9B
    {}, // 0x9C
    {}, // 0x9D
    {}, // 0x9E
    {}, // 0x9F
    {.name = "AND", .cycles = 4, .op = &CPU::AND_R8}, // 0xA0
    {.name = "AND", .cycles = 4, .op = &CPU::AND_R8}, // 0xA1
    {.name = "AND", .cycles = 4, .op = &CPU::AND_R8}, // 0xA2
    {.name = "AND", .cycles = 4, .op = &CPU::AND_R8}, // 0xA3
    {.name = "AND", .cycles = 4, .op = &CPU::AND_R8}, // 0xA4
    {.name = "AND", .cycles = 4, .op = &CPU::AND_R8}, // 0xA5
    {}, // 0xA6
    {.name = "AND", .cycles = 4, .op = &CPU::AND_R8}, // 0xA7
    {.name = "XOR", .cycles = 4, .op = &CPU::XOR_R8}, // 0xA8
    {.name = "XOR", .cycles = 4, .op = &CPU::XOR_R8}, // 0xA9
    {.name = "XOR", .cycles = 4, .op = &CPU::XOR_R8}, // 0xAA
    {.name = "XOR", .cycles = 4, .op = &CPU::XOR_R8}, // 0xAB
    {.name = "XOR", .cycles = 4, .op = &CPU::XOR_R8}, // 0xAC
    {.name = "XOR", .cycles = 4, .op = &CPU::XOR_R8}, // 0xAD
    {}, // 0xAE
    {.name = "XOR", .cycles = 4, .op = &CPU::XOR_R8}, // 0xAF
    {.name = "OR", .cycles = 4, .op = &CPU::OR_R8}, // 0xB0
    {.name = "OR", .cycles = 4, .op = &CPU::OR_R8}, // 0xB1
    {.name = "OR", .cycles = 4, .op = &CPU::OR_R8}, // 0xB2
    {.name = "OR", .cycles = 4, .op = &CPU::OR_R8}, // 0xB3
    {.name = "OR", .cycles = 4, .op = &CPU::OR_R8}, // 0xB4
    {.name = "OR", .cycles = 4, .op = &CPU::OR_R8}, // 0xB5
    {}, // 0xB6
    {.name = "OR", .cycles = 4, .op = &CPU::OR_R8}, // 0xB7
    {}, // 0xB8
    {}, // 0xB9
    {}, // 0xBA
    {}, // 0xBB
    {}, // 0xBC
    {}, // 0xBD
    {}, // 0xBE
    {}, // 0xBF
    {}, // 0xC0
    {}, // 0xC1
    {}, // 0xC2
    {}, // 0xC3
    {}, // 0xC4
    {}, // 0xC5
    {}, // 0xC6
    {}, // 0xC7
    {}, // 0xC8
    {}, // 0xC9
    {}, // 0xCA
    {}, // 0xCB
    {}, // 0xCC
    {}, // 0xCD
    {}, // 0xCE
    {}, // 0xCF
    {}, // 0xD0
    {}, // 0xD1
    {}, // 0xD2
    {.name = "???", .cycles = 0, .op = &CPU::ILL}, // 0xD3
    {}, // 0xD4
    {}, // 0xD5
    {}, // 0xD6
    {}, // 0xD7
    {}, // 0xD8
    {}, // 0xD9
    {}, // 0xDA
    {.name = "???", .cycles = 0, .op = &CPU::ILL}, // 0xDB
    {}, // 0xDC
    {.name = "???", .cycles = 0, .op = &CPU::ILL}, // 0xDD
    {}, // 0xDE
    {}, // 0xDF
    {}, // 0xE0
    {}, // 0xE1
    {}, // 0xE2
    {.name = "???", .cycles = 0, .op = &CPU::ILL}, // 0xE3
    {.name = "???", .cycles = 0, .op = &CPU::ILL}, // 0xE4
    {}, // 0xE5
    {}, // 0xE6
    {}, // 0xE7
    {}, // 0xE8
    {}, // 0xE9
    {}, // 0xEA
    {.name = "???", .cycles = 0, .op = &CPU::ILL}, // 0xEB
    {.name = "???", .cycles = 0, .op = &CPU::ILL}, // 0xEC
    {.name = "???", .cycles = 0, .op = &CPU::ILL}, // 0xED
    {}, // 0xEE
    {}, // 0xEF
    {}, // 0xF0
    {}, // 0xF1
    {}, // 0xF2
    {}, // 0xF3
    {.name = "???", .cycles = 0, .op = &CPU::ILL}, // 0xF4
    {}, // 0xF5
    {}, // 0xF6
    {}, // 0xF7
    {}, // 0xF8
    {}, // 0xF9
    {}, // 0xFA
    {}, // 0xFB
    {.name = "???", .cycles = 0, .op = &CPU::ILL}, // 0xFC
    {.name = "???", .cycles = 0, .op = &CPU::ILL}, // 0xFD
    {}, // 0xFE
    {}, // 0xFF
}};

CPU::CPU()
    : A(0), F(0), B(0), C(0), D(0), E(0), H(0), L(0), PC(0), SP(0), cycles(0), opcode(), memory(), inst(inst_lookup[0])
{
}

void CPU::NOP()
{
}

void CPU::LD_R8_R8() {
    const auto& r_dest = r8_lookup[(this->opcode >> 3) & 0b00000111];
    const auto& r_src = r8_lookup[this->opcode & 0b00000111];

    if (r_dest.reg == nullptr || r_src.reg == nullptr)
    {
        throw BadRegisterException();
    }

    this->str_inst = this->inst.name + " " + r_dest.name + ", " + r_src.name;
    this->*r_dest.reg = this->*r_src.reg;
}

void CPU::LD_R8_IMM8() {
    const auto& r_dest = r8_lookup[(this->opcode >> 3) & 0b00000111U];
    const auto r_imm = this->memory.read(this->PC++);

    if (r_dest.reg == nullptr)
    {
        throw BadRegisterException();
    }

    this->str_inst = this->inst.name + " " + r_dest.name + ", " + std::to_string(r_imm);
    this->*r_dest.reg = r_imm;
}

void CPU::LD_R8_MEM_HL() {
    const auto& r_dest = r8_lookup[(this->opcode >> 3) & 0b00000111U];
    const auto mem_val = this->memory.read(REG16_PAIR_GET(this->H, this->L));

    if (r_dest.reg == nullptr)
    {
        throw BadRegisterException();
    }

    this->str_inst = this->inst.name + " " + r_dest.name + ", " + "$(" + std::to_string(REG16_PAIR_GET(this->H, this->L)) + ")";

    this->*r_dest.reg = mem_val;
}

void CPU::AND_R8()
{
    const auto& r_src = r8_lookup[this->opcode & 0b00000111];

    if (r_src.reg == nullptr)
    {
        throw BadRegisterException();
    }

    this->str_inst = this->inst.name + " " + r_src.name;
    this->A &= this->*r_src.reg;
    if (this->A == 0)
    {
        this->F |= ZERO;
    } else
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

    this->str_inst = this->inst.name + " " + r_src.name;
    this->A |= this->*r_src.reg;
    if (this->A == 0)
    {
        this->F |= ZERO;
    } else
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

    this->str_inst = this->inst.name + " " + r_src.name;
    this->A ^= this->*r_src.reg;
    if (this->A == 0)
    {
        this->F |= ZERO;
    } else
    {
        this->F &= ~ZERO;
    }
    this->F &= ~HALF_CARRY;
    this->F &= ~SUBTRACT;
    this->F &= ~CARRY;
}

void CPU::ADD_R8(){
    const auto& r_src = r8_lookup[this->opcode & 0b00000111];
    uint8_t half_carry = 0;

    if (r_src.reg == nullptr)
    {
        throw BadRegisterException();
    }

    this->str_inst = this->inst.name + " " + r_src.name;

    half_carry = ((this->A & 0x0F) + (this->*r_src.reg & 0x0F)) & 0x10;
    if (half_carry) {
        this->F |= HALF_CARRY;
    } else
    {
        this->F &= ~HALF_CARRY;
    }
    if (__builtin_add_overflow(this->A, this->*r_src.reg, &this->A))
    {
        this->F |= CARRY;
    } else
    {
        this->F &= ~CARRY;
    }
    if (this->A == 0)
    {
        this->F |= ZERO;
    } else
    {
        this->F &= ~ZERO;
    }
    this->F &= ~SUBTRACT;
}

void CPU::ILL() {
    throw IllegalInstructionException();
}




void CPU::cycle() {
    if (this->cycles == 0)
    {
        //TODO: CB Prefix
        this->opcode = this->memory.read(this->PC);
        this->inst = inst_lookup.at(opcode);
        this->cycles = this->inst.cycles;
        this->PC++;
        (this->*inst.op)();
    } else
    {
        this->cycles--;
    }
}