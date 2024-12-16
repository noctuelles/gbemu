#include "CPU.hxx"

#include <x86intrin.h>

#include <iomanip>
#include <utility>

const CPU::InstructionLookupTable CPU::inst_lookup{{
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
    CPU::Instruction::LD_R8_IMM8(),    // 0x3E
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
    CPU::Instruction::PREFIX(),        // 0xCB
    {},                                // 0xCC
    {},                                // 0xCD
    {},                                // 0xCE
    {},                                // 0xCF
    {},                                // 0xD0
    {},                                // 0xD1
    {},                                // 0xD2
    {},                                // 0xD3
    {},                                // 0xD4
    {},                                // 0xD5
    {},                                // 0xD6
    {},                                // 0xD7
    {},                                // 0xD8
    {},                                // 0xD9
    {},                                // 0xDA
    {},                                // 0xDB
    {},                                // 0xDC
    {},                                // 0xDD
    {},                                // 0xDE
    {},                                // 0xDF
    {},                                // 0xE0
    {},                                // 0xE1
    {},                                // 0xE2
    {},                                // 0xE3
    {},                                // 0xE4
    {},                                // 0xE5
    {},                                // 0xE6
    {},                                // 0xE7
    {},                                // 0xE8
    {},                                // 0xE9
    {},                                // 0xEA
    {},                                // 0xEB
    {},                                // 0xEC
    {},                                // 0xED
    {},                                // 0xEE
    {},                                // 0xEF
    {},                                // 0xF0
    {},                                // 0xF1
    {},                                // 0xF2
    {},                                // 0xF3
    {},                                // 0xF4
    {},                                // 0xF5
    {},                                // 0xF6
    {},                                // 0xF7
    {},                                // 0xF8
    {},                                // 0xF9
    {},                                // 0xFA
    {},                                // 0xFB
    {},                                // 0xFC
    {},                                // 0xFD
    {},                                // 0xFE
    {},                                // 0xFF
}};

