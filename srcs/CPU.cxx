#include "CPU.hxx"

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
    {},                               // 0x00
    {},                               // 0x01
    {},                               // 0x02
    {},                               // 0x03
    {},                               // 0x04
    {},                               // 0x05
    {},                               // 0x06
    {},                               // 0x07
    {},                               // 0x08
    {},                               // 0x09
    {},                               // 0x0A
    {},                               // 0x0B
    {},                               // 0x0C
    {},                               // 0x0D
    {},                               // 0x0E
    {},                               // 0x0F
    {},                               // 0x10
    {},                               // 0x11
    {},                               // 0x12
    {},                               // 0x13
    {},                               // 0x14
    {},                               // 0x15
    {},                               // 0x16
    {},                               // 0x17
    {},                               // 0x18
    {},                               // 0x19
    {},                               // 0x1A
    {},                               // 0x1B
    {},                               // 0x1C
    {},                               // 0x1D
    {},                               // 0x1E
    {},                               // 0x1F
    {},                               // 0x20
    {},                               // 0x21
    {},                               // 0x22
    {},                               // 0x23
    {},                               // 0x24
    {},                               // 0x25
    {},                               // 0x26
    {},                               // 0x27
    {},                               // 0x28
    {},                               // 0x29
    {},                               // 0x2A
    {},                               // 0x2B
    {},                               // 0x2C
    {},                               // 0x2D
    {},                               // 0x2E
    {},                               // 0x2F
    CPU::Instruction::SWAP_R8(),      // 0x30
    CPU::Instruction::SWAP_R8(),      // 0x31
    CPU::Instruction::SWAP_R8(),      // 0x32
    CPU::Instruction::SWAP_R8(),      // 0x33
    CPU::Instruction::SWAP_R8(),      // 0x34
    CPU::Instruction::SWAP_R8(),      // 0x35
    CPU::Instruction::SWAP_MEM_HL(),  // 0x36
    CPU::Instruction::SWAP_R8(),      // 0x37
    {},                               // 0x38
    {},                               // 0x39
    {},                               // 0x3A
    {},                               // 0x3B
    {},                               // 0x3C
    {},                               // 0x3D
    {},                               // 0x3E
    {},                               // 0x3F
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

constexpr CPU::Instruction::Instruction(const size_t cycles, void (CPU::*op)())
    : cycles(cycles), op(op)
{
}

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

CPU::CPU(Bus& bus) : reg(), cycles(0), opcode(0), cb_prefixed(false), inst(inst_lookup[0]), bus(bus)
{
}

CPU::~CPU() = default;

void CPU::NOP() {}

void CPU::LD_R8_R8()
{
    const auto r_dest = static_cast<InstructionRegister8>((this->opcode >> 3) & 0b00000111);
    const auto r_src  = static_cast<uint8_t>(this->opcode & 0b00000111);

    this->set_register_r8_imm8(r_dest, r_src);
}

void CPU::LD_R8_IMM8()
{
    const auto r_dest = static_cast<InstructionRegister8>((this->opcode >> 3) & 0b00000111);
    const auto r_imm{this->bus.read(this->reg.PC++)};

    this->set_register_r8_imm8(r_dest, r_imm);
}

void CPU::LD_R16_IMM16()
{
    const auto r_dest = static_cast<InstructionRegister16>((this->opcode >> 4) & 0b00000011U);
    const auto imm_lsb{this->bus.read(this->reg.PC++)};
    const auto imm_msb{this->bus.read(this->reg.PC++)};
    const auto imm_val = static_cast<uint16_t>(imm_lsb << 8 | imm_msb);

    this->set_register_r16_imm16(r_dest, imm_val);
}

void CPU::LD_R8_MEM_HL()
{
    const auto r_dest  = static_cast<InstructionRegister8>((this->opcode >> 3) & 0b00000111);
    const auto mem_val = this->bus.read(REG16_PAIR_GET(this->reg.H, this->reg.L));

    this->set_register_r8_imm8(r_dest, mem_val);
}

void CPU::LD_MEM_HL_R8()
{
    const auto r_dest = get_register8(static_cast<InstructionRegister8>(this->opcode & 0b00000111));
    this->bus.write(REG16_PAIR_GET(this->reg.H, this->reg.L), this->reg.*r_dest);
}
void CPU::LD_A_MEM_16() {}

