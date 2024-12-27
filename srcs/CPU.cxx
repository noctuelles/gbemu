#include "CPU.hxx"

/**
 * @note https://gbdev.io/gb-opcodes/optables/
 * https://rgbds.gbdev.io/docs/v0.8.0/gbz80.7
 * https://dn790000.ca.archive.org/0/items/GameBoyProgManVer1.1/GameBoyProgManVer1.1.pdf
 */

#include <Utils.hxx>
#include <iostream>
#include <utility>

const CPU::InstructionLookupTable CPU::inst_lookup{{
    Instruction::NOP(),             // 0x00
    Instruction::LD_R16_IMM16(),    // 0x01
    Instruction::LD_MEM_R16_A(),    // 0x02
    Instruction::INC_R16(),         // 0x03
    Instruction::INC_R8(),          // 0x04
    Instruction::DEC_R8(),          // 0x05
    Instruction::LD_R8_IMM8(),      // 0x06
    Instruction::RLCA(),            // 0x07
    Instruction::LD_MEM_16_SP(),    // 0x08
    Instruction::ADD_HL_R16(),      // 0x09
    Instruction::LD_A_MEM_R16(),    // 0x0A
    Instruction::DEC_R16(),         // 0x0B
    Instruction::INC_R8(),          // 0x0C
    Instruction::DEC_R8(),          // 0x0D
    Instruction::LD_R8_IMM8(),      // 0x0E
    Instruction::RRCA(),            // 0x0F
    Instruction::STOP(),            // 0x10
    Instruction::LD_R16_IMM16(),    // 0x11
    Instruction::LD_MEM_R16_A(),    // 0x12
    Instruction::INC_R16(),         // 0x13
    Instruction::INC_R8(),          // 0x14
    Instruction::DEC_R8(),          // 0x15
    Instruction::LD_R8_IMM8(),      // 0x16
    Instruction::RLA(),             // 0x17
    Instruction::JR_IMM8(),         // 0x18
    Instruction::ADD_HL_R16(),      // 0x19
    Instruction::LD_A_MEM_R16(),    // 0x1A
    Instruction::DEC_R16(),         // 0x1B
    Instruction::INC_R8(),          // 0x1C
    Instruction::DEC_R8(),          // 0x1D
    Instruction::LD_R8_IMM8(),      // 0x1E
    Instruction::RRA(),             // 0x1F
    Instruction::JR_CC_IMM8(),      // 0x20
    Instruction::LD_R16_IMM16(),    // 0x21
    Instruction::LD_MEM_R16_A(),    // 0x22
    Instruction::INC_R16(),         // 0x23
    Instruction::INC_R8(),          // 0x24
    Instruction::DEC_R8(),          // 0x25
    Instruction::LD_R8_IMM8(),      // 0x26
    Instruction::DAA(),             // 0x27
    Instruction::JR_CC_IMM8(),      // 0x28
    Instruction::ADD_HL_R16(),      // 0x29
    Instruction::LD_A_MEM_R16(),    // 0x2A
    Instruction::DEC_R16(),         // 0x2B
    Instruction::INC_R8(),          // 0x2C
    Instruction::DEC_R8(),          // 0x2D
    Instruction::LD_R8_IMM8(),      // 0x2E
    Instruction::CPL(),             // 0x2F
    Instruction::JR_CC_IMM8(),      // 0x30
    Instruction::LD_R16_IMM16(),    // 0x31
    Instruction::LD_MEM_R16_A(),    // 0x32
    Instruction::INC_R16(),         // 0x33
    Instruction::INC_MEM_HL(),      // 0x34
    Instruction::DEC_MEM_HL(),      // 0x35
    Instruction::LD_MEM_HL_IMM8(),  // 0x36
    Instruction::SCF(),             // 0x37
    Instruction::JR_CC_IMM8(),      // 0x38
    Instruction::ADD_HL_R16(),      // 0x39
    Instruction::LD_A_MEM_R16(),    // 0x3A
    Instruction::DEC_R16(),         // 0x3B
    Instruction::INC_R8(),          // 0x3C
    Instruction::DEC_R8(),          // 0x3D
    Instruction::LD_R8_IMM8(),      // 0x3E
    Instruction::CCF(),             // 0x3F
    Instruction::LD_R8_R8(),        // 0x40
    Instruction::LD_R8_R8(),        // 0x41
    Instruction::LD_R8_R8(),        // 0x42
    Instruction::LD_R8_R8(),        // 0x43
    Instruction::LD_R8_R8(),        // 0x44
    Instruction::LD_R8_R8(),        // 0x45
    Instruction::LD_R8_MEM_HL(),    // 0x46
    Instruction::LD_R8_R8(),        // 0x47
    Instruction::LD_R8_R8(),        // 0x48
    Instruction::LD_R8_R8(),        // 0x49
    Instruction::LD_R8_R8(),        // 0x4A
    Instruction::LD_R8_R8(),        // 0x4B
    Instruction::LD_R8_R8(),        // 0x4C
    Instruction::LD_R8_R8(),        // 0x4D
    Instruction::LD_R8_MEM_HL(),    // 0x4E
    Instruction::LD_R8_R8(),        // 0x4F
    Instruction::LD_R8_R8(),        // 0x50
    Instruction::LD_R8_R8(),        // 0x51
    Instruction::LD_R8_R8(),        // 0x52
    Instruction::LD_R8_R8(),        // 0x53
    Instruction::LD_R8_R8(),        // 0x54
    Instruction::LD_R8_R8(),        // 0x55
    Instruction::LD_R8_MEM_HL(),    // 0x56
    Instruction::LD_R8_R8(),        // 0x57
    Instruction::LD_R8_R8(),        // 0x58
    Instruction::LD_R8_R8(),        // 0x59
    Instruction::LD_R8_R8(),        // 0x5A
    Instruction::LD_R8_R8(),        // 0x5B
    Instruction::LD_R8_R8(),        // 0x5C
    Instruction::LD_R8_R8(),        // 0x5D
    Instruction::LD_R8_MEM_HL(),    // 0x5E
    Instruction::LD_R8_R8(),        // 0x5F
    Instruction::LD_R8_R8(),        // 0x60
    Instruction::LD_R8_R8(),        // 0x61
    Instruction::LD_R8_R8(),        // 0x62
    Instruction::LD_R8_R8(),        // 0x63
    Instruction::LD_R8_R8(),        // 0x64
    Instruction::LD_R8_R8(),        // 0x65
    Instruction::LD_R8_MEM_HL(),    // 0x66
    Instruction::LD_R8_R8(),        // 0x67
    Instruction::LD_R8_R8(),        // 0x68
    Instruction::LD_R8_R8(),        // 0x69
    Instruction::LD_R8_R8(),        // 0x6A
    Instruction::LD_R8_R8(),        // 0x6B
    Instruction::LD_R8_R8(),        // 0x6C
    Instruction::LD_R8_R8(),        // 0x6D
    Instruction::LD_R8_MEM_HL(),    // 0x6E
    Instruction::LD_R8_R8(),        // 0x6F
    Instruction::LD_MEM_HL_R8(),    // 0x70
    Instruction::LD_MEM_HL_R8(),    // 0x71
    Instruction::LD_MEM_HL_R8(),    // 0x72
    Instruction::LD_MEM_HL_R8(),    // 0x73
    Instruction::LD_MEM_HL_R8(),    // 0x74
    Instruction::LD_MEM_HL_R8(),    // 0x75
    Instruction::HALT(),            // 0x76
    Instruction::LD_MEM_HL_R8(),    // 0x77
    Instruction::LD_R8_R8(),        // 0x78
    Instruction::LD_R8_R8(),        // 0x79
    Instruction::LD_R8_R8(),        // 0x7A
    Instruction::LD_R8_R8(),        // 0x7B
    Instruction::LD_R8_R8(),        // 0x7C
    Instruction::LD_R8_R8(),        // 0x7D
    Instruction::LD_R8_MEM_HL(),    // 0x7E
    Instruction::LD_R8_R8(),        // 0x7F
    Instruction::ADD_R8(),          // 0x80
    Instruction::ADD_R8(),          // 0x81
    Instruction::ADD_R8(),          // 0x82
    Instruction::ADD_R8(),          // 0x83
    Instruction::ADD_R8(),          // 0x84
    Instruction::ADD_R8(),          // 0x85
    Instruction::ADD_MEM_HL(),      // 0x86
    Instruction::ADD_R8(),          // 0x87
    Instruction::ADC_R8(),          // 0x88
    Instruction::ADC_R8(),          // 0x8b
    Instruction::ADC_R8(),          // 0x8A
    Instruction::ADC_R8(),          // 0x8B
    Instruction::ADC_R8(),          // 0x8C
    Instruction::ADC_R8(),          // 0x8D
    Instruction::ADC_MEM_HL(),      // 0x8E
    Instruction::ADC_R8(),          // 0x90
    Instruction::SUB_R8(),          // 0x90
    Instruction::SUB_R8(),          // 0x91
    Instruction::SUB_R8(),          // 0x92
    Instruction::SUB_R8(),          // 0x93
    Instruction::SUB_R8(),          // 0x94
    Instruction::SUB_R8(),          // 0x95
    Instruction::SUB_MEM_HL(),      // 0x96
    Instruction::SUB_R8(),          // 0x97
    Instruction::SBC_R8(),          // 0x98
    Instruction::SBC_R8(),          // 0x99
    Instruction::SBC_R8(),          // 0x9A
    Instruction::SBC_R8(),          // 0x9B
    Instruction::SBC_R8(),          // 0x9C
    Instruction::SBC_R8(),          // 0x9D
    Instruction::SBC_MEM_HL(),      // 0x9E
    Instruction::SBC_R8(),          // 0x9F
    Instruction::AND_R8(),          // 0xA0
    Instruction::AND_R8(),          // 0xA1
    Instruction::AND_R8(),          // 0xA2
    Instruction::AND_R8(),          // 0xA3
    Instruction::AND_R8(),          // 0xA4
    Instruction::AND_R8(),          // 0xA5
    Instruction::AND_MEM_HL(),      // 0xA6
    Instruction::AND_R8(),          // 0xA7
    Instruction::XOR_R8(),          // 0xA8
    Instruction::XOR_R8(),          // 0xA9
    Instruction::XOR_R8(),          // 0xAA
    Instruction::XOR_R8(),          // 0xAB
    Instruction::XOR_R8(),          // 0xAC
    Instruction::XOR_R8(),          // 0xAD
    Instruction::XOR_MEM_HL(),      // 0xAE
    Instruction::XOR_R8(),          // 0xAF
    Instruction::OR_R8(),           // 0xB0
    Instruction::OR_R8(),           // 0xB1
    Instruction::OR_R8(),           // 0xB2
    Instruction::OR_R8(),           // 0xB3
    Instruction::OR_R8(),           // 0xB4
    Instruction::OR_R8(),           // 0xB5
    Instruction::OR_MEM_HL(),       // 0xB6
    Instruction::OR_R8(),           // 0xB7
    Instruction::CP_R8(),           // 0xB8
    Instruction::CP_R8(),           // 0xB9
    Instruction::CP_R8(),           // 0xBA
    Instruction::CP_R8(),           // 0xBB
    Instruction::CP_R8(),           // 0xBC
    Instruction::CP_R8(),           // 0xBD
    Instruction::CP_MEM_HL(),       // 0xBE
    Instruction::CP_R8(),           // 0xBF
    Instruction::RET_CC(),          // 0xC0
    Instruction::POP_R16(),         // 0xC1
    Instruction::JP_CC_IMM16(),     // 0xC2
    Instruction::JP_IMM16(),        // 0xC3
    Instruction::CALL_CC_IMM16(),   // 0xC4
    Instruction::PUSH_R16(),        // 0xC5
    Instruction::ADD_IMM8(),        // 0xC6
    Instruction::RST_VEC(),         // 0xC7
    Instruction::RET_CC(),          // 0xC8
    Instruction::RET(),             // 0xC9
    Instruction::JP_CC_IMM16(),     // 0xCA
    Instruction::PREFIX(),          // 0xCB
    Instruction::CALL_CC_IMM16(),   // 0xCC
    Instruction::CALL_IMM16(),      // 0xCD
    Instruction::ADC_IMM8(),        // 0xCE
    Instruction::RST_VEC(),         // 0xCF
    Instruction::RET_CC(),          // 0xD0
    Instruction::POP_R16(),         // 0xD1
    Instruction::JP_CC_IMM16(),     // 0xD2
    {/* ILLEGAL */},                // 0xD3
    Instruction::CALL_CC_IMM16(),   // 0xD4
    Instruction::PUSH_R16(),        // 0xD5
    Instruction::SUB_IMM8(),        // 0xD6
    Instruction::RST_VEC(),         // 0xD7
    Instruction::RET_CC(),          // 0xD8
    {/* RETI */},                   // 0xD9
    Instruction::JP_CC_IMM16(),     // 0xDA
    {/* ILLEGAL */},                // 0xDB
    Instruction::CALL_CC_IMM16(),   // 0xDC
    {/* ILLEGAL */},                // 0xDD
    Instruction::SBC_IMM8(),        // 0xDE
    Instruction::RST_VEC(),         // 0xDF
    Instruction::LDH_MEM_16_A(),    // 0xE0
    Instruction::POP_R16(),         // 0xE1
    Instruction::LDH_MEM_C_A(),     // 0xE2
    {/* ILLEGAL */},                // 0xE3
    {/* ILLEGAL */},                // 0xE4
    Instruction::PUSH_R16(),        // 0xE5
    Instruction::AND_IMM8(),        // 0xE6
    Instruction::RST_VEC(),         // 0xE7
    Instruction::ADD_SP_IMM8(),     // 0xE8
    Instruction::JP_HL(),           // 0xE9
    {/* LD (u16), A */},            // 0xEA
    {/* ILLEGAL */},                // 0xEB
    {/* ILLEGAL */},                // 0xEC
    {/* ILLEGAL */},                // 0xED
    Instruction::XOR_IMM8(),        // 0xEE
    Instruction::RST_VEC(),         // 0xEF
    Instruction::LDH_A_MEM_16(),    // 0xF0
    Instruction::POP_R16(),         // 0xF1
    Instruction::LDH_A_MEM_C(),     // 0xF2
    {/* DI */},                     // 0xF3
    {/* ILLEGAL */},                // 0xF4
    Instruction::PUSH_R16(),        // 0xF5
    Instruction::OR_IMM8(),         // 0xF6
    Instruction::RST_VEC(),         // 0xF7
    {},                             // 0xF8
    {},                             // 0xF9
    {},                             // 0xFA
    {},                             // 0xFB
    {},                             // 0xFC
    {},                             // 0xFD
    Instruction::CP_IMM8(),         // 0xFE
    Instruction::RST_VEC(),         // 0xFF
}};