const CPU::InstructionLookupTable CPU::cb_prefixed_inst_lookup{{
    CPU::Instruction::RLC_R8(),       // 0x00
    CPU::Instruction::RLC_R8(),       // 0x01
    CPU::Instruction::RLC_R8(),       // 0x02
    CPU::Instruction::RLC_R8(),       // 0x03
    CPU::Instruction::RLC_R8(),       // 0x04
    CPU::Instruction::RLC_R8(),       // 0x05
    CPU::Instruction::RLC_MEM_HL(),   // 0x06
    CPU::Instruction::RLC_R8(),       // 0x07
    CPU::Instruction::RRC_R8(),       // 0x08
    CPU::Instruction::RRC_R8(),       // 0x0a
    CPU::Instruction::RRC_R8(),       // 0x0A
    CPU::Instruction::RRC_R8(),       // 0x0B
    CPU::Instruction::RRC_R8(),       // 0x0C
    CPU::Instruction::RRC_R8(),       // 0x0D
    CPU::Instruction::RRC_MEM_HL(),   // 0x0E
    CPU::Instruction::RRC_R8(),       // 0x0F
    CPU::Instruction::RL_R8(),        // 0x10
    CPU::Instruction::RL_R8(),        // 0x11
    CPU::Instruction::RL_R8(),        // 0x12
    CPU::Instruction::RL_R8(),        // 0x13
    CPU::Instruction::RL_R8(),        // 0x14
    CPU::Instruction::RL_R8(),        // 0x15
    CPU::Instruction::RL_MEM_HL(),    // 0x16
    CPU::Instruction::RL_R8(),        // 0x17
    CPU::Instruction::RR_R8(),        // 0x18
    CPU::Instruction::RR_R8(),        // 0x19
    CPU::Instruction::RR_R8(),        // 0x1A
    CPU::Instruction::RR_R8(),        // 0x1B
    CPU::Instruction::RR_R8(),        // 0x1C
    CPU::Instruction::RR_R8(),        // 0x1D
    CPU::Instruction::RR_MEM_HL(),    // 0x1E
    CPU::Instruction::RR_R8(),        // 0x1F
    CPU::Instruction::SLA_R8(),       // 0x20
    CPU::Instruction::SLA_R8(),       // 0x22
    CPU::Instruction::SLA_R8(),       // 0x22
    CPU::Instruction::SLA_R8(),       // 0x23
    CPU::Instruction::SLA_R8(),       // 0x24
    CPU::Instruction::SLA_R8(),       // 0x25
    CPU::Instruction::SLA_MEM_HL(),   // 0x26
    CPU::Instruction::SLA_R8(),       // 0x27
    CPU::Instruction::SRA_R8(),       // 0x28
    CPU::Instruction::SRA_R8(),       // 0x29
    CPU::Instruction::SRA_R8(),       // 0x2A
    CPU::Instruction::SRA_R8(),       // 0x2B
    CPU::Instruction::SRA_R8(),       // 0x2C
    CPU::Instruction::SRA_R8(),       // 0x2D
    CPU::Instruction::SRA_MEM_HL(),   // 0x2E
    CPU::Instruction::SRA_R8(),       // 0x2F
    CPU::Instruction::SWAP_R8(),      // 0x30
    CPU::Instruction::SWAP_R8(),      // 0x31
    CPU::Instruction::SWAP_R8(),      // 0x32
    CPU::Instruction::SWAP_R8(),      // 0x33
    CPU::Instruction::SWAP_R8(),      // 0x34
    CPU::Instruction::SWAP_R8(),      // 0x35
    CPU::Instruction::SWAP_MEM_HL(),  // 0x36
    CPU::Instruction::SWAP_R8(),      // 0x37
    CPU::Instruction::SRL_R8(),       // 0x38
    CPU::Instruction::SRL_R8(),       // 0x39
    CPU::Instruction::SRL_R8(),       // 0x3A
    CPU::Instruction::SRL_R8(),       // 0x3B
    CPU::Instruction::SRL_R8(),       // 0x3C
    CPU::Instruction::SRL_R8(),       // 0x3D
    CPU::Instruction::SRL_MEM_HL(),   // 0x3E
    CPU::Instruction::SRL_R8(),       // 0x3F
    CPU::Instruction::BIT_R8(),       // 0x40
    CPU::Instruction::BIT_R8(),       // 0x41
    CPU::Instruction::BIT_R8(),       // 0x42
    CPU::Instruction::BIT_R8(),       // 0x43
    CPU::Instruction::BIT_R8(),       // 0x44
    CPU::Instruction::BIT_R8(),       // 0x45
    CPU::Instruction::BIT_MEM_HL(),   // 0x46
    CPU::Instruction::BIT_R8(),       // 0x47
    CPU::Instruction::BIT_R8(),       // 0x48
    CPU::Instruction::BIT_R8(),       // 0x49
    CPU::Instruction::BIT_R8(),       // 0x4A
    CPU::Instruction::BIT_R8(),       // 0x4B
    CPU::Instruction::BIT_R8(),       // 0x4C
    CPU::Instruction::BIT_R8(),       // 0x4D
    CPU::Instruction::BIT_MEM_HL(),   // 0x4E
    CPU::Instruction::BIT_R8(),       // 0x4F
    CPU::Instruction::BIT_R8(),       // 0x50
    CPU::Instruction::BIT_R8(),       // 0x51
    CPU::Instruction::BIT_R8(),       // 0x52
    CPU::Instruction::BIT_R8(),       // 0x53
    CPU::Instruction::BIT_R8(),       // 0x54
    CPU::Instruction::BIT_R8(),       // 0x55
    CPU::Instruction::BIT_MEM_HL(),   // 0x56
    CPU::Instruction::BIT_R8(),       // 0x57
    CPU::Instruction::BIT_R8(),       // 0x58
    CPU::Instruction::BIT_R8(),       // 0x59
    CPU::Instruction::BIT_R8(),       // 0x5A
    CPU::Instruction::BIT_R8(),       // 0x5B
    CPU::Instruction::BIT_R8(),       // 0x5C
    CPU::Instruction::BIT_R8(),       // 0x5D
    CPU::Instruction::BIT_MEM_HL(),   // 0x5E
    CPU::Instruction::BIT_R8(),       // 0x5F
    CPU::Instruction::BIT_R8(),       // 0x60
    CPU::Instruction::BIT_R8(),       // 0x61
    CPU::Instruction::BIT_R8(),       // 0x62
    CPU::Instruction::BIT_R8(),       // 0x63
    CPU::Instruction::BIT_R8(),       // 0x64
    CPU::Instruction::BIT_R8(),       // 0x65
    CPU::Instruction::BIT_MEM_HL(),   // 0x66
    CPU::Instruction::BIT_R8(),       // 0x67
    CPU::Instruction::BIT_R8(),       // 0x68
    CPU::Instruction::BIT_R8(),       // 0x69
    CPU::Instruction::BIT_R8(),       // 0x6A
    CPU::Instruction::BIT_R8(),       // 0x6B
    CPU::Instruction::BIT_R8(),       // 0x6C
    CPU::Instruction::BIT_R8(),       // 0x6D
    CPU::Instruction::BIT_MEM_HL(),   // 0x6E
    CPU::Instruction::BIT_R8(),       // 0x6F
    CPU::Instruction::BIT_R8(),       // 0x70
    CPU::Instruction::BIT_R8(),       // 0x71
    CPU::Instruction::BIT_R8(),       // 0x72
    CPU::Instruction::BIT_R8(),       // 0x73
    CPU::Instruction::BIT_R8(),       // 0x74
    CPU::Instruction::BIT_R8(),       // 0x75
    CPU::Instruction::BIT_MEM_HL(),   // 0x76
    CPU::Instruction::BIT_R8(),       // 0x77
    CPU::Instruction::BIT_R8(),       // 0x78
    CPU::Instruction::BIT_R8(),       // 0x79
    CPU::Instruction::BIT_R8(),       // 0x7A
    CPU::Instruction::BIT_R8(),       // 0x7B
    CPU::Instruction::BIT_R8(),       // 0x7C
    CPU::Instruction::BIT_R8(),       // 0x7D
    CPU::Instruction::BIT_MEM_HL(),   // 0x7E
    CPU::Instruction::BIT_R8(),       // 0x7F
    CPU::Instruction::RES_R8(),       // 0x80
    CPU::Instruction::RES_R8(),       // 0x81
    CPU::Instruction::RES_R8(),       // 0x82
    CPU::Instruction::RES_R8(),       // 0x83
    CPU::Instruction::RES_R8(),       // 0x84
    CPU::Instruction::RES_R8(),       // 0x85
    CPU::Instruction::RES_MEM_HL(),   // 0x86
    CPU::Instruction::RES_R8(),       // 0x87
    CPU::Instruction::RES_R8(),       // 0x88
    CPU::Instruction::RES_R8(),       // 0x89
    CPU::Instruction::RES_R8(),       // 0x8A
    CPU::Instruction::RES_R8(),       // 0x8B
    CPU::Instruction::RES_R8(),       // 0x8C
    CPU::Instruction::RES_R8(),       // 0x8D
    CPU::Instruction::RES_MEM_HL(),   // 0x8E
    CPU::Instruction::RES_R8(),       // 0x8F
    CPU::Instruction::RES_R8(),       // 0x90
    CPU::Instruction::RES_R8(),       // 0x91
    CPU::Instruction::RES_R8(),       // 0x92
    CPU::Instruction::RES_R8(),       // 0x93
    CPU::Instruction::RES_R8(),       // 0x94
    CPU::Instruction::RES_R8(),       // 0x95
    CPU::Instruction::RES_MEM_HL(),   // 0x96
    CPU::Instruction::RES_R8(),       // 0x97
    CPU::Instruction::RES_R8(),       // 0x98
    CPU::Instruction::RES_R8(),       // 0x99
    CPU::Instruction::RES_R8(),       // 0x9A
    CPU::Instruction::RES_R8(),       // 0x9B
    CPU::Instruction::RES_R8(),       // 0x9C
    CPU::Instruction::RES_R8(),       // 0x9D
    CPU::Instruction::RES_MEM_HL(),   // 0x9E
    CPU::Instruction::RES_R8(),       // 0x9F
    CPU::Instruction::RES_R8(),       // 0xA0
    CPU::Instruction::RES_R8(),       // 0xA1
    CPU::Instruction::RES_R8(),       // 0xA2
    CPU::Instruction::RES_R8(),       // 0xA3
    CPU::Instruction::RES_R8(),       // 0xA4
    CPU::Instruction::RES_R8(),       // 0xA5
    CPU::Instruction::RES_MEM_HL(),   // 0xA6
    CPU::Instruction::RES_R8(),       // 0xA7
    CPU::Instruction::RES_R8(),       // 0xA8
    CPU::Instruction::RES_R8(),       // 0xA9
    CPU::Instruction::RES_R8(),       // 0xAA
    CPU::Instruction::RES_R8(),       // 0xAB
    CPU::Instruction::RES_R8(),       // 0xAC
    CPU::Instruction::RES_R8(),       // 0xAD
    CPU::Instruction::RES_MEM_HL(),   // 0xAE
    CPU::Instruction::RES_R8(),       // 0xAF
    CPU::Instruction::RES_R8(),       // 0xB0
    CPU::Instruction::RES_R8(),       // 0xB1
    CPU::Instruction::RES_R8(),       // 0xB2
    CPU::Instruction::RES_R8(),       // 0xB3
    CPU::Instruction::RES_R8(),       // 0xB4
    CPU::Instruction::RES_R8(),       // 0xB5
    CPU::Instruction::RES_MEM_HL(),   // 0xB6
    CPU::Instruction::RES_R8(),       // 0xB7
    CPU::Instruction::RES_R8(),       // 0xB8
    CPU::Instruction::RES_R8(),       // 0xB9
    CPU::Instruction::RES_R8(),       // 0xBA
    CPU::Instruction::RES_R8(),       // 0xBB
    CPU::Instruction::RES_R8(),       // 0xBC
    CPU::Instruction::RES_R8(),       // 0xBD
    CPU::Instruction::RES_MEM_HL(),   // 0xBE
    CPU::Instruction::RES_R8(),       // 0xBF
    CPU::Instruction::SET_R8(),       // 0xC0
    CPU::Instruction::SET_R8(),       // 0xC1
    CPU::Instruction::SET_R8(),       // 0xC2
    CPU::Instruction::SET_R8(),       // 0xC3
    CPU::Instruction::SET_R8(),       // 0xC4
    CPU::Instruction::SET_R8(),       // 0xC5
    CPU::Instruction::SET_MEM_HL(),   // 0xC6
    CPU::Instruction::SET_R8(),       // 0xC7
    CPU::Instruction::SET_R8(),       // 0xC8
    CPU::Instruction::SET_R8(),       // 0xC9
    CPU::Instruction::SET_R8(),       // 0xCA
    CPU::Instruction::SET_R8(),       // 0xCB
    CPU::Instruction::SET_R8(),       // 0xCC
    CPU::Instruction::SET_R8(),       // 0xCD
    CPU::Instruction::SET_MEM_HL(),   // 0xCE
    CPU::Instruction::SET_R8(),       // 0xCF
    CPU::Instruction::SET_R8(),       // 0xD0
    CPU::Instruction::SET_R8(),       // 0xD1
    CPU::Instruction::SET_R8(),       // 0xD2
    CPU::Instruction::SET_R8(),       // 0xD3
    CPU::Instruction::SET_R8(),       // 0xD4
    CPU::Instruction::SET_R8(),       // 0xD5
    CPU::Instruction::SET_MEM_HL(),   // 0xD6
    CPU::Instruction::SET_R8(),       // 0xD7
    CPU::Instruction::SET_R8(),       // 0xD8
    CPU::Instruction::SET_R8(),       // 0xD9
    CPU::Instruction::SET_R8(),       // 0xDA
    CPU::Instruction::SET_R8(),       // 0xDB
    CPU::Instruction::SET_R8(),       // 0xDC
    CPU::Instruction::SET_R8(),       // 0xDD
    CPU::Instruction::SET_MEM_HL(),   // 0xDE
    CPU::Instruction::SET_R8(),       // 0xDF
    CPU::Instruction::SET_R8(),       // 0xE0
    CPU::Instruction::SET_R8(),       // 0xE1
    CPU::Instruction::SET_R8(),       // 0xE2
    CPU::Instruction::SET_R8(),       // 0xE3
    CPU::Instruction::SET_R8(),       // 0xE4
    CPU::Instruction::SET_R8(),       // 0xE5
    CPU::Instruction::SET_MEM_HL(),   // 0xE6
    CPU::Instruction::SET_R8(),       // 0xE7
    CPU::Instruction::SET_R8(),       // 0xE8
    CPU::Instruction::SET_R8(),       // 0xE9
    CPU::Instruction::SET_R8(),       // 0xEA
    CPU::Instruction::SET_R8(),       // 0xEB
    CPU::Instruction::SET_R8(),       // 0xEC
    CPU::Instruction::SET_R8(),       // 0xED
    CPU::Instruction::SET_MEM_HL(),   // 0xEE
    CPU::Instruction::SET_R8(),       // 0xEF
    CPU::Instruction::SET_R8(),       // 0xF0
    CPU::Instruction::SET_R8(),       // 0xF1
    CPU::Instruction::SET_R8(),       // 0xF2
    CPU::Instruction::SET_R8(),       // 0xF3
    CPU::Instruction::SET_R8(),       // 0xF4
    CPU::Instruction::SET_R8(),       // 0xF5
    CPU::Instruction::SET_MEM_HL(),   // 0xF6
    CPU::Instruction::SET_R8(),       // 0xF7
    CPU::Instruction::SET_R8(),       // 0xF8
    CPU::Instruction::SET_R8(),       // 0xF9
    CPU::Instruction::SET_R8(),       // 0xFA
    CPU::Instruction::SET_R8(),       // 0xFB
    CPU::Instruction::SET_R8(),       // 0xFC
    CPU::Instruction::SET_R8(),       // 0xFD
    CPU::Instruction::SET_MEM_HL(),   // 0xFE
    CPU::Instruction::SET_R8(),       // 0xFF
}};