void CPU::RES_R8()
{
    const auto r_dest = get_register8(static_cast<InstructionRegister8>(this->opcode & 0b00000111));
    const auto bit    = static_cast<uint8_t>((this->opcode & 0b00111000) >> 3);

    this->reg.*r_dest &= ~(1 << bit);
}
void CPU::RES_MEM_HL()
{
    const auto bit     = static_cast<uint8_t>((this->opcode & 0b00111000) >> 3);
    auto       mem_val = this->bus.read(REG16_PAIR_GET(this->reg.H, this->reg.L));

    mem_val &= ~(1 << bit);

    this->bus.write(REG16_PAIR_GET(this->reg.H, this->reg.L), mem_val);
}
void CPU::SET_R8()
{
    const auto r_dest = get_register8(static_cast<InstructionRegister8>(this->opcode & 0b00000111));
    const auto bit    = static_cast<uint8_t>((this->opcode & 0b00111000) >> 3);

    this->reg.*r_dest |= 1 << bit;
}

void CPU::SET_MEM_HL()
{
    const auto bit     = static_cast<uint8_t>((this->opcode & 0b00111000) >> 3);
    auto       mem_val = this->bus.read(REG16_PAIR_GET(this->reg.H, this->reg.L));

    mem_val |= 1 << bit;

    this->bus.write(REG16_PAIR_GET(this->reg.H, this->reg.L), mem_val);
}

void CPU::AND_R8()
{
    const auto r_src = get_register8(static_cast<InstructionRegister8>(this->opcode & 0b00000111));

    this->reg.A &= this->reg.*r_src;
    if (this->reg.A == 0)
    {
        this->reg.F |= Flags::ZERO;
    }
    else
    {
        this->reg.F &= ~Flags::ZERO;
    }
    this->reg.F |= Flags::HALF_CARRY;
    this->reg.F &= ~Flags::SUBTRACT;
    this->reg.F &= ~Flags::CARRY;
}

void CPU::OR_R8()
{
    const auto r_src = get_register8(static_cast<InstructionRegister8>(this->opcode & 0b00000111));

    this->reg.A |= this->reg.*r_src;
    if (this->reg.A == 0)
    {
        this->reg.F |= Flags::ZERO;
    }
    else
    {
        this->reg.F &= ~Flags::ZERO;
    }
    this->reg.F &= ~Flags::HALF_CARRY;
    this->reg.F &= ~Flags::SUBTRACT;
    this->reg.F &= ~Flags::CARRY;
}

void CPU::XOR_R8()
{
    const auto r_src = get_register8(static_cast<InstructionRegister8>(this->opcode & 0b00000111));

    this->reg.A ^= this->reg.*r_src;
    if (this->reg.A == 0)
    {
        this->reg.F |= Flags::ZERO;
    }
    else
    {
        this->reg.F &= ~Flags::ZERO;
    }
    this->reg.F &= ~Flags::HALF_CARRY;
    this->reg.F &= ~Flags::SUBTRACT;
    this->reg.F &= ~Flags::CARRY;
}

void CPU::ADD_R8()
{
    const auto r_src = get_register8(static_cast<InstructionRegister8>(this->opcode & 0b00000111));
    uint8_t    half_carry = 0;

    half_carry = ((this->reg.A & 0x0F) + (this->reg.*r_src & 0x0F)) & 0x10;
    if (half_carry)
    {
        this->reg.F |= Flags::HALF_CARRY;
    }
    else
    {
        this->reg.F &= ~Flags::HALF_CARRY;
    }
    if (__builtin_add_overflow(this->reg.A, this->reg.*r_src, &this->reg.A))
    {
        this->reg.F |= Flags::CARRY;
    }
    else
    {
        this->reg.F &= ~Flags::CARRY;
    }
    if (this->reg.A == 0)
    {
        this->reg.F |= Flags::ZERO;
    }
    else
    {
        this->reg.F &= ~Flags::ZERO;
    }
    this->reg.F &= ~Flags::SUBTRACT;
}

void CPU::INC_R8()
{
    const auto r_dest =
        get_register8(static_cast<InstructionRegister8>((this->opcode >> 4) & 0b00000011U));
    this->reg.*r_dest += 1;
}

void CPU::ILL()
{
    throw IllegalInstructionException();
}