const CPU::InstructionLookupTable CPU::cb_prefixed_inst_lookup{{
    Instruction::RLC_R8(),       // 0x00
    Instruction::RLC_R8(),       // 0x01
    Instruction::RLC_R8(),       // 0x02
    Instruction::RLC_R8(),       // 0x03
    Instruction::RLC_R8(),       // 0x04
    Instruction::RLC_R8(),       // 0x05
    Instruction::RLC_MEM_HL(),   // 0x06
    Instruction::RLC_R8(),       // 0x07
    Instruction::RRC_R8(),       // 0x08
    Instruction::RRC_R8(),       // 0x0a
    Instruction::RRC_R8(),       // 0x0A
    Instruction::RRC_R8(),       // 0x0B
    Instruction::RRC_R8(),       // 0x0C
    Instruction::RRC_R8(),       // 0x0D
    Instruction::RRC_MEM_HL(),   // 0x0E
    Instruction::RRC_R8(),       // 0x0F
    Instruction::RL_R8(),        // 0x10
    Instruction::RL_R8(),        // 0x11
    Instruction::RL_R8(),        // 0x12
    Instruction::RL_R8(),        // 0x13
    Instruction::RL_R8(),        // 0x14
    Instruction::RL_R8(),        // 0x15
    Instruction::RL_MEM_HL(),    // 0x16
    Instruction::RL_R8(),        // 0x17
    Instruction::RR_R8(),        // 0x18
    Instruction::RR_R8(),        // 0x19
    Instruction::RR_R8(),        // 0x1A
    Instruction::RR_R8(),        // 0x1B
    Instruction::RR_R8(),        // 0x1C
    Instruction::RR_R8(),        // 0x1D
    Instruction::RR_MEM_HL(),    // 0x1E
    Instruction::RR_R8(),        // 0x1F
    Instruction::SLA_R8(),       // 0x20
    Instruction::SLA_R8(),       // 0x22
    Instruction::SLA_R8(),       // 0x22
    Instruction::SLA_R8(),       // 0x23
    Instruction::SLA_R8(),       // 0x24
    Instruction::SLA_R8(),       // 0x25
    Instruction::SLA_MEM_HL(),   // 0x26
    Instruction::SLA_R8(),       // 0x27
    Instruction::SRA_R8(),       // 0x28
    Instruction::SRA_R8(),       // 0x29
    Instruction::SRA_R8(),       // 0x2A
    Instruction::SRA_R8(),       // 0x2B
    Instruction::SRA_R8(),       // 0x2C
    Instruction::SRA_R8(),       // 0x2D
    Instruction::SRA_MEM_HL(),   // 0x2E
    Instruction::SRA_R8(),       // 0x2F
    Instruction::SWAP_R8(),      // 0x30
    Instruction::SWAP_R8(),      // 0x31
    Instruction::SWAP_R8(),      // 0x32
    Instruction::SWAP_R8(),      // 0x33
    Instruction::SWAP_R8(),      // 0x34
    Instruction::SWAP_R8(),      // 0x35
    Instruction::SWAP_MEM_HL(),  // 0x36
    Instruction::SWAP_R8(),      // 0x37
    Instruction::SRL_R8(),       // 0x38
    Instruction::SRL_R8(),       // 0x39
    Instruction::SRL_R8(),       // 0x3A
    Instruction::SRL_R8(),       // 0x3B
    Instruction::SRL_R8(),       // 0x3C
    Instruction::SRL_R8(),       // 0x3D
    Instruction::SRL_MEM_HL(),   // 0x3E
    Instruction::SRL_R8(),       // 0x3F
    Instruction::BIT_R8(),       // 0x40
    Instruction::BIT_R8(),       // 0x41
    Instruction::BIT_R8(),       // 0x42
    Instruction::BIT_R8(),       // 0x43
    Instruction::BIT_R8(),       // 0x44
    Instruction::BIT_R8(),       // 0x45
    Instruction::BIT_MEM_HL(),   // 0x46
    Instruction::BIT_R8(),       // 0x47
    Instruction::BIT_R8(),       // 0x48
    Instruction::BIT_R8(),       // 0x49
    Instruction::BIT_R8(),       // 0x4A
    Instruction::BIT_R8(),       // 0x4B
    Instruction::BIT_R8(),       // 0x4C
    Instruction::BIT_R8(),       // 0x4D
    Instruction::BIT_MEM_HL(),   // 0x4E
    Instruction::BIT_R8(),       // 0x4F
    Instruction::BIT_R8(),       // 0x50
    Instruction::BIT_R8(),       // 0x51
    Instruction::BIT_R8(),       // 0x52
    Instruction::BIT_R8(),       // 0x53
    Instruction::BIT_R8(),       // 0x54
    Instruction::BIT_R8(),       // 0x55
    Instruction::BIT_MEM_HL(),   // 0x56
    Instruction::BIT_R8(),       // 0x57
    Instruction::BIT_R8(),       // 0x58
    Instruction::BIT_R8(),       // 0x59
    Instruction::BIT_R8(),       // 0x5A
    Instruction::BIT_R8(),       // 0x5B
    Instruction::BIT_R8(),       // 0x5C
    Instruction::BIT_R8(),       // 0x5D
    Instruction::BIT_MEM_HL(),   // 0x5E
    Instruction::BIT_R8(),       // 0x5F
    Instruction::BIT_R8(),       // 0x60
    Instruction::BIT_R8(),       // 0x61
    Instruction::BIT_R8(),       // 0x62
    Instruction::BIT_R8(),       // 0x63
    Instruction::BIT_R8(),       // 0x64
    Instruction::BIT_R8(),       // 0x65
    Instruction::BIT_MEM_HL(),   // 0x66
    Instruction::BIT_R8(),       // 0x67
    Instruction::BIT_R8(),       // 0x68
    Instruction::BIT_R8(),       // 0x69
    Instruction::BIT_R8(),       // 0x6A
    Instruction::BIT_R8(),       // 0x6B
    Instruction::BIT_R8(),       // 0x6C
    Instruction::BIT_R8(),       // 0x6D
    Instruction::BIT_MEM_HL(),   // 0x6E
    Instruction::BIT_R8(),       // 0x6F
    Instruction::BIT_R8(),       // 0x70
    Instruction::BIT_R8(),       // 0x71
    Instruction::BIT_R8(),       // 0x72
    Instruction::BIT_R8(),       // 0x73
    Instruction::BIT_R8(),       // 0x74
    Instruction::BIT_R8(),       // 0x75
    Instruction::BIT_MEM_HL(),   // 0x76
    Instruction::BIT_R8(),       // 0x77
    Instruction::BIT_R8(),       // 0x78
    Instruction::BIT_R8(),       // 0x79
    Instruction::BIT_R8(),       // 0x7A
    Instruction::BIT_R8(),       // 0x7B
    Instruction::BIT_R8(),       // 0x7C
    Instruction::BIT_R8(),       // 0x7D
    Instruction::BIT_MEM_HL(),   // 0x7E
    Instruction::BIT_R8(),       // 0x7F
    Instruction::RES_R8(),       // 0x80
    Instruction::RES_R8(),       // 0x81
    Instruction::RES_R8(),       // 0x82
    Instruction::RES_R8(),       // 0x83
    Instruction::RES_R8(),       // 0x84
    Instruction::RES_R8(),       // 0x85
    Instruction::RES_MEM_HL(),   // 0x86
    Instruction::RES_R8(),       // 0x87
    Instruction::RES_R8(),       // 0x88
    Instruction::RES_R8(),       // 0x89
    Instruction::RES_R8(),       // 0x8A
    Instruction::RES_R8(),       // 0x8B
    Instruction::RES_R8(),       // 0x8C
    Instruction::RES_R8(),       // 0x8D
    Instruction::RES_MEM_HL(),   // 0x8E
    Instruction::RES_R8(),       // 0x8F
    Instruction::RES_R8(),       // 0x90
    Instruction::RES_R8(),       // 0x91
    Instruction::RES_R8(),       // 0x92
    Instruction::RES_R8(),       // 0x93
    Instruction::RES_R8(),       // 0x94
    Instruction::RES_R8(),       // 0x95
    Instruction::RES_MEM_HL(),   // 0x96
    Instruction::RES_R8(),       // 0x97
    Instruction::RES_R8(),       // 0x98
    Instruction::RES_R8(),       // 0x99
    Instruction::RES_R8(),       // 0x9A
    Instruction::RES_R8(),       // 0x9B
    Instruction::RES_R8(),       // 0x9C
    Instruction::RES_R8(),       // 0x9D
    Instruction::RES_MEM_HL(),   // 0x9E
    Instruction::RES_R8(),       // 0x9F
    Instruction::RES_R8(),       // 0xA0
    Instruction::RES_R8(),       // 0xA1
    Instruction::RES_R8(),       // 0xA2
    Instruction::RES_R8(),       // 0xA3
    Instruction::RES_R8(),       // 0xA4
    Instruction::RES_R8(),       // 0xA5
    Instruction::RES_MEM_HL(),   // 0xA6
    Instruction::RES_R8(),       // 0xA7
    Instruction::RES_R8(),       // 0xA8
    Instruction::RES_R8(),       // 0xA9
    Instruction::RES_R8(),       // 0xAA
    Instruction::RES_R8(),       // 0xAB
    Instruction::RES_R8(),       // 0xAC
    Instruction::RES_R8(),       // 0xAD
    Instruction::RES_MEM_HL(),   // 0xAE
    Instruction::RES_R8(),       // 0xAF
    Instruction::RES_R8(),       // 0xB0
    Instruction::RES_R8(),       // 0xB1
    Instruction::RES_R8(),       // 0xB2
    Instruction::RES_R8(),       // 0xB3
    Instruction::RES_R8(),       // 0xB4
    Instruction::RES_R8(),       // 0xB5
    Instruction::RES_MEM_HL(),   // 0xB6
    Instruction::RES_R8(),       // 0xB7
    Instruction::RES_R8(),       // 0xB8
    Instruction::RES_R8(),       // 0xB9
    Instruction::RES_R8(),       // 0xBA
    Instruction::RES_R8(),       // 0xBB
    Instruction::RES_R8(),       // 0xBC
    Instruction::RES_R8(),       // 0xBD
    Instruction::RES_MEM_HL(),   // 0xBE
    Instruction::RES_R8(),       // 0xBF
    Instruction::SET_R8(),       // 0xC0
    Instruction::SET_R8(),       // 0xC1
    Instruction::SET_R8(),       // 0xC2
    Instruction::SET_R8(),       // 0xC3
    Instruction::SET_R8(),       // 0xC4
    Instruction::SET_R8(),       // 0xC5
    Instruction::SET_MEM_HL(),   // 0xC6
    Instruction::SET_R8(),       // 0xC7
    Instruction::SET_R8(),       // 0xC8
    Instruction::SET_R8(),       // 0xC9
    Instruction::SET_R8(),       // 0xCA
    Instruction::SET_R8(),       // 0xCB
    Instruction::SET_R8(),       // 0xCC
    Instruction::SET_R8(),       // 0xCD
    Instruction::SET_MEM_HL(),   // 0xCE
    Instruction::SET_R8(),       // 0xCF
    Instruction::SET_R8(),       // 0xD0
    Instruction::SET_R8(),       // 0xD1
    Instruction::SET_R8(),       // 0xD2
    Instruction::SET_R8(),       // 0xD3
    Instruction::SET_R8(),       // 0xD4
    Instruction::SET_R8(),       // 0xD5
    Instruction::SET_MEM_HL(),   // 0xD6
    Instruction::SET_R8(),       // 0xD7
    Instruction::SET_R8(),       // 0xD8
    Instruction::SET_R8(),       // 0xD9
    Instruction::SET_R8(),       // 0xDA
    Instruction::SET_R8(),       // 0xDB
    Instruction::SET_R8(),       // 0xDC
    Instruction::SET_R8(),       // 0xDD
    Instruction::SET_MEM_HL(),   // 0xDE
    Instruction::SET_R8(),       // 0xDF
    Instruction::SET_R8(),       // 0xE0
    Instruction::SET_R8(),       // 0xE1
    Instruction::SET_R8(),       // 0xE2
    Instruction::SET_R8(),       // 0xE3
    Instruction::SET_R8(),       // 0xE4
    Instruction::SET_R8(),       // 0xE5
    Instruction::SET_MEM_HL(),   // 0xE6
    Instruction::SET_R8(),       // 0xE7
    Instruction::SET_R8(),       // 0xE8
    Instruction::SET_R8(),       // 0xE9
    Instruction::SET_R8(),       // 0xEA
    Instruction::SET_R8(),       // 0xEB
    Instruction::SET_R8(),       // 0xEC
    Instruction::SET_R8(),       // 0xED
    Instruction::SET_MEM_HL(),   // 0xEE
    Instruction::SET_R8(),       // 0xEF
    Instruction::SET_R8(),       // 0xF0
    Instruction::SET_R8(),       // 0xF1
    Instruction::SET_R8(),       // 0xF2
    Instruction::SET_R8(),       // 0xF3
    Instruction::SET_R8(),       // 0xF4
    Instruction::SET_R8(),       // 0xF5
    Instruction::SET_MEM_HL(),   // 0xF6
    Instruction::SET_R8(),       // 0xF7
    Instruction::SET_R8(),       // 0xF8
    Instruction::SET_R8(),       // 0xF9
    Instruction::SET_R8(),       // 0xFA
    Instruction::SET_R8(),       // 0xFB
    Instruction::SET_R8(),       // 0xFC
    Instruction::SET_R8(),       // 0xFD
    Instruction::SET_MEM_HL(),   // 0xFE
    Instruction::SET_R8(),       // 0xFF
}};