constexpr CPU::Instruction::Instruction() : cycles(0), op(&CPU::ILL) {}

constexpr CPU::Instruction::Instruction(const size_t cycles, void (CPU::*op)()) : cycles(cycles), op(op) {}

constexpr CPU::Instruction CPU::Instruction::LD_R8_R8()
{
    return Instruction{8U, &CPU::LD_R8_R8};
}

constexpr CPU::Instruction CPU::Instruction::LD_R8_IMM8()
{
    return Instruction{8U, &CPU::LD_R8_IMM8};
}

constexpr CPU::Instruction CPU::Instruction::LD_R16_IMM16()
{
    return Instruction{12U, &CPU::LD_R16_IMM16};
}

constexpr CPU::Instruction CPU::Instruction::LD_R8_MEM_HL()
{
    return Instruction{8U, &CPU::LD_R8_MEM_HL};
}

constexpr CPU::Instruction CPU::Instruction::LD_MEM_HL_R8()
{
    return Instruction{8U, &CPU::LD_MEM_HL_R8};
}
constexpr CPU::Instruction CPU::Instruction::LD_MEM_HL_IMM8()
{
    return Instruction{8U, &CPU::LD_MEM_HL_IMM8};
}

constexpr CPU::Instruction CPU::Instruction::AND_R8()
{
    return Instruction{4U, &CPU::AND_R8};
}