void CPU::PREFIX()
{
    this->cb_prefixed = true;
}
void CPU::BIT_R8()
{
    const auto r_dest = get_register8(static_cast<InstructionRegister8>(this->opcode & 0b00000111));
    const auto bit    = static_cast<uint8_t>((this->opcode & 0b00111000) >> 3);

    if (!(this->reg.*r_dest & (1 << bit)))
    {
        this->reg.F |= Flags::ZERO;
    }
    this->reg.F |= Flags::HALF_CARRY;
    this->reg.F &= ~Flags::SUBTRACT;
}

void CPU::SWAP_R8()
{
    const auto r_dest = get_register8(static_cast<InstructionRegister8>(this->opcode & 0b00000111));

    this->reg.*r_dest =
        ((this->reg.*r_dest & 0b11110000) >> 4) | ((this->reg.*r_dest & 0b00001111) << 4);

    this->reg.F &= ~Flags::SUBTRACT;
    this->reg.F &= ~Flags::HALF_CARRY;
    this->reg.F &= ~Flags::CARRY;

    if (this->reg.*r_dest == 0)
    {
        this->reg.F |= Flags::ZERO;
    }
}

void CPU::SWAP_MEM_HL()
{
    auto val = this->bus.read(REG16_PAIR_GET(this->reg.H, this->reg.L));

    val = ((val & 0b11110000) >> 4) | ((val & 0b00001111) << 4);
    this->reg.F &= ~Flags::SUBTRACT;
    this->reg.F &= ~Flags::HALF_CARRY;
    this->reg.F &= ~Flags::CARRY;

    if (val == 0)
    {
        this->reg.F |= Flags::ZERO;
    }

    this->bus.write(REG16_PAIR_GET(this->reg.H, this->reg.L), val);
}

void CPU::BIT_MEM_HL()
{
    const auto bit = static_cast<uint8_t>((this->opcode & 0b00111000) >> 3);

    if (const auto mem_val = this->bus.read(REG16_PAIR_GET(this->reg.H, this->reg.L));
        !(mem_val & (1 << bit)))
    {
        this->reg.F |= Flags::ZERO;
    }
    this->reg.F |= Flags::HALF_CARRY;
    this->reg.F &= ~Flags::SUBTRACT;
}

void CPU::cycle()
{
    if (this->cycles == 0)
    {
        this->opcode = this->bus.read(this->reg.PC);
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
        this->reg.PC++;
        (this->*inst.op)();
    }
    else
    {
        this->cycles--;
    }
}
CPU::Register CPU::get_register() const noexcept
{
    return this->reg;
}

void CPU::set_register_r16_imm16(const InstructionRegister16 reg, const uint16_t value)
{
    switch (reg)
    {
        case InstructionRegister16::BC:
            this->reg.B = value & 0xFF;
            this->reg.C = value >> 8;
            break;
        case InstructionRegister16::DE:
            this->reg.D = value & 0xFF;
            this->reg.E = value >> 8;
            break;
        case InstructionRegister16::HL:
            this->reg.H = value & 0xFF;
            this->reg.L = value >> 8;
            break;
        case InstructionRegister16::SP:
            this->reg.SP = value;
            break;
        default:
            throw BadRegisterException();
    }
}

void CPU::set_register_r8_imm8(const InstructionRegister8 reg, const uint8_t value)
{
    const auto r_dest = get_register8(reg);
    this->reg.*r_dest = value;
}

void CPU::set_register_r8_r8(const InstructionRegister8 reg_dst, const InstructionRegister8 reg_src)
{
    const auto r_dest = get_register8(reg_dst);
    const auto r_src  = get_register8(reg_src);

    this->reg.*r_dest = this->reg.*r_src;
}

CPU::Register8 CPU::get_register8(InstructionRegister8 reg)
{
    switch (reg)
    {
        case InstructionRegister8::A:
            return &CPU::Register::A;
        case InstructionRegister8::B:
            return &CPU::Register::B;
        case InstructionRegister8::C:
            return &CPU::Register::C;
        case InstructionRegister8::D:
            return &CPU::Register::D;
        case InstructionRegister8::E:
            return &CPU::Register::E;
        case InstructionRegister8::H:
            return &CPU::Register::H;
        case InstructionRegister8::L:
            return &CPU::Register::L;
        default:
            throw BadRegisterException();
    }
}