constexpr CPU::Instruction::Instruction() : format("ILL"), op(&CPU::ILL) {}

constexpr CPU::Instruction::Instruction(const std::string_view format, void (CPU::*op)()) : format(format), op(op) {}

constexpr CPU::Instruction CPU::Instruction::NOP()
{
    return Instruction{"NOP", &CPU::NOP};
}

constexpr CPU::Instruction CPU::Instruction::STOP()
{
    return Instruction{"STOP", &CPU::STOP};
}

constexpr CPU::Instruction CPU::Instruction::HALT()
{
    return Instruction{"HALT", &CPU::HALT};
}

constexpr CPU::Instruction CPU::Instruction::CPL()
{
    return Instruction{"CPL", &CPU::CPL};
}

constexpr CPU::Instruction CPU::Instruction::CCF()
{
    return Instruction{"CCF", &CPU::CCF};
}

constexpr CPU::Instruction CPU::Instruction::DAA()
{
    return Instruction{"DAA", &CPU::DAA};
}

constexpr CPU::Instruction CPU::Instruction::SCF()
{
    return Instruction{"SCF", &CPU::SCF};
}

constexpr CPU::Instruction CPU::Instruction::LD_R8_R8()
{
    return Instruction{"LD {:s}, {:s}", &CPU::LD_R8_R8};
}