constexpr CPU::Instruction CPU::Instruction::XOR_R8()
{
    return Instruction{4U, &CPU::XOR_R8};
}

constexpr CPU::Instruction CPU::Instruction::OR_R8()
{
    return Instruction{4U, &CPU::OR_R8};
}

constexpr CPU::Instruction CPU::Instruction::ILL()
{
    return Instruction{0U, &CPU::ILL};
}

constexpr CPU::Instruction CPU::Instruction::PREFIX()
{
    return Instruction{4U, &CPU::PREFIX};
}

constexpr CPU::Instruction CPU::Instruction::RRC_R8()
{
    return Instruction{8U, &CPU::RRC_R8};
}

constexpr CPU::Instruction CPU::Instruction::RRC_MEM_HL()
{
    return Instruction{16U, &CPU::RRC_MEM_HL};
}

constexpr CPU::Instruction CPU::Instruction::RLC_R8()
{
    return Instruction{8U, &CPU::RLC_R8};
}

constexpr CPU::Instruction CPU::Instruction::RLC_MEM_HL()
{
    return Instruction{16U, &CPU::RLC_MEM_HL};
}

constexpr CPU::Instruction CPU::Instruction::RR_R8()
{
    return Instruction{8U, &CPU::RR_R8};
}

constexpr CPU::Instruction CPU::Instruction::RR_MEM_HL()
{
    return Instruction{16U, &CPU::RR_MEM_HL};
}

