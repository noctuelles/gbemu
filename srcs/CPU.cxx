#include "CPU.hxx"

/**
 * @note https://gbdev.io/gb-opcodes/optables/
 * https://rgbds.gbdev.io/docs/v0.8.0/gbz80.7
 * https://dn790000.ca.archive.org/0/items/GameBoyProgManVer1.1/GameBoyProgManVer1.1.pdf
 */

#include <Utils.hxx>
#include <format>
#include <utility>

const CPU::InstructionLookupTable CPU::inst_lookup{{
    {},                            // 0x00
    Instruction::LD_R16_IMM16(),   // 0x01
    {},                            // 0x02
    Instruction::INC_R16(),        // 0x03
    Instruction::INC_R8(),         // 0x04
    Instruction::DEC_R8(),         // 0x05
    Instruction::LD_R8_IMM8(),     // 0x06
    {},                            // 0x07
    {},                            // 0x08
    Instruction::ADD_HL_R16(),     // 0x09
    {},                            // 0x0A
    Instruction::DEC_R16(),        // 0x0B
    Instruction::INC_R8(),         // 0x0C
    Instruction::DEC_R8(),         // 0x0D
    Instruction::LD_R8_IMM8(),     // 0x0E
    {},                            // 0x0F
    {},                            // 0x10
    Instruction::LD_R16_IMM16(),   // 0x11
    {},                            // 0x12
    Instruction::INC_R16(),        // 0x13
    Instruction::INC_R8(),         // 0x14
    Instruction::DEC_R8(),         // 0x15
    Instruction::LD_R8_IMM8(),     // 0x16
    {},                            // 0x17
    Instruction::JR_IMM8(),        // 0x18
    Instruction::ADD_HL_R16(),     // 0x19
    {},                            // 0x1A
    Instruction::DEC_R16(),        // 0x1B
    Instruction::INC_R8(),         // 0x1C
    Instruction::DEC_R8(),         // 0x1D
    Instruction::LD_R8_IMM8(),     // 0x1E
    {},                            // 0x1F
    Instruction::JR_CC_IMM8(),     // 0x20
    Instruction::LD_R16_IMM16(),   // 0x21
    {},                            // 0x22
    Instruction::INC_R16(),        // 0x23
    Instruction::INC_R8(),         // 0x24
    Instruction::DEC_R8(),         // 0x25
    Instruction::LD_R8_IMM8(),     // 0x26
    {},                            // 0x27
    Instruction::JR_CC_IMM8(),     // 0x28
    Instruction::ADD_HL_R16(),     // 0x29
    {},                            // 0x2A
    Instruction::DEC_R16(),        // 0x2B
    Instruction::INC_R8(),         // 0x2C
    Instruction::DEC_R8(),         // 0x2D
    Instruction::LD_R8_IMM8(),     // 0x2E
    {},                            // 0x2F
    Instruction::JR_CC_IMM8(),     // 0x30
    Instruction::LD_R16_IMM16(),   // 0x31
    {},                            // 0x32
    Instruction::INC_R16(),        // 0x33
    Instruction::INC_MEM_HL(),     // 0x34
    Instruction::DEC_MEM_HL(),     // 0x35
    {},                            // 0x36
    {},                            // 0x37
    Instruction::JR_CC_IMM8(),     // 0x38
    Instruction::ADD_HL_R16(),     // 0x39
    {},                            // 0x3A
    Instruction::DEC_R16(),        // 0x3B
    Instruction::INC_R8(),         // 0x3C
    Instruction::DEC_R8(),         // 0x3D
    Instruction::LD_R8_IMM8(),     // 0x3E
    {},                            // 0x3F
    Instruction::LD_R8_R8(),       // 0x40
    Instruction::LD_R8_R8(),       // 0x41
    Instruction::LD_R8_R8(),       // 0x42
    Instruction::LD_R8_R8(),       // 0x43
    Instruction::LD_R8_R8(),       // 0x44
    Instruction::LD_R8_R8(),       // 0x45
    Instruction::LD_R8_MEM_HL(),   // 0x46
    Instruction::LD_R8_R8(),       // 0x47
    Instruction::LD_R8_R8(),       // 0x48
    Instruction::LD_R8_R8(),       // 0x49
    Instruction::LD_R8_R8(),       // 0x4A
    Instruction::LD_R8_R8(),       // 0x4B
    Instruction::LD_R8_R8(),       // 0x4C
    Instruction::LD_R8_R8(),       // 0x4D
    Instruction::LD_R8_MEM_HL(),   // 0x4E
    Instruction::LD_R8_R8(),       // 0x4F
    Instruction::LD_R8_R8(),       // 0x50
    Instruction::LD_R8_R8(),       // 0x51
    Instruction::LD_R8_R8(),       // 0x52
    Instruction::LD_R8_R8(),       // 0x53
    Instruction::LD_R8_R8(),       // 0x54
    Instruction::LD_R8_R8(),       // 0x55
    Instruction::LD_R8_MEM_HL(),   // 0x56
    Instruction::LD_R8_R8(),       // 0x57
    Instruction::LD_R8_R8(),       // 0x58
    Instruction::LD_R8_R8(),       // 0x59
    Instruction::LD_R8_R8(),       // 0x5A
    Instruction::LD_R8_R8(),       // 0x5B
    Instruction::LD_R8_R8(),       // 0x5C
    Instruction::LD_R8_R8(),       // 0x5D
    Instruction::LD_R8_MEM_HL(),   // 0x5E
    Instruction::LD_R8_R8(),       // 0x5F
    Instruction::LD_R8_R8(),       // 0x60
    Instruction::LD_R8_R8(),       // 0x61
    Instruction::LD_R8_R8(),       // 0x62
    Instruction::LD_R8_R8(),       // 0x63
    Instruction::LD_R8_R8(),       // 0x64
    Instruction::LD_R8_R8(),       // 0x65
    Instruction::LD_R8_MEM_HL(),   // 0x66
    Instruction::LD_R8_R8(),       // 0x67
    Instruction::LD_R8_R8(),       // 0x68
    Instruction::LD_R8_R8(),       // 0x69
    Instruction::LD_R8_R8(),       // 0x6A
    Instruction::LD_R8_R8(),       // 0x6B
    Instruction::LD_R8_R8(),       // 0x6C
    Instruction::LD_R8_R8(),       // 0x6D
    Instruction::LD_R8_MEM_HL(),   // 0x6E
    Instruction::LD_R8_R8(),       // 0x6F
    Instruction::LD_MEM_HL_R8(),   // 0x70
    Instruction::LD_MEM_HL_R8(),   // 0x71
    Instruction::LD_MEM_HL_R8(),   // 0x72
    Instruction::LD_MEM_HL_R8(),   // 0x73
    Instruction::LD_MEM_HL_R8(),   // 0x74
    Instruction::LD_MEM_HL_R8(),   // 0x75
    {},                            // 0x76
    Instruction::LD_MEM_HL_R8(),   // 0x77
    Instruction::LD_R8_R8(),       // 0x78
    Instruction::LD_R8_R8(),       // 0x79
    Instruction::LD_R8_R8(),       // 0x7A
    Instruction::LD_R8_R8(),       // 0x7B
    Instruction::LD_R8_R8(),       // 0x7C
    Instruction::LD_R8_R8(),       // 0x7D
    Instruction::LD_R8_MEM_HL(),   // 0x7E
    Instruction::LD_R8_R8(),       // 0x7F
    Instruction::ADD_A_R8(),       // 0x80
    Instruction::ADD_A_R8(),       // 0x81
    Instruction::ADD_A_R8(),       // 0x82
    Instruction::ADD_A_R8(),       // 0x83
    Instruction::ADD_A_R8(),       // 0x84
    Instruction::ADD_A_R8(),       // 0x85
    Instruction::ADD_A_MEM_HL(),   // 0x86
    Instruction::ADD_A_R8(),       // 0x87
    Instruction::ADC_A_R8(),       // 0x88
    Instruction::ADC_A_R8(),       // 0x8b
    Instruction::ADC_A_R8(),       // 0x8A
    Instruction::ADC_A_R8(),       // 0x8B
    Instruction::ADC_A_R8(),       // 0x8C
    Instruction::ADC_A_R8(),       // 0x8D
    Instruction::ADC_A_MEM_HL(),   // 0x8E
    Instruction::ADC_A_R8(),       // 0x90
    {},                            // 0x90
    {},                            // 0x91
    {},                            // 0x92
    {},                            // 0x93
    {},                            // 0x94
    {},                            // 0x95
    {},                            // 0x96
    {},                            // 0x97
    {},                            // 0x98
    {},                            // 0x99
    {},                            // 0x9A
    {},                            // 0x9B
    {},                            // 0x9C
    {},                            // 0x9D
    {},                            // 0x9E
    {},                            // 0x9F
    Instruction::AND_R8(),         // 0xA0
    Instruction::AND_R8(),         // 0xA1
    Instruction::AND_R8(),         // 0xA2
    Instruction::AND_R8(),         // 0xA3
    Instruction::AND_R8(),         // 0xA4
    Instruction::AND_R8(),         // 0xA5
    {},                            // 0xA6
    Instruction::AND_R8(),         // 0xA7
    Instruction::XOR_R8(),         // 0xA8
    Instruction::XOR_R8(),         // 0xA9
    Instruction::XOR_R8(),         // 0xAA
    Instruction::XOR_R8(),         // 0xAB
    Instruction::XOR_R8(),         // 0xAC
    Instruction::XOR_R8(),         // 0xAD
    {},                            // 0xAE
    Instruction::XOR_R8(),         // 0xAF
    Instruction::OR_R8(),          // 0xB0
    Instruction::OR_R8(),          // 0xB1
    Instruction::OR_R8(),          // 0xB2
    Instruction::OR_R8(),          // 0xB3
    Instruction::OR_R8(),          // 0xB4
    Instruction::OR_R8(),          // 0xB5
    {},                            // 0xB6
    Instruction::OR_R8(),          // 0xB7
    {},                            // 0xB8
    {},                            // 0xB9
    {},                            // 0xBA
    {},                            // 0xBB
    {},                            // 0xBC
    {},                            // 0xBD
    {},                            // 0xBE
    {},                            // 0xBF
    Instruction::RET_CC(),         // 0xC0
    Instruction::POP_R16(),        // 0xC1
    Instruction::JP_CC_IMM16(),    // 0xC2
    Instruction::JP_IMM16(),       // 0xC3
    Instruction::CALL_CC_IMM16(),  // 0xC4
    Instruction::PUSH_R16(),       // 0xC5
    Instruction::ADD_A_IMM8(),     // 0xC6
    {},                            // 0xC7
    Instruction::RET_CC(),         // 0xC8
    Instruction::RET(),            // 0xC9
    Instruction::JP_CC_IMM16(),    // 0xCA
    Instruction::PREFIX(),         // 0xCB
    Instruction::CALL_CC_IMM16(),  // 0xCC
    Instruction::CALL_IMM16(),     // 0xCD
    {},                            // 0xCE
    {},                            // 0xCF
    Instruction::RET_CC(),         // 0xD0
    Instruction::POP_R16(),        // 0xD1
    Instruction::JP_CC_IMM16(),    // 0xD2
    {},                            // 0xD3
    Instruction::CALL_CC_IMM16(),  // 0xD4
    Instruction::PUSH_R16(),       // 0xD5
    {},                            // 0xD6
    {},                            // 0xD7
    Instruction::RET_CC(),         // 0xD8
    {},                            // 0xD9
    Instruction::JP_CC_IMM16(),    // 0xDA
    {},                            // 0xDB
    Instruction::CALL_CC_IMM16(),  // 0xDC
    {},                            // 0xDD
    {},                            // 0xDE
    {},                            // 0xDF
    {},                            // 0xE0
    Instruction::POP_R16(),        // 0xE1
    {},                            // 0xE2
    {},                            // 0xE3
    {},                            // 0xE4
    Instruction::PUSH_R16(),       // 0xE5
    {},                            // 0xE6
    {},                            // 0xE7
    {},                            // 0xE8
    Instruction::JP_HL(),          // 0xE9
    {},                            // 0xEA
    {},                            // 0xEB
    {},                            // 0xEC
    {},                            // 0xED
    {},                            // 0xEE
    {},                            // 0xEF
    {},                            // 0xF0
    Instruction::POP_R16(),        // 0xF1
    {},                            // 0xF2
    {},                            // 0xF3
    {},                            // 0xF4
    Instruction::PUSH_R16(),       // 0xF5
    {},                            // 0xF6
    {},                            // 0xF7
    {},                            // 0xF8
    {},                            // 0xF9
    {},                            // 0xFA
    {},                            // 0xFB
    {},                            // 0xFC
    {},                            // 0xFD
    {},                            // 0xFE
    {},                            // 0xFF
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

constexpr CPU::Instruction CPU::Instruction::ADC_A_R8()
{
    return Instruction{4U, &CPU::ADC_A_R8};
}

constexpr CPU::Instruction CPU::Instruction::ADC_A_MEM_HL()
{
    return Instruction{4U, &CPU::ADC_A_MEM_HL};
}

constexpr CPU::Instruction CPU::Instruction::ADD_A_R8()
{
    return Instruction{4U, &CPU::ADD_A_R8};
}

constexpr CPU::Instruction CPU::Instruction::ADD_A_IMM8()
{
    return Instruction{4U, &CPU::ADD_A_IMM8};
}

constexpr CPU::Instruction CPU::Instruction::ADD_A_MEM_HL()
{
    return Instruction{4U, &CPU::ADD_A_MEM_HL};
}

constexpr CPU::Instruction CPU::Instruction::ADD_HL_R16()
{
    return Instruction{8U, &CPU::ADD_HL_R16};
}

constexpr CPU::Instruction CPU::Instruction::INC_R8()
{
    return Instruction{4U, &CPU::INC_R8};
}

constexpr CPU::Instruction CPU::Instruction::DEC_R8()
{
    return Instruction{4U, &CPU::DEC_R8};
}

constexpr CPU::Instruction CPU::Instruction::INC_R16()
{
    return Instruction{8U, &CPU::INC_R16};
}

constexpr CPU::Instruction CPU::Instruction::DEC_R16()
{
    return Instruction{8U, &CPU::DEC_R16};
}

constexpr CPU::Instruction CPU::Instruction::INC_MEM_HL()
{
    return Instruction{12U, &CPU::INC_MEM_HL};
}

constexpr CPU::Instruction CPU::Instruction::DEC_MEM_HL()
{
    return Instruction{12U, &CPU::DEC_MEM_HL};
}

constexpr CPU::Instruction CPU::Instruction::JP_IMM16()
{
    return Instruction{16U, &CPU::JP_IMM16};
}

constexpr CPU::Instruction CPU::Instruction::JP_HL()
{
    return Instruction{4U, &CPU::JP_HL};
}

constexpr CPU::Instruction CPU::Instruction::JP_CC_IMM16()
{
    return Instruction{12U, &CPU::JP_CC_IMM16};
}

constexpr CPU::Instruction CPU::Instruction::CALL_IMM16()
{
    return Instruction{24U, &CPU::CALL_IMM16};
}

constexpr CPU::Instruction CPU::Instruction::CALL_CC_IMM16()
{
    return Instruction{12U, &CPU::CALL_CC_IMM16};
}

constexpr CPU::Instruction CPU::Instruction::RET()
{
    return Instruction{16U, &CPU::RET};
}

constexpr CPU::Instruction CPU::Instruction::RET_CC()
{
    return Instruction{8U, &CPU::RET_CC};
}

constexpr CPU::Instruction CPU::Instruction::JR_IMM8()
{
    return Instruction{12U, &CPU::JR_IMM8};
}

constexpr CPU::Instruction CPU::Instruction::JR_CC_IMM8()
{
    return Instruction{8U, &CPU::JR_CC_IMM8};
}

constexpr CPU::Instruction CPU::Instruction::PUSH_R16()
{
    return Instruction{16U, &CPU::PUSH_R16};
}

constexpr CPU::Instruction CPU::Instruction::POP_R16()
{
    return Instruction{12U, &CPU::POP_R16};
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
    const auto dest{this->get_register8(Register8Position::LEFTMOST)};
    const auto src{this->get_register8(Register8Position::RIGHTMOST)};
    this->reg.u8.*dest = this->reg.u8.*src;
}

void CPU::LD_R8_IMM8()
{
    const auto dest{this->get_register8(Register8Position::LEFTMOST)};
    const auto imm{this->bus.read(this->reg.u16.PC++)};

    this->reg.u8.*dest = imm;
}

void CPU::LD_R16_IMM16()
{
    const auto dest{this->get_register16()};
    const auto imm_lsb{this->bus.read(this->reg.u16.PC++)};
    const auto imm_msb{this->bus.read(this->reg.u16.PC++)};

    this->reg.u16.*dest = static_cast<uint16_t>(imm_msb << 8 | imm_lsb);
}

void CPU::LD_R8_MEM_HL()
{
    const auto src{this->get_register8_dest_from_opcode()};
    const auto mem_val{this->bus.read(this->reg.u16.HL)};

    this->reg.u8.*src = mem_val;
}

void CPU::LD_MEM_HL_R8()
{
    const auto src{this->get_register8_src_from_opcode()};
    this->bus.write(this->reg.u16.HL, this->reg.u8.*src);
}

void CPU::LD_A_MEM_R16() {}

void CPU::LD_MEM_HL_IMM8()
{
    this->bus.write(this->reg.u16.HL, this->bus.read(this->reg.u16.PC++));
}

void CPU::PUSH_R16()
{
    const auto reg{this->get_register16_stack()};
    this->push_16(u16_msb(this->reg.u16.*reg), u16_lsb(this->reg.u16.*reg));
}

void CPU::POP_R16()
{
    const auto reg{this->get_register16_stack()};
    const auto val = this->pop_16();

    this->reg.u16.*reg = val;
}

void CPU::RES_R8()
{
    const auto src{this->get_register8(Register8Position::RIGHTMOST)};
    const auto bit{this->get_b3()};

    this->reg.u8.*src &= ~(1 << bit);
}

void CPU::RES_MEM_HL()
{
    const auto bit{this->get_b3()};
    auto       mem_val{this->bus.read(this->reg.u16.HL)};

    mem_val &= ~(1 << bit);

    this->bus.write(this->reg.u16.HL, mem_val);
}

void CPU::SET_R8()
{
    const auto src{this->get_register8(Register8Position::RIGHTMOST)};
    const auto bit{this->get_b3()};

    this->reg.u8.*src |= 1U << bit;
}

void CPU::SET_MEM_HL()
{
    const auto bit{this->get_b3()};
    auto       mem_val = this->bus.read(this->reg.u16.HL);

    mem_val |= 1U << bit;

    this->bus.write(this->reg.u16.HL, mem_val);
}

void CPU::AND_R8()
{
    const auto src{this->get_register8(Register8Position::RIGHTMOST)};

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
    const auto src = get_register8(Register8Position::RIGHTMOST);

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
    const auto r_src{get_register8(Register8Position::RIGHTMOST)};

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

uint16_t CPU::ADD_16(const uint16_t a, const uint16_t b)
{
    uint32_t result32{};

    if (((a & 0x7FF) + (b & 0x7FF)) & 0x800 != 0)
    {
        this->reg.u8.F |= Flags::HALF_CARRY;
    }
    else
    {
        this->reg.u8.F &= ~Flags::HALF_CARRY;
    }
    result32 = a + b;
    if ((result32 & 0x1000) != 0)
    {
        this->reg.u8.F |= Flags::CARRY;
    }
    else
    {
        this->reg.u8.F &= ~Flags::CARRY;
    }
    this->reg.u8.F &= ~Flags::SUBTRACT;
    return static_cast<uint16_t>(result32);
}

uint8_t CPU::ADD_8(const uint8_t a, const uint8_t b, bool add_carry)
{
    uint16_t result16{};
    uint8_t  result8{};

    add_carry = this->reg.u8.F & Flags::CARRY ? true : false;

    if (((a & 0b1111) + (b & 0b1111) + add_carry) & 0b10000 != 0)
    {
        this->reg.u8.F |= Flags::HALF_CARRY;
    }
    else
    {
        this->reg.u8.F &= ~Flags::HALF_CARRY;
    }
    result16 = a + b + add_carry;
    if ((result16 & 0b100000000U) != 0)
    {
        this->reg.u8.F |= Flags::CARRY;
    }
    else
    {
        this->reg.u8.F &= ~Flags::CARRY;
    }
    result8 = static_cast<uint8_t>(result16);
    if (result8 == 0)
    {
        this->reg.u8.F |= Flags::ZERO;
    }
    else
    {
        this->reg.u8.F &= ~Flags::ZERO;
    }
    this->reg.u8.F &= ~Flags::SUBTRACT;

    return (result8);
}

void CPU::ADD_A_R8()
{
    const auto operand{this->get_register8(Register8Position::RIGHTMOST)};
    this->reg.u8.A = this->ADD_8(this->reg.u8.A, this->reg.u8.*operand, false);
}

void CPU::ADD_A_MEM_HL()
{
    const auto operand{this->bus.read(this->reg.u16.HL)};
    this->bus.write(this->reg.u16.HL, this->ADD_8(this->reg.u8.A, operand, false));
}

void CPU::ADD_A_IMM8()
{
    const auto operand{this->bus.read(this->reg.u16.PC++)};
    this->reg.u8.A = this->ADD_8(this->reg.u8.A, operand, false);
}

void CPU::ADD_HL_R16()
{
    const auto operand{this->get_register16()};
    this->reg.u16.HL = this->ADD_16(this->reg.u16.HL, this->reg.u16.*operand);
}

void CPU::ADC_A_R8()
{
    const auto operand{this->get_register8(Register8Position::RIGHTMOST)};
    this->reg.u8.A = this->ADD_8(this->reg.u8.A, this->reg.u8.*operand, true);
}

void CPU::ADC_A_MEM_HL()
{
    const auto operand{this->bus.read(this->reg.u16.HL)};
    this->bus.write(this->reg.u16.HL, this->ADD_8(this->reg.u8.A, operand, true));
}

uint8_t CPU::STEP_IMM8(uint8_t value, const StepType type)
{
    if (type == StepType::INCREMENT)
    {
        value += 1;
        this->reg.u8.F &= ~Flags::SUBTRACT;
    }
    else
    {
        value -= 1;
        this->reg.u8.F |= Flags::SUBTRACT;
    }
    if (value == 0)
    {
        this->reg.u8.F |= Flags::ZERO;
    }
    else
    {
        this->reg.u8.F &= ~Flags::ZERO;
    }
    if (value & 0b00010000U)
    {
        this->reg.u8.F |= Flags::HALF_CARRY;
    }
    else
    {
        this->reg.u8.F &= ~Flags::HALF_CARRY;
    }
    return value;
}

void CPU::INC_R8()
{
    const auto operand = this->get_register8(Register8Position::LEFTMOST);
    this->reg.u8.*operand += this->STEP_IMM8(this->reg.u8.*operand, StepType::INCREMENT);
}

void CPU::DEC_R8()
{
    const auto operand = this->get_register8(Register8Position::LEFTMOST);
    this->reg.u8.*operand += this->STEP_IMM8(this->reg.u8.*operand, StepType::DECREMENT);
}

void CPU::INC_R16()
{
    const auto operand = this->get_register16();
    this->reg.u16.*operand += 1;
}

void CPU::DEC_R16()
{
    const auto dest = this->get_register16();
    this->reg.u16.*dest -= 1;
}

void CPU::INC_MEM_HL()
{
    const auto value = this->bus.read(this->reg.u16.HL);
    this->bus.write(this->reg.u16.HL, this->STEP_IMM8(value, StepType::INCREMENT));
}

void CPU::DEC_MEM_HL()
{
    const auto value = this->bus.read(this->reg.u16.HL);
    this->bus.write(this->reg.u16.HL, this->STEP_IMM8(value, StepType::DECREMENT));
}

void CPU::ILL()
{
    throw IllegalInstruction(this->opcode);
}

void CPU::PREFIX()
{
    this->cb_prefixed = true;
}

void CPU::JP_IMM16()
{
    const auto imm_lsb = this->bus.read(this->reg.u16.PC++);
    const auto imm_msb = this->bus.read(this->reg.u16.PC++);

    this->reg.u16.PC = imm_msb << 8 | imm_lsb;
}

void CPU::JP_HL()
{
    this->reg.u16.PC = this->reg.u16.HL;
}

void CPU::JP_CC_IMM16()
{
    if (this->check_condition_is_met())
    {
        this->cycles += 4;
        this->JP_IMM16();
    }
    else
    {
        this->reg.u16.PC += 2;
    }
}

void CPU::CALL_IMM16()
{
    const auto addr_lsb = this->bus.read(this->reg.u16.PC++);
    const auto addr_msb = this->bus.read(this->reg.u16.PC++);

    this->push_16(u16_msb(this->reg.u16.PC), u16_lsb(this->reg.u16.PC));
    this->reg.u16.PC = u8_to_u16({addr_msb, addr_lsb});
}

void CPU::CALL_CC_IMM16()
{
    if (this->check_condition_is_met())
    {
        this->cycles += 12;
        this->CALL_IMM16();
    }
    else
    {
        this->reg.u16.PC += 2;
    }
}

void CPU::JR_IMM8()
{
    const auto displacement = static_cast<int8_t>(this->bus.read(this->reg.u16.PC++));
    this->reg.u16.PC += displacement;
}

void CPU::JR_CC_IMM8()
{
    if (this->check_condition_is_met())
    {
        this->cycles += 4;
        this->JR_IMM8();
    }
    else
    {
        this->reg.u16.PC += 1;
    }
}

void CPU::RET()
{
    this->reg.u16.PC = this->pop_16();
}

void CPU::RET_CC()
{
    if (this->check_condition_is_met())
    {
        this->cycles += 12;
        this->RET();
    }
}

auto CPU::ROTATE(uint8_t val, const RotateDirection rotate_direction, const bool rotate_through_carry) noexcept
{
    auto has_new_carry = false;

    if (rotate_direction == RotateDirection::LEFT)
    {
        has_new_carry = (val & 0b10000000U) != 0;
        val <<= 1;
        if (rotate_through_carry)
        {
            val |= (this->reg.u8.F & Flags::CARRY ? 0b00000001U : 0b00000000U);
        }
    }
    else
    {
        has_new_carry = (val & 0b00000001U) != 0;
        val >>= 1;
        if (rotate_through_carry)
        {
            val |= (this->reg.u8.F & Flags::CARRY ? 0b10000000U : 0b00000000U);
        }
    }
    if (val == 0)
    {
        this->reg.u8.F |= Flags::ZERO;
    }
    else
    {
        this->reg.u8.F &= ~Flags::ZERO;
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
    const auto operand    = this->get_register8(Register8Position::RIGHTMOST);
    this->reg.u8.*operand = this->ROTATE(this->reg.u8.*operand, RotateDirection::LEFT, false);
}

void CPU::RLC_MEM_HL()
{
    auto val = this->bus.read(this->reg.u16.HL);
    val      = this->ROTATE(val, RotateDirection::LEFT, false);
    this->bus.write(this->reg.u16.HL, val);
}

void CPU::RRC_R8()
{
    const auto operand    = this->get_register8(Register8Position::RIGHTMOST);
    this->reg.u8.*operand = this->ROTATE(this->reg.u8.*operand, RotateDirection::RIGHT, false);
}

void CPU::RRC_MEM_HL()
{
    auto val = this->bus.read(this->reg.u16.HL);
    val      = this->ROTATE(val, RotateDirection::RIGHT, false);
    this->bus.write(this->reg.u16.HL, val);
}

void CPU::RL_R8()
{
    const auto operand    = this->get_register8(Register8Position::RIGHTMOST);
    this->reg.u8.*operand = this->ROTATE(this->reg.u8.*operand, RotateDirection::LEFT, true);
}
void CPU::RL_MEM_HL()
{
    auto val = this->bus.read(this->reg.u16.HL);
    val      = this->ROTATE(val, RotateDirection::LEFT, true);
    this->bus.write(this->reg.u16.HL, val);
}

void CPU::RR_R8()
{
    const auto operand    = this->get_register8(Register8Position::RIGHTMOST);
    this->reg.u8.*operand = this->ROTATE(this->reg.u8.*operand, RotateDirection::RIGHT, true);
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
    sign_bit = (val & 0b10000000U) != 0;
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
    else
    {
        this->reg.u8.F &= ~Flags::ZERO;
    }

    this->reg.u8.F &= ~Flags::HALF_CARRY;
    this->reg.u8.F &= ~Flags::SUBTRACT;

    return val;
}

void CPU::SRA_R8()
{
    const auto src    = this->get_register8(Register8Position::RIGHTMOST);
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
    const auto src    = this->get_register8(Register8Position::RIGHTMOST);
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
    const auto src    = this->get_register8(Register8Position::RIGHTMOST);
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
    else
    {
        this->reg.u8.F &= ~Flags::ZERO;
    }

    return (val);
}

void CPU::SWAP_R8()
{
    const auto src    = this->get_register8(Register8Position::RIGHTMOST);
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
    if (val & (1 << bit))
    {
        this->reg.u8.F &= ~Flags::ZERO;
    }
    else
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
    const auto src = this->get_register8(Register8Position::RIGHTMOST);
    const auto bit = static_cast<uint8_t>((this->opcode & 0b00111000U) >> 3U);
    this->BIT(this->reg.u8.*src, bit);
}

size_t CPU::cycle()
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
    return this->cycles;
}

CPU::Register CPU::get_register() const noexcept
{
    return this->reg;
}

uint8_t CPU::get_b3() const
{
    return (this->opcode >> 3) & 0b00000111;
}

void CPU::push_16(const uint8_t msb, const uint8_t lsb)
{
    this->bus.write(this->reg.u16.SP--, msb);
    this->bus.write(this->reg.u16.SP--, lsb);
}

uint16_t CPU::pop_16()
{
    const auto lsb = this->bus.read(++this->reg.u16.SP);
    const auto msb = this->bus.read(++this->reg.u16.SP);
    return u8_to_u16({msb, lsb});
}

CPU::Register8 CPU::get_register8(const Register8Position shift) const
{
    switch (static_cast<Register8Placeholder>((this->opcode >> static_cast<uint8_t>(shift)) & 0b00000011U))
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

CPU::Register16 CPU::get_register16_memory() const
{
    switch (static_cast<Register16MemoryPlaceholder>((this->opcode & 0b00110000U) >> 4))
    {
        case Register16MemoryPlaceholder::BC:
            return &Register::U16::BC;
        case Register16MemoryPlaceholder::DE:
            return &Register::U16::DE;
        case Register16MemoryPlaceholder::HL_PLUS:
        case Register16MemoryPlaceholder::HL_MINUS:
            return &Register::U16::HL;
        default:
            throw BadRegister();
    }
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
    const auto condition     = static_cast<ConditionalPlaceholder>((this->opcode >> 3) & 0b00000011U);
    auto       condition_met = false;

    switch (condition)
    {
        case ConditionalPlaceholder::C:
            condition_met = (this->reg.u8.F & Flags::CARRY) != 0;
            break;
        case ConditionalPlaceholder::Z:
            condition_met = (this->reg.u8.F & Flags::ZERO) != 0;
            break;
        case ConditionalPlaceholder::NC:
            condition_met = (this->reg.u8.F & Flags::CARRY) == 0;
            break;
        case ConditionalPlaceholder::NZ:
            condition_met = (this->reg.u8.F & Flags::ZERO) == 0;
            break;
    }

    return condition_met;
}