constexpr CPU::Instruction CPU::Instruction::LD_R8_IMM8()
{
    return Instruction{"LD {:s}, {:X}h", &CPU::LD_R8_IMM8};
}

constexpr CPU::Instruction CPU::Instruction::LD_R16_IMM16()
{
    return Instruction{"LD {:s}, {:X}h", &CPU::LD_R16_IMM16};
}

constexpr CPU::Instruction CPU::Instruction::LD_R8_MEM_HL()
{
    return Instruction{"LD {:s}, [HL]", &CPU::LD_R8_MEM_HL};
}

constexpr CPU::Instruction CPU::Instruction::LD_MEM_HL_R8()
{
    return Instruction{"LD [HL], {:s}", &CPU::LD_MEM_HL_R8};
}

constexpr CPU::Instruction CPU::Instruction::LD_MEM_HL_IMM8()
{
    return Instruction{"LD [HL], {:02X}h", &CPU::LD_MEM_HL_IMM8};
}

constexpr CPU::Instruction CPU::Instruction::LD_MEM_R16_A()
{
    return Instruction{"LD [{:s}], A", &CPU::LD_MEM_R16_A};
}

constexpr CPU::Instruction CPU::Instruction::LD_MEM_16_SP()
{
    return Instruction{"LD [{:04X}h], SP", &CPU::LD_MEM_16_SP};
}

constexpr CPU::Instruction CPU::Instruction::LD_A_MEM_R16()
{
    return Instruction{"LD A, [{:s}]", &CPU::LD_A_MEM_R16};
}

constexpr CPU::Instruction CPU::Instruction::LDH_MEM_16_A()
{
    return Instruction{"LD [FF00h + {:02X}], A", &CPU::LDH_MEM_16_A};
}

constexpr CPU::Instruction CPU::Instruction::LDH_A_MEM_16()
{
    return Instruction{"LD A, [FF00h + {:02X}h]", &CPU::LDH_MEM_16_A};
}

constexpr CPU::Instruction CPU::Instruction::LDH_MEM_C_A()
{
    return Instruction{"LD [FF00h + C], A", &CPU::LDH_MEM_C_A};
}

constexpr CPU::Instruction CPU::Instruction::LDH_A_MEM_C()
{
    return Instruction{"LD A, [FF00h + C]", &CPU::LDH_A_MEM_C};
}

constexpr CPU::Instruction CPU::Instruction::AND_R8()
{
    return Instruction{"AND {:s}", &CPU::AND_R8};
}

constexpr CPU::Instruction CPU::Instruction::AND_MEM_HL()
{
    return Instruction{"AND [HL]", &CPU::AND_MEM_HL};
}

constexpr CPU::Instruction CPU::Instruction::AND_IMM8()
{
    return Instruction{"AND {:02X}h", &CPU::AND_IMM8};
}

constexpr CPU::Instruction CPU::Instruction::XOR_R8()
{
    return Instruction{"XOR {:s}", &CPU::XOR_R8};
}

constexpr CPU::Instruction CPU::Instruction::XOR_MEM_HL()
{
    return Instruction{"XOR [HL]", &CPU::XOR_MEM_HL};
}

constexpr CPU::Instruction CPU::Instruction::XOR_IMM8()
{
    return Instruction{"XOR {:02X}h", &CPU::XOR_IMM8};
}

constexpr CPU::Instruction CPU::Instruction::OR_R8()
{
    return Instruction{"OR {:s}", &CPU::OR_R8};
}

constexpr CPU::Instruction CPU::Instruction::OR_MEM_HL()
{
    return Instruction{"OR [HL]", &CPU::OR_MEM_HL};
}

constexpr CPU::Instruction CPU::Instruction::OR_IMM8()
{
    return Instruction{"OR {:02X}h", &CPU::OR_IMM8};
}

constexpr CPU::Instruction CPU::Instruction::ILL()
{
    return Instruction{"ILL", &CPU::ILL};
}

constexpr CPU::Instruction CPU::Instruction::PREFIX()
{
    return Instruction{"", &CPU::PREFIX};
}

constexpr CPU::Instruction CPU::Instruction::ADC_R8()
{
    return Instruction{"ADC {:s}", &CPU::ADC_R8};
}

constexpr CPU::Instruction CPU::Instruction::ADC_MEM_HL()
{
    return Instruction{"ADC [HL]", &CPU::ADC_MEM_HL};
}

constexpr CPU::Instruction CPU::Instruction::ADC_IMM8()
{
    return Instruction{"ADC {:02X}h", &CPU::ADC_IMM8};
}

constexpr CPU::Instruction CPU::Instruction::ADD_R8()
{
    return Instruction{"ADD {:s}", &CPU::ADD_R8};
}

constexpr CPU::Instruction CPU::Instruction::ADD_IMM8()
{
    return Instruction{"ADD {:02X}h", &CPU::ADD_IMM8};
}

constexpr CPU::Instruction CPU::Instruction::ADD_MEM_HL()
{
    return Instruction{"ADD [HL]", &CPU::ADD_MEM_HL};
}

constexpr CPU::Instruction CPU::Instruction::ADD_HL_R16()
{
    return Instruction{"ADD HL, {:s}", &CPU::ADD_HL_R16};
}

constexpr CPU::Instruction CPU::Instruction::ADD_SP_IMM8()
{
    return Instruction{"ADD SP, {:02X}h", &CPU::ADD_SP_IMM8};
}

constexpr CPU::Instruction CPU::Instruction::SUB_R8()
{
    return Instruction{"SUB {:s}", &CPU::SUB_R8};
}

constexpr CPU::Instruction CPU::Instruction::SUB_MEM_HL()
{
    return Instruction{"SUB [HL]", &CPU::SUB_MEM_HL};
}

constexpr CPU::Instruction CPU::Instruction::SUB_IMM8()
{
    return Instruction{"SUB {:02X}h", &CPU::SUB_IMM8};
}

constexpr CPU::Instruction CPU::Instruction::SBC_R8()
{
    return Instruction{"SBC {:s}", &CPU::SBC_R8};
}

constexpr CPU::Instruction CPU::Instruction::SBC_MEM_HL()
{
    return Instruction{"SBC [HL]", &CPU::SBC_MEM_HL};
}

constexpr CPU::Instruction CPU::Instruction::SBC_IMM8()
{
    return Instruction{"SBC {:02X}h", &CPU::SBC_IMM8};
}

constexpr CPU::Instruction CPU::Instruction::CP_R8()
{
    return Instruction{"CP {:s}", &CPU::CP_R8};
}

constexpr CPU::Instruction CPU::Instruction::CP_MEM_HL()
{
    return Instruction{"CP [HL]", &CPU::CP_MEM_HL};
}

constexpr CPU::Instruction CPU::Instruction::CP_IMM8()
{
    return Instruction{"CP {:02X}h", &CPU::CP_IMM8};
}

constexpr CPU::Instruction CPU::Instruction::INC_R8()
{
    return Instruction{"INC {:s}", &CPU::INC_R8};
}

constexpr CPU::Instruction CPU::Instruction::DEC_R8()
{
    return Instruction{"DEC {:s}", &CPU::DEC_R8};
}

constexpr CPU::Instruction CPU::Instruction::INC_R16()
{
    return Instruction{"INC {:s}", &CPU::INC_R16};
}

constexpr CPU::Instruction CPU::Instruction::DEC_R16()
{
    return Instruction{"DEC {:s}", &CPU::DEC_R16};
}

constexpr CPU::Instruction CPU::Instruction::INC_MEM_HL()
{
    return Instruction{"INC [HL]", &CPU::INC_MEM_HL};
}

constexpr CPU::Instruction CPU::Instruction::DEC_MEM_HL()
{
    return Instruction{"DEC [HL]", &CPU::DEC_MEM_HL};
}

constexpr CPU::Instruction CPU::Instruction::JP_IMM16()
{
    return Instruction{"JP {:04X}h", &CPU::JP_IMM16};
}

constexpr CPU::Instruction CPU::Instruction::JP_HL()
{
    return Instruction{"JP HL", &CPU::JP_HL};
}

constexpr CPU::Instruction CPU::Instruction::JP_CC_IMM16()
{
    return Instruction{"JP {:s}, {:04X}h", &CPU::JP_CC_IMM16};
}

constexpr CPU::Instruction CPU::Instruction::CALL_IMM16()
{
    return Instruction{"CALL {:04X}h", &CPU::CALL_IMM16};
}

constexpr CPU::Instruction CPU::Instruction::CALL_CC_IMM16()
{
    return Instruction{"CALL {:s}, {:04X}h", &CPU::CALL_CC_IMM16};
}

constexpr CPU::Instruction CPU::Instruction::RET()
{
    return Instruction{"RET", &CPU::RET};
}

constexpr CPU::Instruction CPU::Instruction::RET_CC()
{
    return Instruction{"RET {:s}", &CPU::RET_CC};
}

constexpr CPU::Instruction CPU::Instruction::JR_IMM8()
{
    return Instruction{"JR {:02X}h", &CPU::JR_IMM8};
}

constexpr CPU::Instruction CPU::Instruction::JR_CC_IMM8()
{
    return Instruction{"JR {:s}, {:02X}h", &CPU::JR_CC_IMM8};
}

constexpr CPU::Instruction CPU::Instruction::RST_VEC()
{
    return Instruction{"RST {:02X}h", &CPU::RST_VEC};
}

constexpr CPU::Instruction CPU::Instruction::PUSH_R16()
{
    return Instruction{"PUSH {:s}", &CPU::PUSH_R16};
}

constexpr CPU::Instruction CPU::Instruction::POP_R16()
{
    return Instruction{"POP {:s}", &CPU::POP_R16};
}