constexpr CPU::Instruction CPU::Instruction::RL_R8()
{
    return Instruction{8U, &CPU::RL_R8};
}

constexpr CPU::Instruction CPU::Instruction::RL_MEM_HL()
{
    return Instruction{16U, &CPU::RL_MEM_HL};
}

constexpr CPU::Instruction CPU::Instruction::SLA_R8()
{
    return Instruction{8U, &CPU::SLA_R8};
}

constexpr CPU::Instruction CPU::Instruction::SLA_MEM_HL()
{
    return Instruction{8U, &CPU::SLA_MEM_HL};
}

constexpr CPU::Instruction CPU::Instruction::SRL_R8()
{
    return Instruction{8U, &CPU::SRL_R8};
}

constexpr CPU::Instruction CPU::Instruction::SRL_MEM_HL()
{
    return Instruction{16U, &CPU::SRL_R8};
}
constexpr CPU::Instruction CPU::Instruction::SRA_R8()
{
    return Instruction{8U, &CPU::SRA_R8};
}

constexpr CPU::Instruction CPU::Instruction::SRA_MEM_HL()
{
    return Instruction{16U, &CPU::SRA_MEM_HL};
}

constexpr CPU::Instruction CPU::Instruction::SWAP_R8()
{
    return Instruction{8U, &CPU::SWAP_R8};
}

constexpr CPU::Instruction CPU::Instruction::SWAP_MEM_HL()
{
    return Instruction{16U, &CPU::SWAP_MEM_HL};
}

constexpr CPU::Instruction CPU::Instruction::BIT_R8()
{
    return Instruction{8U, &CPU::BIT_R8};
}

constexpr CPU::Instruction CPU::Instruction::BIT_MEM_HL()
{
    return Instruction{12U, &CPU::BIT_MEM_HL};
}

constexpr CPU::Instruction CPU::Instruction::RES_R8()
{
    return Instruction{8U, &CPU::RES_R8};
}

constexpr CPU::Instruction CPU::Instruction::RES_MEM_HL()
{
    return Instruction{16U, &CPU::RES_MEM_HL};
}
constexpr CPU::Instruction CPU::Instruction::SET_R8()
{
    return Instruction{8U, &CPU::SET_R8};
}

constexpr CPU::Instruction CPU::Instruction::SET_MEM_HL()
{
    return Instruction{16U, &CPU::SET_MEM_HL};
}

const char* CPU::BadRegister::what() const noexcept
{
    return "Bad Register";
}

CPU::CPU(Bus& bus) : reg(), cycles(0), opcode(0), cb_prefixed(false), inst(inst_lookup[0]), bus(bus) {}

CPU::~CPU() = default;

void CPU::NOP() {}

void CPU::LD_R8_R8()
{
    const auto [dest, src] = this->get_register8_dest_src_from_opcode();
    this->reg.u8.*dest     = this->reg.u8.*src;
}

void CPU::LD_R8_IMM8()
{
    const auto dest = this->get_register8_dest_from_opcode();
    const auto imm{this->bus.read(this->reg.u16.PC++)};

    this->reg.u8.*dest = imm;
}

void CPU::LD_R16_IMM16()
{
    const auto dest{this->get_register16_dest_from_opcode()};
    const auto imm_lsb{this->bus.read(this->reg.u16.PC++)};
    const auto imm_msb{this->bus.read(this->reg.u16.PC++)};

    this->reg.u16.*dest = static_cast<uint16_t>(imm_msb << 8 | imm_lsb);
}

void CPU::LD_R8_MEM_HL()
{
    const auto src{this->get_register8_dest_from_opcode()};
    const auto mem_val = this->bus.read(this->reg.u16.HL);

    this->reg.u8.*src = mem_val;
}

void CPU::LD_MEM_HL_R8()
{
    const auto src{this->get_register8_src_from_opcode()};
    this->bus.write(this->reg.u16.HL, this->reg.u8.*src);
}

void CPU::LD_MEM_HL_IMM8()
{
    this->bus.write(this->reg.u16.HL, this->bus.read(this->reg.u16.PC++));
}