constexpr CPU::Instruction CPU::Instruction::RRC_R8()
{
    return Instruction{"RRC {:s}", &CPU::RRC_R8};
}

constexpr CPU::Instruction CPU::Instruction::RRC_MEM_HL()
{
    return Instruction{"RRC [HL]", &CPU::RRC_MEM_HL};
}

constexpr CPU::Instruction CPU::Instruction::RLC_R8()
{
    return Instruction{"RLC {:s}", &CPU::RLC_R8};
}

constexpr CPU::Instruction CPU::Instruction::RLC_MEM_HL()
{
    return Instruction{"RLC [HL]", &CPU::RLC_MEM_HL};
}

constexpr CPU::Instruction CPU::Instruction::RR_R8()
{
    return Instruction{"RR {:s}", &CPU::RR_R8};
}

constexpr CPU::Instruction CPU::Instruction::RR_MEM_HL()
{
    return Instruction{"RR [HL]", &CPU::RR_MEM_HL};
}

constexpr CPU::Instruction CPU::Instruction::RL_R8()
{
    return Instruction{"RL {:s}", &CPU::RL_R8};
}

constexpr CPU::Instruction CPU::Instruction::RLA()
{
    return Instruction{"RLA", &CPU::RLA};
}

constexpr CPU::Instruction CPU::Instruction::RLCA()
{
    return Instruction{"RLCA", &CPU::RLCA};
}

constexpr CPU::Instruction CPU::Instruction::RRA()
{
    return Instruction{"RRA", &CPU::RRA};
}

constexpr CPU::Instruction CPU::Instruction::RRCA()
{
    return Instruction{"RRCA", &CPU::RRCA};
}

constexpr CPU::Instruction CPU::Instruction::RL_MEM_HL()
{
    return Instruction{"RL [HL]", &CPU::RL_MEM_HL};
}

constexpr CPU::Instruction CPU::Instruction::SLA_R8()
{
    return Instruction{"SLA {:s}", &CPU::SLA_R8};
}

constexpr CPU::Instruction CPU::Instruction::SLA_MEM_HL()
{
    return Instruction{"SLA [HL]", &CPU::SLA_MEM_HL};
}

constexpr CPU::Instruction CPU::Instruction::SRL_R8()
{
    return Instruction{"SRL {:s}", &CPU::SRL_R8};
}

constexpr CPU::Instruction CPU::Instruction::SRL_MEM_HL()
{
    return Instruction{"SRL [HL]", &CPU::SRL_R8};
}
constexpr CPU::Instruction CPU::Instruction::SRA_R8()
{
    return Instruction{"SRA {:s}", &CPU::SRA_R8};
}

constexpr CPU::Instruction CPU::Instruction::SRA_MEM_HL()
{
    return Instruction{"SRA [HL]", &CPU::SRA_MEM_HL};
}

constexpr CPU::Instruction CPU::Instruction::SWAP_R8()
{
    return Instruction{"SWAP {:s}", &CPU::SWAP_R8};
}

constexpr CPU::Instruction CPU::Instruction::SWAP_MEM_HL()
{
    return Instruction{"SWAP [HL]", &CPU::SWAP_MEM_HL};
}

constexpr CPU::Instruction CPU::Instruction::BIT_R8()
{
    return Instruction{"BIT {:d}, {:s}", &CPU::BIT_R8};
}

constexpr CPU::Instruction CPU::Instruction::BIT_MEM_HL()
{
    return Instruction{"BIT {:d}, [HL]", &CPU::BIT_MEM_HL};
}

constexpr CPU::Instruction CPU::Instruction::RES_R8()
{
    return Instruction{"RES {:d}, {:s}", &CPU::RES_R8};
}

constexpr CPU::Instruction CPU::Instruction::RES_MEM_HL()
{
    return Instruction{"RES {:d}, [HL]", &CPU::RES_MEM_HL};
}
constexpr CPU::Instruction CPU::Instruction::SET_R8()
{
    return Instruction{"SET {:d}, {:s}", &CPU::SET_R8};
}

constexpr CPU::Instruction CPU::Instruction::SET_MEM_HL()
{
    return Instruction{"SET {:d}, [HL]", &CPU::SET_MEM_HL};
}

const char* CPU::BadRegister::what() const noexcept
{
    return "Bad Register";
}

CPU::CPU(Bus& bus) : reg(), ticks(0), opcode(0), cb_prefixed(false), inst(inst_lookup[0]), bus(bus) {}

CPU::~CPU() = default;

void CPU::NOP()
{
    (void) this->opcode;
}

void CPU::STOP()
{
    throw std::runtime_error("Not implemented");
}

void CPU::HALT()
{
    throw std::runtime_error("Not implemented");
}

void CPU::CPL()
{
    this->reg.u8.A = ~this->reg.u8.A;
    this->set_subtract(true);
    this->set_half_carry(true);
}

void CPU::CCF()
{
    this->set_carry(!this->carry());
    this->set_subtract(false);
    this->set_half_carry(false);
}

void CPU::DAA()
{
    uint8_t adj{};

    if ((!this->subtract() && this->reg.u8.A & 0x0F > 0x09) || this->half_carry())
    {
        adj |= 0x06;
    }
    if ((!this->subtract() && this->reg.u8.A > 0x99) || this->carry())
    {
        adj |= 0x60;
        this->set_carry(true);
    }
    if (!this->subtract())
    {
        this->reg.u8.A += adj;
    }
    else
    {
        this->reg.u8.A -= adj;
    }
    this->set_half_carry(false);
    this->set_zero(this->reg.u8.A == 0);
}

void CPU::SCF()
{
    this->set_carry(true);
    this->set_subtract(false);
    this->set_half_carry(false);
}

void CPU::EI() {}

void CPU::LD_R8_R8()
{
    this->reg.u8.*this->get_register8(Register8Position::LEFTMOST) =
        this->reg.u8.*this->get_register8(Register8Position::RIGHTMOST);
}

void CPU::LD_R8_IMM8()
{
    this->micro_ops.emplace(
        [this]()
        {
            const auto dest{this->get_register8(Register8Position::LEFTMOST)};
            const auto imm{this->bus.read(this->reg.u16.PC++)};

            this->reg.u8.*dest = imm;
        });
}

void CPU::LD_R16_IMM16()
{
    this->micro_ops.emplace([this] { this->tmp.Z = this->bus.read(this->reg.u16.PC++); });
    this->micro_ops.emplace(
        [this]
        {
            const auto dest{this->get_register16()};
            this->tmp.W         = this->bus.read(this->reg.u16.PC++);
            this->reg.u16.*dest = this->tmp.WZ;
        });
}

void CPU::LD_MEM_R16_A()
{
    this->micro_ops.emplace(
        [this]
        {
            const auto [placeholder, dest]{this->get_register16_memory()};
            this->bus.write(this->reg.u16.*dest, this->reg.u8.A);
            if (placeholder == Register16MemoryPlaceholder::HL_PLUS)
            {
                this->reg.u16.HL += 1;
            }
            else if (placeholder == Register16MemoryPlaceholder::HL_MINUS)
            {
                this->reg.u16.HL -= 1;
            }
        });
}

void CPU::LD_A_MEM_R16()
{
    this->micro_ops.emplace(
        [this]
        {
            const auto [placeholder, source]{this->get_register16_memory()};
            this->reg.u8.A = this->bus.read(this->reg.u16.*source);
            if (placeholder == Register16MemoryPlaceholder::HL_PLUS)
            {
                this->reg.u16.HL += 1;
            }
            else if (placeholder == Register16MemoryPlaceholder::HL_MINUS)
            {
                this->reg.u16.HL -= 1;
            }
        });
}

void CPU::LD_R8_MEM_HL()
{
    this->micro_ops.emplace(
        [this] { this->reg.u8.*this->get_register8(Register8Position::LEFTMOST) = this->bus.read(this->reg.u16.HL); });
}

void CPU::LD_MEM_HL_R8()
{
    this->micro_ops.emplace(
        [this] { this->bus.write(this->reg.u16.HL, this->reg.u8.*this->get_register8(Register8Position::RIGHTMOST)); });
}

void CPU::LD_MEM_HL_IMM8()
{
    this->micro_ops.emplace([this] { this->tmp.Z = this->bus.read(this->reg.u16.PC++); });
    this->micro_ops.emplace([this] { this->bus.write(this->reg.u16.HL, this->tmp.Z); });
}

void CPU::LDH_MEM_16_A()
{
    this->micro_ops.emplace([this] { this->tmp.Z = this->bus.read(this->reg.u16.PC++); });
    this->micro_ops.emplace(
        [this]
        {
            this->tmp.W = 0xFF;
            this->bus.write(this->tmp.WZ, this->tmp.Z);
        });
}

void CPU::LDH_A_MEM_16()
{
    this->micro_ops.emplace([this] { this->tmp.Z = this->bus.read(this->reg.u16.PC++); });
    this->micro_ops.emplace(
        [this]
        {
            this->tmp.W    = 0xFF;
            this->reg.u8.A = this->bus.read(this->tmp.WZ);
        });
}

void CPU::LDH_MEM_C_A()
{
    this->micro_ops.emplace(
        [this]
        {
            this->tmp.W = 0xFF;
            this->tmp.Z = this->reg.u8.C;
            this->bus.write(this->tmp.WZ, this->reg.u8.A);
        });
}

void CPU::LDH_A_MEM_C()
{
    this->micro_ops.emplace(
        [this]
        {
            this->tmp.W    = 0xFF;
            this->tmp.Z    = this->reg.u8.C;
            this->reg.u8.A = this->bus.read(this->tmp.WZ);
        });
}