void CPU::PUSH_R16()
{
    const auto src{static_cast<uint8_t>((this->opcode >> 4) & 0b00000011U)};
    Register16 rsrc{nullptr};

    switch (src)
    {
        case 0b00:
            rsrc = &Register::U16::BC;
            break;
        case 0b01:
            rsrc = &Register::U16::DE;
            break;
        case 0b10:
            rsrc = &Register::U16::HL;
            break;
        case 0b11:
            rsrc = &Register::U16::AF;
            break;
        default:
            throw BadRegister();
    }

    this->bus.write(this->reg.u16.SP--, (this->reg.u16.*rsrc >> 8U) & 0xFFU);
    this->bus.write(this->reg.u16.SP--, this->reg.u16.*rsrc & 0xFFU);
}

void CPU::RES_R8()
{
    const auto src = this->get_register8_src_from_opcode();
    const auto bit = static_cast<uint8_t>((this->opcode & 0b00111000) >> 3);

    this->reg.u8.*src &= ~(1 << bit);
}

void CPU::RES_MEM_HL()
{
    const auto bit     = static_cast<uint8_t>((this->opcode & 0b00111000) >> 3);
    auto       mem_val = this->bus.read(this->reg.u16.HL);

    mem_val &= ~(1 << bit);

    this->bus.write(this->reg.u16.HL, mem_val);
}

void CPU::SET_R8()
{
    const auto src = this->get_register8_src_from_opcode();
    const auto bit = static_cast<uint8_t>((this->opcode & 0b00111000) >> 3);

    this->reg.u8.*src |= 1 << bit;
}

void CPU::SET_MEM_HL()
{
    const auto bit     = static_cast<uint8_t>((this->opcode & 0b00111000) >> 3);
    auto       mem_val = this->bus.read(this->reg.u16.HL);

    mem_val |= 1 << bit;

    this->bus.write(this->reg.u16.HL, mem_val);
}

void CPU::AND_R8()
{
    const auto src = get_register8(static_cast<OperandRegister8>(this->opcode & 0b00000111));

    this->reg.u8.A &= this->reg.u8.*src;
    if (this->reg.u8.A == 0)
    {
        this->reg.u8.F |= Flags::ZERO;
    }
    else
    {
        this->reg.u8.F &= ~Flags::ZERO;
    }
    this->reg.u8.F |= Flags::HALF_CARRY;
    this->reg.u8.F &= ~Flags::SUBTRACT;
    this->reg.u8.F &= ~Flags::CARRY;
}

void CPU::OR_R8()
{
    const auto src = this->get_register8_src_from_opcode();

    this->reg.u8.A |= this->reg.u8.*src;
    if (this->reg.u8.A == 0)
    {
        this->reg.u8.F |= Flags::ZERO;
    }
    else
    {
        this->reg.u8.F &= ~Flags::ZERO;
    }
    this->reg.u8.F &= ~Flags::HALF_CARRY;
    this->reg.u8.F &= ~Flags::SUBTRACT;
    this->reg.u8.F &= ~Flags::CARRY;
}

void CPU::XOR_R8()
{
    const auto r_src = get_register8(static_cast<OperandRegister8>(this->opcode & 0b00000111));

    this->reg.u8.A ^= this->reg.u8.*r_src;
    if (this->reg.u8.A == 0)
    {
        this->reg.u8.F |= Flags::ZERO;
    }
    else
    {
        this->reg.u8.F &= ~Flags::ZERO;
    }
    this->reg.u8.F &= ~Flags::HALF_CARRY;
    this->reg.u8.F &= ~Flags::SUBTRACT;
    this->reg.u8.F &= ~Flags::CARRY;
}

void CPU::ADD_A_R8()
{
    const auto src        = this->get_register8_src_from_opcode();
    uint8_t    half_carry = 0;

    half_carry = ((this->reg.u8.A & 0x0F) + (this->reg.u8.*src & 0x0F)) & 0x10;
    if (half_carry)
    {
        this->reg.u8.F |= Flags::HALF_CARRY;
    }
    else
    {
        this->reg.u8.F &= ~Flags::HALF_CARRY;
    }
    if (__builtin_add_overflow(this->reg.u8.A, this->reg.u8.*src, &this->reg.u8.A))
    {
        this->reg.u8.F |= Flags::CARRY;
    }
    if (this->reg.u8.A == 0)
    {
        this->reg.u8.F |= Flags::ZERO;
    }
    this->reg.u8.F &= ~Flags::SUBTRACT;
}

void CPU::INC_R8()
{
    const auto dest = get_register8(static_cast<OperandRegister8>((this->opcode >> 4) & 0b00000011U));
    this->reg.u8.*dest += 1;
}

void CPU::ILL()
{
    throw IllegalInstruction();
}

void CPU::PREFIX()
{
    this->cb_prefixed = true;
}