void CPU::LD_MEM_16_SP()
{
    this->micro_ops.emplace([this] { this->tmp.Z = this->bus.read(this->reg.u16.PC++); });
    this->micro_ops.emplace([this] { this->tmp.W = this->bus.read(this->reg.u16.PC++); });
    this->micro_ops.emplace([this] { this->bus.write(this->tmp.WZ, u16_lsb(this->reg.u16.SP)); });
    this->micro_ops.emplace([this] { this->bus.write(this->tmp.WZ + 1, u16_msb(this->reg.u16.SP)); });
}

void CPU::PUSH_R16()
{
    this->micro_ops.emplace([this]() { this->reg.u16.SP--; });
    this->micro_ops.emplace(
        [this]() { this->bus.write(this->reg.u16.SP--, u16_msb(this->reg.u16.*this->get_register16_stack())); });
    this->micro_ops.emplace(
        [this]() { this->bus.write(this->reg.u16.SP, u16_lsb(this->reg.u16.*this->get_register16_stack())); });
}

void CPU::POP_R16()
{
    this->micro_ops.emplace([this]() { this->tmp.Z = this->bus.read(this->reg.u16.SP++); });
    this->micro_ops.emplace(
        [this]()
        {
            this->tmp.W                                 = this->bus.read(this->reg.u16.SP++);
            this->reg.u16.*this->get_register16_stack() = this->tmp.WZ;
        });
}

void CPU::RES_R8()
{
    this->reg.u8.*this->get_register8(Register8Position::RIGHTMOST) &= ~(1 << this->get_b3());
}

void CPU::RES_MEM_HL()
{
    this->micro_ops.emplace([this] { this->tmp.Z = this->bus.read(this->reg.u16.HL); });
    this->micro_ops.emplace(
        [this]
        {
            this->tmp.Z &= ~(1 << this->get_b3());
            this->bus.write(this->reg.u16.HL, this->tmp.Z);
        });
}

void CPU::SET_R8()
{
    const auto src{this->get_register8(Register8Position::RIGHTMOST)};
    this->reg.u8.*src |= 1U << this->get_b3();
}

void CPU::SET_MEM_HL()
{
    this->micro_ops.emplace([this] { this->tmp.Z = this->bus.read(this->reg.u16.HL); });
    this->micro_ops.emplace(
        [this]
        {
            this->tmp.Z |= 1U << this->get_b3();
            this->bus.write(this->reg.u16.HL, this->tmp.Z);
        });
}

void CPU::BITWISE(const BitwiseOperator op, const uint8_t operand)
{
    switch (op)
    {
        case BitwiseOperator::OR:
            this->reg.u8.A |= operand;
            this->set_half_carry(false);
            break;
        case BitwiseOperator::AND:
            this->reg.u8.A &= operand;
            this->set_half_carry(true);
            break;
        case BitwiseOperator::XOR:
            this->reg.u8.A ^= operand;
            this->set_half_carry(false);
            break;
    }

    this->set_zero(this->reg.u8.A == 0);
    this->set_subtract(false);
    this->set_carry(false);
}

void CPU::AND_R8()
{
    this->BITWISE(BitwiseOperator::AND, this->reg.u8.*this->get_register8(Register8Position::RIGHTMOST));
}

void CPU::AND_MEM_HL()
{
    this->micro_ops.emplace([this] { this->BITWISE(BitwiseOperator::AND, this->bus.read(this->reg.u16.HL)); });
}

void CPU::AND_IMM8()
{
    this->micro_ops.emplace([this] { this->BITWISE(BitwiseOperator::AND, this->bus.read(this->reg.u16.PC++)); });
}

void CPU::OR_R8()
{
    this->BITWISE(BitwiseOperator::OR, this->reg.u8.*this->get_register8(Register8Position::RIGHTMOST));
}

void CPU::OR_MEM_HL()
{
    this->micro_ops.emplace([this] { this->BITWISE(BitwiseOperator::OR, this->bus.read(this->reg.u16.HL)); });
}

void CPU::OR_IMM8()
{
    this->micro_ops.emplace([this] { this->BITWISE(BitwiseOperator::OR, this->bus.read(this->reg.u16.PC++)); });
}

void CPU::XOR_R8()
{
    this->BITWISE(BitwiseOperator::XOR, this->reg.u8.*this->get_register8(Register8Position::RIGHTMOST));
}

void CPU::XOR_MEM_HL()
{
    this->micro_ops.emplace([this] { this->BITWISE(BitwiseOperator::XOR, this->bus.read(this->reg.u16.HL)); });
}

void CPU::XOR_IMM8()
{
    this->micro_ops.emplace([this] { this->BITWISE(BitwiseOperator::XOR, this->bus.read(this->reg.u16.PC++)); });
}

uint16_t CPU::ADD_16(const uint16_t a, const uint16_t b)
{
    uint32_t result32{};

    this->set_half_carry(((a & 0x7FF) + (b & 0x7FF)) > 0x800);
    result32 = a + b;
    this->set_carry(result32 > 0xFFFF);
    this->set_subtract(false);

    return static_cast<uint16_t>(result32);
}

uint16_t CPU::ADD_16(const uint16_t a, const int8_t b)
{
    uint32_t result32{};

    this->set_half_carry(((a & 0x7FF) + (b & 0x7FF)) > 0x800);
    result32 = a + b;
    this->set_carry(result32 > 0xFFFF);
    this->set_zero(false);
    this->set_subtract(false);

    return static_cast<uint16_t>(result32);
}

uint8_t CPU::ADD_8(const uint8_t a, const uint8_t b, bool add_carry)
{
    uint16_t result16{};
    uint8_t  result8{};

    if (add_carry)
    {
        add_carry = this->carry();
    }
    this->set_half_carry((a & 0xF) + (b & 0XF) + add_carry > 0xF);
    result16 = a + b + add_carry;
    this->set_carry(result16 > 0xFF);
    result8 = static_cast<uint8_t>(result16);
    this->set_zero(result8 == 0);
    this->set_subtract(false);

    return (result8);
}

void CPU::ADD_R8()
{
    this->reg.u8.A =
        this->ADD_8(this->reg.u8.A, this->reg.u8.*this->get_register8(Register8Position::RIGHTMOST), false);
}

void CPU::ADD_MEM_HL()
{
    this->micro_ops.emplace(
        [this]
        { this->bus.write(this->reg.u16.HL, this->ADD_8(this->reg.u8.A, this->bus.read(this->reg.u16.HL), false)); });
}

void CPU::ADD_IMM8()
{
    this->micro_ops.emplace(
        [this] { this->reg.u8.A = this->ADD_8(this->reg.u8.A, this->bus.read(this->reg.u16.PC++), false); });
}

void CPU::ADD_HL_R16()
{
    const auto result{this->ADD_16(this->reg.u16.HL, this->reg.u16.*this->get_register16())};
    this->reg.u8.L = u16_lsb(result);
    this->micro_ops.emplace([this, result] { this->reg.u8.H = u16_msb(result); });
}

void CPU::ADD_SP_IMM8()
{
    uint16_t result{};

    this->micro_ops.emplace([this] { this->tmp.Z = this->bus.read(this->reg.u16.PC++); });
    this->micro_ops.emplace(
        [this, &result]
        {
            result      = this->ADD_16(this->reg.u16.SP, static_cast<int8_t>(this->tmp.Z));
            this->tmp.Z = u16_lsb(result);
        });
    this->micro_ops.emplace(
        [this, result]
        {
            this->tmp.W      = u16_msb(result);
            this->reg.u16.SP = this->tmp.WZ;
        });
}

void CPU::ADC_R8()
{
    this->reg.u8.A = this->ADD_8(this->reg.u8.A, this->reg.u8.*this->get_register8(Register8Position::RIGHTMOST), true);
}

void CPU::ADC_MEM_HL()
{
    this->micro_ops.emplace(
        [this]
        { this->bus.write(this->reg.u16.HL, this->ADD_8(this->reg.u8.A, this->bus.read(this->reg.u16.HL), true)); });
}

void CPU::ADC_IMM8()
{
    this->micro_ops.emplace(
        [this]
        { this->bus.write(this->reg.u16.HL, this->ADD_8(this->reg.u8.A, this->bus.read(this->reg.u16.PC++), true)); });
}

uint8_t CPU::SUB_8(const uint8_t a, const uint8_t b, bool sub_carry)
{
    uint8_t result{};

    if (sub_carry)
    {
        sub_carry = this->carry();
    }
    this->set_subtract(true);
    this->set_half_carry((a & 0xF) < (b & 0xF) + sub_carry);
    this->set_carry(a < b + sub_carry);

    result = a - b - sub_carry;

    return result;
}

void CPU::SUB_R8()
{
    this->reg.u8.A =
        this->SUB_8(this->reg.u8.A, this->reg.u8.*this->get_register8(Register8Position::RIGHTMOST), false);
}

void CPU::SUB_MEM_HL()
{
    this->micro_ops.emplace([this]
                            { this->reg.u8.A = this->SUB_8(this->reg.u8.A, this->bus.read(this->reg.u16.HL), false); });
}

void CPU::SUB_IMM8()
{
    this->micro_ops.emplace(
        [this] { this->reg.u8.A = this->SUB_8(this->reg.u8.A, this->bus.read(this->reg.u16.PC++), false); });
}

void CPU::SBC_R8()
{
    this->reg.u8.A = this->SUB_8(this->reg.u8.A, this->reg.u8.*this->get_register8(Register8Position::RIGHTMOST), true);
}

void CPU::SBC_MEM_HL()
{
    this->micro_ops.emplace([this]
                            { this->reg.u8.A = this->SUB_8(this->reg.u8.A, this->bus.read(this->reg.u16.HL), true); });
}

void CPU::SBC_IMM8()
{
    this->micro_ops.emplace(
        [this] { this->reg.u8.A = this->SUB_8(this->reg.u8.A, this->bus.read(this->reg.u16.PC++), true); });
}

void CPU::CP_R8()
{
    (void) this->SUB_8(this->reg.u8.A, this->reg.u8.*this->get_register8(Register8Position::RIGHTMOST), false);
}