auto CPU::ROTATE(uint8_t val, const RotateDirection rotate_direction, const bool rotate_through_carry) noexcept
{
    auto has_new_carry = false;

    if (rotate_direction == RotateDirection::LEFT)
    {
        has_new_carry = (val & 0b10000000) != 0;
        val <<= 1;
        if (rotate_through_carry)
        {
            val |= (this->reg.u8.F & Flags::CARRY ? 0b00000001 : 0b00000000);
        }
    }
    else
    {
        has_new_carry = (val & 0b00000001) != 0;
        val >>= 1;
        if (rotate_through_carry)
        {
            val |= (this->reg.u8.F & Flags::CARRY ? 0b10000000 : 0b00000000);
        }
    }
    if (has_new_carry)
    {
        this->reg.u8.F |= Flags::CARRY;
    }
    else
    {
        this->reg.u8.F &= ~Flags::CARRY;
    }
    this->reg.u8.F &= ~Flags::HALF_CARRY;
    this->reg.u8.F &= ~Flags::SUBTRACT;

    return (val);
}
void CPU::RLC_R8()
{
    const auto src    = this->get_register8_src_from_opcode();
    this->reg.u8.*src = this->ROTATE(this->reg.u8.*src, RotateDirection::LEFT, false);
}

void CPU::RLC_MEM_HL()
{
    auto val = this->bus.read(this->reg.u16.HL);
    val      = this->ROTATE(val, RotateDirection::LEFT, false);
    this->bus.write(this->reg.u16.HL, val);
}

void CPU::RRC_R8()
{
    const auto src    = this->get_register8_src_from_opcode();
    this->reg.u8.*src = this->ROTATE(this->reg.u8.*src, RotateDirection::RIGHT, false);
}

void CPU::RRC_MEM_HL()
{
    auto val = this->bus.read(this->reg.u16.HL);
    val      = this->ROTATE(val, RotateDirection::RIGHT, false);
    this->bus.write(this->reg.u16.HL, val);
}

void CPU::RL_R8()
{
    const auto src    = this->get_register8_src_from_opcode();
    this->reg.u8.*src = this->ROTATE(this->reg.u8.*src, RotateDirection::LEFT, true);
}
void CPU::RL_MEM_HL()
{
    auto val = this->bus.read(this->reg.u16.HL);
    val      = this->ROTATE(val, RotateDirection::LEFT, true);
    this->bus.write(this->reg.u16.HL, val);
}

void CPU::RR_R8()
{
    const auto src    = this->get_register8_src_from_opcode();
    this->reg.u8.*src = this->ROTATE(this->reg.u8.*src, RotateDirection::RIGHT, true);
}

void CPU::RR_MEM_HL()
{
    auto val = this->bus.read(this->reg.u16.HL);
    val      = this->ROTATE(val, RotateDirection::LEFT, true);
    this->bus.write(this->reg.u16.HL, val);
}

auto CPU::SHIFT(uint8_t val, const ShiftType shift_type, const ShiftDirection shift_direction) noexcept
{
    auto sign_bit = false;

    if (val & ((shift_direction == ShiftDirection::RIGHT) ? 0b00000001U : 0b10000000U))
    {
        this->reg.u8.F |= Flags::CARRY;
    }
    else
    {
        this->reg.u8.F &= ~Flags::CARRY;
    }
    sign_bit = val & 0b10000000U != 0;
    if (shift_direction == ShiftDirection::RIGHT)
    {
        val >>= 1;
    }
    else
    {
        val <<= 1;
    }
    if (sign_bit && shift_type == ShiftType::ARITHMETIC && shift_direction == ShiftDirection::RIGHT)
    {
        val |= 0b10000000U;
    }
    if (val == 0)
    {
        this->reg.u8.F |= Flags::ZERO;
    }
    this->reg.u8.F &= ~Flags::HALF_CARRY;
    this->reg.u8.F &= ~Flags::SUBTRACT;

    return val;
}

void CPU::SRA_R8()
{
    const auto src    = this->get_register8_src_from_opcode();
    this->reg.u8.*src = this->SHIFT(this->reg.u8.*src, ShiftType::ARITHMETIC, ShiftDirection::RIGHT);
}

void CPU::SRA_MEM_HL()
{
    auto val = this->bus.read(this->reg.u16.HL);
    val      = this->SHIFT(val, ShiftType::ARITHMETIC, ShiftDirection::RIGHT);
    this->bus.write(this->reg.u16.HL, val);
}

void CPU::SLA_R8()
{
    const auto src    = this->get_register8_src_from_opcode();
    this->reg.u8.*src = this->SHIFT(this->reg.u8.*src, ShiftType::ARITHMETIC, ShiftDirection::LEFT);
}

void CPU::SLA_MEM_HL()
{
    auto val = this->bus.read(this->reg.u16.HL);
    val      = this->SHIFT(val, ShiftType::ARITHMETIC, ShiftDirection::LEFT);
    this->bus.write(this->reg.u16.HL, val);
}

void CPU::SRL_R8()
{
    const auto src    = this->get_register8_src_from_opcode();
    this->reg.u8.*src = this->SHIFT(this->reg.u8.*src, ShiftType::LOGICAL, ShiftDirection::RIGHT);
}

void CPU::SRL_MEM_HL()
{
    auto val = this->bus.read(this->reg.u16.HL);
    val      = this->SHIFT(val, ShiftType::LOGICAL, ShiftDirection::RIGHT);
    this->bus.write(this->reg.u16.HL, val);
}

auto CPU::SWAP(uint8_t val)
{
    val = ((val & 0b11110000) >> 4) | ((val & 0b00001111) << 4);

    this->reg.u8.F &= ~Flags::SUBTRACT;
    this->reg.u8.F &= ~Flags::HALF_CARRY;
    this->reg.u8.F &= ~Flags::CARRY;

    if (val == 0)
    {
        this->reg.u8.F |= Flags::ZERO;
    }

    return (val);
}

void CPU::SWAP_R8()
{
    const auto src    = this->get_register8_src_from_opcode();
    this->reg.u8.*src = this->SWAP(this->reg.u8.*src);
}

void CPU::SWAP_MEM_HL()
{
    auto val = this->bus.read(this->reg.u16.HL);
    val      = this->SWAP(val);
    this->bus.write(this->reg.u16.HL, val);
}

void CPU::BIT(const uint8_t val, const uint8_t bit)
{
    if (!(val & (1 << bit)))
    {
        this->reg.u8.F |= Flags::ZERO;
    }
    this->reg.u8.F |= Flags::HALF_CARRY;
    this->reg.u8.F &= ~Flags::SUBTRACT;
}

void CPU::BIT_MEM_HL()
{
    const auto bit     = static_cast<uint8_t>((this->opcode & 0b00111000) >> 3);
    const auto mem_val = this->bus.read(this->reg.u16.HL);
    this->BIT(mem_val, bit);
}

void CPU::BIT_R8()
{
    const auto src = this->get_register8_src_from_opcode();
    const auto bit = static_cast<uint8_t>((this->opcode & 0b00111000U) >> 3U);
    this->BIT(this->reg.u8.*src, bit);
}

void CPU::cycle()
{
    if (this->cycles == 0)
    {
        this->opcode = this->bus.read(this->reg.u16.PC);
        if (this->cb_prefixed)
        {
            this->inst        = cb_prefixed_inst_lookup.at(opcode);
            this->cb_prefixed = false;
        }
        else
        {
            this->inst = inst_lookup.at(opcode);
        }
        this->cycles = this->inst.cycles;
        this->reg.u16.PC++;
        (this->*inst.op)();
    }
    this->cycles--;
}

CPU::Register CPU::get_register() const noexcept
{
    return this->reg;
}

CPU::Register8 CPU::get_register8(const OperandRegister8 reg)
{
    switch (reg)
    {
        case OperandRegister8::A:
            return &Register::U8::A;
        case OperandRegister8::B:
            return &Register::U8::B;
        case OperandRegister8::C:
            return &Register::U8::C;
        case OperandRegister8::D:
            return &Register::U8::D;
        case OperandRegister8::E:
            return &Register::U8::E;
        case OperandRegister8::H:
            return &Register::U8::H;
        case OperandRegister8::L:
            return &Register::U8::L;
        default:
            throw BadRegister();
    }
}

CPU::Register16 CPU::get_register16(const OperandRegister16 reg)
{
    switch (reg)
    {
        case OperandRegister16::BC:
            return &Register::U16::BC;
        case OperandRegister16::DE:
            return &Register::U16::DE;
        case OperandRegister16::HL:
            return &Register::U16::HL;
        case OperandRegister16::SP:
            return &Register::U16::SP;
        default:
            throw BadRegister();
    }
}

CPU::Register16 CPU::get_register16_dest_from_opcode() const
{
    return get_register16(static_cast<OperandRegister16>((this->opcode >> 4) & 0b00000011U));
}

CPU::Register16 CPU::get_register16_dest_from_opcode(const uint8_t opcode)
{
    return get_register16(static_cast<OperandRegister16>((opcode >> 4) & 0b00000011U));
}

CPU::Register8 CPU::get_register8_dest_from_opcode() const
{
    return get_register8(static_cast<OperandRegister8>((this->opcode >> 3) & 0b00000111U));
}

CPU::Register8 CPU::get_register8_dest_from_opcode(const uint8_t opcode)
{
    return get_register8(static_cast<OperandRegister8>((opcode >> 3) & 0b00000111U));
}

CPU::Register8 CPU::get_register8_src_from_opcode() const
{
    return get_register8(static_cast<OperandRegister8>(this->opcode & 0b00000111U));
}

CPU::Register8 CPU::get_register8_src_from_opcode(const uint8_t opcode)
{
    return get_register8(static_cast<OperandRegister8>(opcode & 0b00000111U));
}

std::pair<CPU::Register8, CPU::Register8> CPU::get_register8_dest_src_from_opcode() const
{
    return std::make_pair(this->get_register8_dest_from_opcode(), this->get_register8_src_from_opcode());
}

std::pair<CPU::Register8, CPU::Register8> CPU::get_register8_dest_src_from_opcode(uint8_t opcode)
{
    return std::make_pair(get_register8_dest_from_opcode(opcode), get_register8_src_from_opcode(opcode));
}