void CPU::CP_MEM_HL()
{
    this->micro_ops.emplace([this] { (void) this->SUB_8(this->reg.u8.A, this->bus.read(this->reg.u16.HL), false); });
}

void CPU::CP_IMM8()
{
    this->micro_ops.emplace([this] { (void) this->SUB_8(this->reg.u8.A, this->bus.read(this->reg.u16.PC++), false); });
}

uint8_t CPU::STEP_IMM8(uint8_t value, const StepType type)
{
    if (type == StepType::INCREMENT)
    {
        value += 1;
    }
    else
    {
        value -= 1;
    }
    this->set_subtract(type == StepType::DECREMENT);
    this->set_zero(value == 0);
    this->set_half_carry(value & 0b00010000 != 0);
    return value;
}

void CPU::INC_R8()
{
    const auto operand    = this->get_register8(Register8Position::LEFTMOST);
    this->reg.u8.*operand = this->STEP_IMM8(this->reg.u8.*operand, StepType::INCREMENT);
}

void CPU::DEC_R8()
{
    const auto operand    = this->get_register8(Register8Position::LEFTMOST);
    this->reg.u8.*operand = this->STEP_IMM8(this->reg.u8.*operand, StepType::DECREMENT);
}

void CPU::INC_R16()
{
    this->micro_ops.emplace([this] { this->reg.u16.*this->get_register16() += 1; });
}

void CPU::DEC_R16()
{
    this->micro_ops.emplace([this] { this->reg.u16.*this->get_register16() -= 1; });
}

void CPU::INC_MEM_HL()
{
    this->micro_ops.emplace([this] { this->tmp.Z = this->bus.read(this->reg.u16.HL); });
    this->micro_ops.emplace([this]
                            { this->bus.write(this->reg.u16.HL, this->STEP_IMM8(this->tmp.Z, StepType::INCREMENT)); });
}

void CPU::DEC_MEM_HL()
{
    this->micro_ops.emplace([this] { this->tmp.Z = this->bus.read(this->reg.u16.HL); });
    this->micro_ops.emplace([this]
                            { this->bus.write(this->reg.u16.HL, this->STEP_IMM8(this->tmp.Z, StepType::DECREMENT)); });
}

void CPU::ILL()
{
    throw IllegalInstruction(this->opcode, this->reg.u16.PC);
}

void CPU::DI() {}

void CPU::PREFIX()
{
    this->cb_prefixed = true;
}

void CPU::JP_IMM16()
{
    this->micro_ops.emplace([this]() { this->tmp.Z = this->bus.read(this->reg.u16.PC++); });
    this->micro_ops.emplace([this]() { this->tmp.W = this->bus.read(this->reg.u16.PC++); });
    this->micro_ops.emplace([this]() { this->reg.u16.PC = this->tmp.WZ; });
}

void CPU::JP_HL()
{
    this->reg.u16.PC = this->reg.u16.HL;
}

void CPU::JP_CC_IMM16()
{
    this->micro_ops.emplace([this]() { this->tmp.Z = this->bus.read(this->reg.u16.PC++); });
    this->micro_ops.emplace(
        [this]()
        {
            this->tmp.W = this->bus.read(this->reg.u16.PC++);
            if (check_condition_is_met())
            {
                this->micro_ops.emplace([this]() { this->reg.u16.PC = this->tmp.WZ; });
            }
        });
}

void CPU::CALL_IMM16()
{
    this->micro_ops.emplace([this]() { this->tmp.Z = this->bus.read(this->reg.u16.PC++); });
    this->micro_ops.emplace([this]() { this->tmp.W = this->bus.read(this->reg.u16.PC++); });
    this->micro_ops.emplace([this]() { this->reg.u16.SP--; });
    this->micro_ops.emplace([this]() { this->bus.write(this->reg.u16.SP--, u16_msb(this->reg.u16.PC)); });
    this->micro_ops.emplace(
        [this]()
        {
            this->bus.write(this->reg.u16.SP, u16_lsb(this->reg.u16.PC));
            this->reg.u16.PC = this->tmp.WZ;
        });
}

void CPU::CALL_CC_IMM16()
{
    this->micro_ops.emplace([this] { this->tmp.Z = this->bus.read(this->reg.u16.PC++); });
    this->micro_ops.emplace(
        [this]()
        {
            this->tmp.W = this->bus.read(this->reg.u16.PC++);
            if (check_condition_is_met())
            {
                this->micro_ops.emplace([this] { this->reg.u16.SP--; });
                this->micro_ops.emplace([this] { this->bus.write(this->reg.u16.SP--, u16_msb(this->reg.u16.PC)); });
                this->micro_ops.emplace(
                    [this]
                    {
                        this->bus.write(this->reg.u16.SP, u16_lsb(this->reg.u16.PC));
                        this->reg.u16.PC = this->tmp.WZ;
                    });
            }
        });
}

void CPU::JR_IMM8()
{
    this->micro_ops.emplace([this] { this->tmp.Z = this->bus.read(this->reg.u16.PC++); });
    this->micro_ops.emplace([this] { this->reg.u16.PC += static_cast<int8_t>(this->tmp.Z); });
}

void CPU::JR_CC_IMM8()
{
    this->micro_ops.emplace(
        [this]
        {
            this->tmp.Z = this->bus.read(this->reg.u16.PC++);
            if (this->check_condition_is_met())
            {
                this->micro_ops.emplace([this] { this->reg.u16.PC += static_cast<int8_t>(this->tmp.Z); });
            }
        });
}

void CPU::RET()
{
    this->micro_ops.emplace([this] { this->tmp.Z = this->bus.read(this->reg.u16.SP++); });
    this->micro_ops.emplace([this] { this->tmp.W = this->bus.read(this->reg.u16.SP++); });
    this->micro_ops.emplace([this] { this->reg.u16.PC = this->tmp.WZ; });
}

void CPU::RET_CC()
{
    this->micro_ops.emplace(
        [this]
        {
            if (this->check_condition_is_met())
            {
                this->RET();
            }
        });
}

void CPU::RST_VEC()
{
    this->micro_ops.emplace([this]() { this->reg.u16.SP--; });
    this->micro_ops.emplace([this]() { this->bus.write(this->reg.u16.SP--, u16_msb(this->reg.u16.PC)); });
    this->micro_ops.emplace(
        [this]()
        {
            this->bus.write(this->reg.u16.SP, u16_lsb(this->reg.u16.PC));
            this->reg.u16.PC = static_cast<uint16_t>(this->get_tgt3());
        });
}

auto CPU::ROTATE(uint8_t val, const RotateDirection rotate_direction, const bool rotate_through_carry) noexcept
{
    auto has_new_carry = false;

    if (rotate_direction == RotateDirection::LEFT)
    {
        has_new_carry = (val & 0x80) != 0;
        val <<= 1;
        if (rotate_through_carry)
        {
            val |= (this->carry() ? 0x01 : 0x00);
        }
    }
    else
    {
        has_new_carry = (val & 0x01) != 0;
        val >>= 1;
        if (rotate_through_carry)
        {
            val |= (this->carry() ? 0x80 : 0x00);
        }
    }
    this->set_zero(val == 0);
    this->set_carry(has_new_carry);
    this->set_half_carry(false);
    this->set_subtract(false);
    return (val);
}

void CPU::RLC_R8()
{
    const auto operand{this->get_register8(Register8Position::RIGHTMOST)};
    this->reg.u8.*operand = this->ROTATE(this->reg.u8.*operand, RotateDirection::LEFT, false);
}

void CPU::RLC_MEM_HL()
{
    this->micro_ops.emplace([this] { this->tmp.Z = this->bus.read(this->reg.u16.HL); });
    this->micro_ops.emplace(
        [this]
        {
            this->tmp.Z = this->ROTATE(this->tmp.Z, RotateDirection::LEFT, false);
            this->bus.write(this->reg.u16.HL, this->tmp.Z);
        });
}

void CPU::RRC_R8()
{
    const auto operand{this->get_register8(Register8Position::RIGHTMOST)};
    this->reg.u8.*operand = this->ROTATE(this->reg.u8.*operand, RotateDirection::RIGHT, false);
}

void CPU::RRC_MEM_HL()
{
    this->micro_ops.emplace([this] { this->tmp.Z = this->bus.read(this->reg.u16.HL); });
    this->micro_ops.emplace(
        [this]
        {
            this->tmp.Z = this->ROTATE(this->tmp.Z, RotateDirection::RIGHT, false);
            this->bus.write(this->reg.u16.HL, this->tmp.Z);
        });
}

void CPU::RL_R8()
{
    const auto operand    = this->get_register8(Register8Position::RIGHTMOST);
    this->reg.u8.*operand = this->ROTATE(this->reg.u8.*operand, RotateDirection::LEFT, true);
}

void CPU::RLA()
{
    this->reg.u8.A = this->ROTATE(this->reg.u8.A, RotateDirection::LEFT, true);
    this->set_zero(false);
}

void CPU::RLCA()
{
    this->reg.u8.A = this->ROTATE(this->reg.u8.A, RotateDirection::LEFT, false);
    this->set_zero(false);
}

void CPU::RRCA()
{
    this->reg.u8.A = this->ROTATE(this->reg.u8.A, RotateDirection::RIGHT, false);
    this->set_zero(false);
}

void CPU::RRA()
{
    this->reg.u8.A = this->ROTATE(this->reg.u8.A, RotateDirection::RIGHT, true);
    this->set_zero(false);
}

void CPU::RL_MEM_HL()
{
    this->micro_ops.emplace([this] { this->tmp.Z = this->bus.read(this->reg.u16.HL); });
    this->micro_ops.emplace(
        [this]
        {
            this->tmp.Z = this->ROTATE(this->tmp.Z, RotateDirection::LEFT, true);
            this->bus.write(this->reg.u16.HL, this->tmp.Z);
        });
}

void CPU::RR_R8()
{
    const auto operand{this->get_register8(Register8Position::RIGHTMOST)};
    this->reg.u8.*operand = this->ROTATE(this->reg.u8.*operand, RotateDirection::RIGHT, true);
}

void CPU::RR_MEM_HL()
{
    this->micro_ops.emplace([this] { this->tmp.Z = this->bus.read(this->reg.u16.HL); });
    this->micro_ops.emplace(
        [this]
        {
            this->tmp.Z = this->ROTATE(this->tmp.Z, RotateDirection::RIGHT, true);
            this->bus.write(this->reg.u16.HL, this->tmp.Z);
        });
}

auto CPU::SHIFT(uint8_t val, const ShiftType shift_type, const ShiftDirection shift_direction) noexcept
{
    auto sign_bit = false;

    this->set_carry((val & (shift_direction == ShiftDirection::RIGHT ? 0x01 : 0x80)) != 0);
    sign_bit = (val & 0x80) != 0;
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
        val |= 0x80;
    }
    this->set_zero(val == 0);
    this->set_half_carry(false);
    this->set_subtract(false);

    return val;
}

void CPU::SRA_R8()
{
    const auto src{this->get_register8(Register8Position::RIGHTMOST)};
    this->reg.u8.*src = this->SHIFT(this->reg.u8.*src, ShiftType::ARITHMETIC, ShiftDirection::RIGHT);
}

void CPU::SRA_MEM_HL()
{
    this->micro_ops.emplace([this] { this->tmp.Z = this->bus.read(this->reg.u16.HL); });
    this->micro_ops.emplace(
        [this]
        {
            this->tmp.Z = this->SHIFT(this->tmp.Z, ShiftType::ARITHMETIC, ShiftDirection::RIGHT);
            this->bus.write(this->reg.u16.HL, this->tmp.Z);
        });
}

void CPU::SLA_R8()
{
    const auto src{this->get_register8(Register8Position::RIGHTMOST)};
    this->reg.u8.*src = this->SHIFT(this->reg.u8.*src, ShiftType::ARITHMETIC, ShiftDirection::LEFT);
}

void CPU::SLA_MEM_HL()
{
    this->micro_ops.emplace([this] { this->tmp.Z = this->bus.read(this->reg.u16.HL); });
    this->micro_ops.emplace(
        [this]
        {
            this->tmp.Z = this->SHIFT(this->tmp.Z, ShiftType::ARITHMETIC, ShiftDirection::LEFT);
            this->bus.write(this->reg.u16.HL, this->tmp.Z);
        });
}

void CPU::SRL_R8()
{
    const auto src{this->get_register8(Register8Position::RIGHTMOST)};
    this->reg.u8.*src = this->SHIFT(this->reg.u8.*src, ShiftType::LOGICAL, ShiftDirection::RIGHT);
}

void CPU::SRL_MEM_HL()
{
    this->micro_ops.emplace([this] { this->tmp.Z = this->bus.read(this->reg.u16.HL); });
    this->micro_ops.emplace(
        [this]
        {
            this->tmp.Z = this->SHIFT(this->tmp.Z, ShiftType::LOGICAL, ShiftDirection::RIGHT);
            this->bus.write(this->reg.u16.HL, this->tmp.Z);
        });
}

auto CPU::SWAP(uint8_t val)
{
    val = (val & 0b11110000) >> 4 | (val & 0b00001111) << 4;

    this->set_zero(val == 0);
    this->set_subtract(false);
    this->set_half_carry(false);
    this->set_carry(false);

    return (val);
}

void CPU::SWAP_R8()
{
    const auto src{this->get_register8(Register8Position::RIGHTMOST)};
    this->reg.u8.*src = this->SWAP(this->reg.u8.*src);
}

void CPU::SWAP_MEM_HL()
{
    this->micro_ops.emplace([this] { this->tmp.Z = this->bus.read(this->reg.u16.HL); });
    this->micro_ops.emplace(
        [this]
        {
            this->tmp.Z = this->SWAP(this->tmp.Z);
            this->bus.write(this->reg.u16.HL, this->tmp.Z);
        });
}

void CPU::BIT(const uint8_t val, const uint8_t bit)
{
    this->set_zero((val & (1U << bit)) == 0);
    this->set_subtract(false);
    this->set_half_carry(true);
}

void CPU::BIT_MEM_HL()
{
    this->micro_ops.emplace(
        [this]
        {
            this->tmp.Z = this->bus.read(this->reg.u16.HL);
            this->BIT(this->tmp.Z, this->get_b3());
        });
}

void CPU::BIT_R8()
{
    const auto src{this->get_register8(Register8Position::RIGHTMOST)};
    this->BIT(this->reg.u8.*src, this->get_b3());
}

void CPU::tick()
{
    if (this->ticks++ < 4)
    {
        return;
    }
    this->ticks = 0;
    switch (this->state)
    {
        case CPUState::FETCH_DECODE:
            this->opcode = this->bus.read(this->reg.u16.PC++);
            if (this->cb_prefixed)
            {
                this->inst        = cb_prefixed_inst_lookup.at(opcode);
                this->cb_prefixed = false;
            }
            else
            {
                this->inst = inst_lookup.at(opcode);
            }  // PC:21
            (this->*inst.op)();
            if (!this->micro_ops.empty())
            {
                this->state = CPUState::EXECUTE;
            }
            break;
        case CPUState::EXECUTE:
            this->micro_ops.front()();
            this->micro_ops.pop();
            if (this->micro_ops.empty())
            {
                this->state = CPUState::FETCH_DECODE;
            }
            break;
    };
}

CPU::Register CPU::get_register() const noexcept
{
    return this->reg;
}

uint8_t CPU::get_b3() const
{
    return (this->opcode >> 3) & 0b00000111;
}

uint8_t CPU::get_tgt3() const
{
    return (this->opcode & 0b00111000);
}

CPU::Register8 CPU::get_register8(const Register8Position shift) const
{
    switch (static_cast<Register8Placeholder>((this->opcode >> static_cast<uint8_t>(shift)) & 0b00000111U))
    {
        case Register8Placeholder::A:
            return &Register::U8::A;
        case Register8Placeholder::B:
            return &Register::U8::B;
        case Register8Placeholder::C:
            return &Register::U8::C;
        case Register8Placeholder::D:
            return &Register::U8::D;
        case Register8Placeholder::E:
            return &Register::U8::E;
        case Register8Placeholder::H:
            return &Register::U8::H;
        case Register8Placeholder::L:
            return &Register::U8::L;
        default:
            throw BadRegister();
    }
}

CPU::Register16 CPU::get_register16() const
{
    switch (static_cast<Register16Placeholder>((this->opcode & 0b00110000U) >> 4))
    {
        case Register16Placeholder::BC:
            return &Register::U16::BC;
        case Register16Placeholder::DE:
            return &Register::U16::DE;
        case Register16Placeholder::HL:
            return &Register::U16::HL;
        case Register16Placeholder::SP:
            return &Register::U16::SP;
        default:
            throw BadRegister();
    }
}

std::pair<CPU::Register16MemoryPlaceholder, CPU::Register16> CPU::get_register16_memory() const
{
    const auto r16placeholder{static_cast<Register16MemoryPlaceholder>((this->opcode & 0b00110000U) >> 4)};
    Register16 reg{};

    switch (r16placeholder)
    {
        case Register16MemoryPlaceholder::BC:
            reg = &Register::U16::BC;
            break;
        case Register16MemoryPlaceholder::DE:
            reg = &Register::U16::DE;
            break;
        case Register16MemoryPlaceholder::HL_PLUS:
        case Register16MemoryPlaceholder::HL_MINUS:
            reg = &Register::U16::HL;
            break;
        default:
            throw BadRegister();
    }

    return std::make_pair(r16placeholder, reg);
}

CPU::Register16 CPU::get_register16_stack() const
{
    switch (static_cast<Register16StackPlaceholder>((this->opcode & 0b00110000U) >> 4))
    {
        case Register16StackPlaceholder::BC:
            return &Register::U16::BC;
        case Register16StackPlaceholder::DE:
            return &Register::U16::DE;
        case Register16StackPlaceholder::HL:
            return &Register::U16::HL;
        case Register16StackPlaceholder::AF:
            return &Register::U16::AF;
        default:
            throw BadRegister();
    }
}

bool CPU::check_condition_is_met() const
{
    switch (static_cast<ConditionalPlaceholder>((this->opcode >> 3) & 0b00000011U))
    {
        case ConditionalPlaceholder::C:
            return this->carry() == true;
        case ConditionalPlaceholder::Z:
            return this->zero() == true;
        case ConditionalPlaceholder::NC:
            return this->carry() == false;
        case ConditionalPlaceholder::NZ:
            return this->zero() == false;
        default:
            throw BadRegister();
    }
}

void CPU::set_carry(const bool carry)
{
    this->reg.u8.F = this->reg.u8.F & ~Flags::CARRY | (carry ? Flags::CARRY : 0);
}

bool CPU::carry() const
{
    return (this->reg.u8.F & Flags::CARRY) != 0;
}

void CPU::set_half_carry(const bool half_carry)
{
    this->reg.u8.F = this->reg.u8.F & ~Flags::HALF_CARRY | (half_carry ? Flags::HALF_CARRY : 0);
}

bool CPU::half_carry() const
{
    return (this->reg.u8.F & Flags::HALF_CARRY) != 0;
}

void CPU::set_subtract(const bool subtract)
{
    this->reg.u8.F = this->reg.u8.F & ~Flags::SUBTRACT | (subtract ? Flags::SUBTRACT : 0);
}

bool CPU::subtract() const
{
    return (this->reg.u8.F & Flags::SUBTRACT) != 0;
}

void CPU::set_zero(const bool zero)
{
    this->reg.u8.F = this->reg.u8.F & ~Flags::ZERO | (zero ? Flags::ZERO : 0);
}

bool CPU::zero() const
{
    return (this->reg.u8.F & Flags::ZERO) != 0;
}