//
// Created by plouvel on 12/27/24.
//

#include "CPU.hxx"
#include "Utils.hxx"

const CPU::InstructionLookupTable CPU::instruction_lookup{{
    {"NOP", &CPU::NOP, {}},                                                           // 0x00
    {"LD BC, ${:04X}", &CPU::LD_R16_IMM16, AddressingMode::IMMEDIATE_EXTENDED},       // 0x01
    {"LD [BC], A", &CPU::LD_MEM_R16_A},                                               // 0x02
    {"INC BC", &CPU::INC_R16},                                                        // 0x03
    {"INC B", &CPU::INC_R8},                                                          // 0x04
    {"DEC B", &CPU::DEC_R8},                                                          // 0x05
    {"LD B, ${:02X}", &CPU::LD_R8_IMM8, AddressingMode::IMMEDIATE},                   // 0x06
    {"RLCA", &CPU::RLCA},                                                             // 0x07
    {"LD [${:04X}], SP", &CPU::LD_MEM_16_SP, AddressingMode::IMMEDIATE_EXTENDED},     // 0x08
    {"ADD HL, BC", &CPU::ADD_HL_R16},                                                 // 0x09
    {"LD A, [BC]", &CPU::LD_A_MEM_R16},                                               // 0x0A
    {"DEC BC"},                                                                       // 0x0B
    {"INC C", &CPU::INC_R8},                                                          // 0x0C
    {"DEC C", &CPU::DEC_R8},                                                          // 0x0D
    {"LD C, ${:02X}", &CPU::LD_R8_IMM8, AddressingMode::IMMEDIATE},                   // 0x0E
    {"RRCA", &CPU::RRCA},                                                             // 0x0F
    {"STOP", &CPU::STOP},                                                             // 0x10
    {"LD DE, ${:04X}", &CPU::LD_R16_IMM16, AddressingMode::IMMEDIATE_EXTENDED},       // 0x11
    {"LD (DE), A", &CPU::LD_MEM_R16_A},                                               // 0x12
    {"INC DE", &CPU::INC_R16},                                                        // 0x13
    {"INC D", &CPU::INC_R8},                                                          // 0x14
    {"DEC D", &CPU::DEC_R8},                                                          // 0x15
    {"LD D, ${:02X}", &CPU::LD_R8_IMM8, AddressingMode::IMMEDIATE},                   // 0x16
    {"RLA", &CPU::RLA},                                                               // 0x17
    {"JR ${:d}", &CPU::JR_IMM8, AddressingMode::RELATIVE},                            // 0x18
    {"ADD HL, DE", &CPU::ADD_HL_R16},                                                 // 0x19
    {"LD A, (DE)", &CPU::LD_A_MEM_R16},                                               // 0x1A
    {"DEC DE", &CPU::DEC_R16},                                                        // 0x1B
    {"INC E", &CPU::INC_R8},                                                          // 0x1C
    {"DEC E", &CPU::DEC_R8},                                                          // 0x1D
    {"LD E, ${:02X}", &CPU::LD_R8_IMM8, AddressingMode::IMMEDIATE},                   // 0x1E
    {"RRA", &CPU::RRA},                                                               // 0x1F
    {"JR NZ, ${:d}", &CPU::JR_CC_IMM8, AddressingMode::RELATIVE},                     // 0x20
    {"LD HL, ${:04X}", &CPU::LD_R16_IMM16, AddressingMode::IMMEDIATE_EXTENDED},       // 0x21
    {"LD (HL+), A", &CPU::LD_MEM_R16_A},                                              // 0x22
    {"INC HL", &CPU::INC_R16},                                                        // 0x23
    {"INC H", &CPU::INC_R8},                                                          // 0x24
    {"DEC H", &CPU::DEC_R8},                                                          // 0x25
    {"LD H, ${:02X}", &CPU::LD_R8_IMM8, AddressingMode::IMMEDIATE},                   // 0x26
    {"DAA", &CPU::DAA},                                                               // 0x27
    {"JR Z, ${:d}", &CPU::JR_CC_IMM8, AddressingMode::RELATIVE},                      // 0x28
    {"ADD HL, HL", &CPU::ADD_HL_R16},                                                 // 0x29
    {"LD A, (HL+)", &CPU::LD_A_MEM_R16},                                              // 0x2A
    {"DEC HL", &CPU::DEC_R16},                                                        // 0x2B
    {"INC L", &CPU::INC_R8},                                                          // 0x2C
    {"DEC L", &CPU::DEC_R8},                                                          // 0x2D
    {"LD L, ${:02X}", &CPU::LD_R8_IMM8, AddressingMode::IMMEDIATE},                   // 0x2E
    {"CPL", &CPU::CPL},                                                               // 0x2F
    {"JR NC, ${:d}", &CPU::JR_CC_IMM8, AddressingMode::RELATIVE},                     // 0x30
    {"LD SP, ${:04X}", &CPU::LD_R16_IMM16, AddressingMode::IMMEDIATE_EXTENDED},       // 0x31
    {"LD (HL-), A", &CPU::LD_MEM_R16_A},                                              // 0x32
    {"INC SP", &CPU::INC_R16},                                                        // 0x33
    {"INC (HL)", &CPU::INC_MEM_HL},                                                   // 0x34
    {"DEC (HL)", &CPU::DEC_MEM_HL},                                                   // 0x35
    {"LD (HL), ${:02X}", &CPU::LD_MEM_HL_IMM8, AddressingMode::IMMEDIATE},            // 0x36
    {"SCF", &CPU::SCF},                                                               // 0x37
    {"JR NC, ${:d}", &CPU::JR_CC_IMM8, AddressingMode::RELATIVE},                     // 0x38
    {"ADD HL, SP", &CPU::ADD_HL_R16},                                                 // 0x39
    {"LD A, (HL-)", &CPU::LD_A_MEM_R16},                                              // 0x3A
    {"DEC SP", &CPU::DEC_R16},                                                        // 0x3B
    {"INC A", &CPU::INC_R8},                                                          // 0x3C
    {"DEC A", &CPU::DEC_R8},                                                          // 0x3D
    {"LD A, ${:02X}", &CPU::LD_R8_IMM8, AddressingMode::IMMEDIATE},                   // 0x3E
    {"CCF", &CPU::CCF},                                                               // 0x3F
    {"LD B, B", &CPU::LD_R8_R8},                                                      // 0x40
    {"LD B, C", &CPU::LD_R8_R8},                                                      // 0x41
    {"LD B, D", &CPU::LD_R8_R8},                                                      // 0x42
    {"LD B, E", &CPU::LD_R8_R8},                                                      // 0x43
    {"LD B, H", &CPU::LD_R8_R8},                                                      // 0x44
    {"LD B, L", &CPU::LD_R8_R8},                                                      // 0x45
    {"LD B, [HL]", &CPU::LD_R8_MEM_HL},                                               // 0x46
    {"LD B, A", &CPU::LD_R8_R8},                                                      // 0x47
    {"LD C, B", &CPU::LD_R8_R8},                                                      // 0x48
    {"LD C, C", &CPU::LD_R8_R8},                                                      // 0x49
    {"LD C, D", &CPU::LD_R8_R8},                                                      // 0x4A
    {"LD C, E", &CPU::LD_R8_R8},                                                      // 0x4B
    {"LD C, H", &CPU::LD_R8_R8},                                                      // 0x4C
    {"LD C, L", &CPU::LD_R8_R8},                                                      // 0x4D
    {"LD C, [HL]", &CPU::LD_R8_MEM_HL},                                               // 0x4E
    {"LD C, A", &CPU::LD_R8_R8},                                                      // 0x4F
    {"LD D, B", &CPU::LD_R8_R8},                                                      // 0x50
    {"LD D, C", &CPU::LD_R8_R8},                                                      // 0x51
    {"LD D, D", &CPU::LD_R8_R8},                                                      // 0x52
    {"LD D, E", &CPU::LD_R8_R8},                                                      // 0x53
    {"LD D, H", &CPU::LD_R8_R8},                                                      // 0x54
    {"LD D, L", &CPU::LD_R8_R8},                                                      // 0x55
    {"LD D, [HL]", &CPU::LD_R8_MEM_HL},                                               // 0x56
    {"LD D, A", &CPU::LD_R8_R8},                                                      // 0x57
    {"LD E, B", &CPU::LD_R8_R8},                                                      // 0x58
    {"LD E, C", &CPU::LD_R8_R8},                                                      // 0x59
    {"LD E, D", &CPU::LD_R8_R8},                                                      // 0x5A
    {"LD E, E", &CPU::LD_R8_R8},                                                      // 0x5B
    {"LD E, H", &CPU::LD_R8_R8},                                                      // 0x5C
    {"LD E, L", &CPU::LD_R8_R8},                                                      // 0x5D
    {"LD E, [HL]", &CPU::LD_R8_MEM_HL},                                               // 0x5E
    {"LD E, A", &CPU::LD_R8_R8},                                                      // 0x5F
    {"LD H, B", &CPU::LD_R8_R8},                                                      // 0x60
    {"LD H, C", &CPU::LD_R8_R8},                                                      // 0x61
    {"LD H, D", &CPU::LD_R8_R8},                                                      // 0x62
    {"LD H, E", &CPU::LD_R8_R8},                                                      // 0x63
    {"LD H, H", &CPU::LD_R8_R8},                                                      // 0x64
    {"LD H, L", &CPU::LD_R8_R8},                                                      // 0x65
    {"LD H, [HL]", &CPU::LD_R8_MEM_HL},                                               // 0x66
    {"LD H, A", &CPU::LD_R8_R8},                                                      // 0x67
    {"LD L, B", &CPU::LD_R8_R8},                                                      // 0x68
    {"LD L, C", &CPU::LD_R8_R8},                                                      // 0x69
    {"LD L, D", &CPU::LD_R8_R8},                                                      // 0x6A
    {"LD L, E", &CPU::LD_R8_R8},                                                      // 0x6B
    {"LD L, H", &CPU::LD_R8_R8},                                                      // 0x6C
    {"LD L, L", &CPU::LD_R8_R8},                                                      // 0x6D
    {"LD L, [HL]", &CPU::LD_R8_MEM_HL},                                               // 0x6E
    {"LD L, A", &CPU::LD_R8_R8},                                                      // 0x6F
    {"LD [HL], B", &CPU::LD_MEM_HL_R8},                                               // 0x70
    {"LD [HL], C", &CPU::LD_MEM_HL_R8},                                               // 0x71
    {"LD [HL], D", &CPU::LD_MEM_HL_R8},                                               // 0x72
    {"LD [HL], E", &CPU::LD_MEM_HL_R8},                                               // 0x73
    {"LD [HL], H", &CPU::LD_MEM_HL_R8},                                               // 0x74
    {"LD [HL], L", &CPU::LD_MEM_HL_R8},                                               // 0x75
    {"HALT", &CPU::HALT},                                                             // 0x76
    {"LD [HL], A", &CPU::LD_MEM_HL_R8},                                               // 0x77
    {"LD A, B", &CPU::LD_R8_R8},                                                      // 0x78
    {"LD A, C", &CPU::LD_R8_R8},                                                      // 0x79
    {"LD A, D", &CPU::LD_R8_R8},                                                      // 0x7A
    {"LD A, E", &CPU::LD_R8_R8},                                                      // 0x7B
    {"LD A, H", &CPU::LD_R8_R8},                                                      // 0x7C
    {"LD A, L", &CPU::LD_R8_R8},                                                      // 0x7D
    {"LD A, [HL]", &CPU::LD_R8_MEM_HL},                                               // 0x7E
    {"LD A, A", &CPU::LD_R8_R8},                                                      // 0x7F
    {"ADD A, B", &CPU::ADD_R8},                                                       // 0x80
    {"ADD A, C", &CPU::ADD_R8},                                                       // 0x81
    {"ADD A, D", &CPU::ADD_R8},                                                       // 0x82
    {"ADD A, E", &CPU::ADD_R8},                                                       // 0x83
    {"ADD A, H", &CPU::ADD_R8},                                                       // 0x84
    {"ADD A, L", &CPU::ADD_R8},                                                       // 0x85
    {"ADD A, (HL)", &CPU::ADD_MEM_HL},                                                // 0x86
    {"ADD A, A", &CPU::ADD_R8},                                                       // 0x87
    {"ADC A, B", &CPU::ADC_R8},                                                       // 0x88
    {"ADC A, C", &CPU::ADC_R8},                                                       // 0x89
    {"ADC A, D", &CPU::ADC_R8},                                                       // 0x8A
    {"ADC A, E", &CPU::ADC_R8},                                                       // 0x8B
    {"ADC A, H", &CPU::ADC_R8},                                                       // 0x8C
    {"ADC A, L", &CPU::ADC_R8},                                                       // 0x8D
    {"ADC A, (HL)", &CPU::ADC_MEM_HL},                                                // 0x8E
    {"ADC A, A", &CPU::ADC_R8},                                                       // 0x8F
    {"SUB A, B", &CPU::SUB_R8},                                                       // 0x90
    {"SUB A, C", &CPU::SUB_R8},                                                       // 0x91
    {"SUB A, D", &CPU::SUB_R8},                                                       // 0x92
    {"SUB A, E", &CPU::SUB_R8},                                                       // 0x93
    {"SUB A, H", &CPU::SUB_R8},                                                       // 0x94
    {"SUB A, L", &CPU::SUB_R8},                                                       // 0x95
    {"SUB A, (HL)", &CPU::SUB_MEM_HL},                                                // 0x96
    {"SUB A, A", &CPU::SUB_R8},                                                       // 0x97
    {"SBC A, B", &CPU::SBC_R8},                                                       // 0x98
    {"SBC A, C", &CPU::SBC_R8},                                                       // 0x99
    {"SBC A, D", &CPU::SBC_R8},                                                       // 0x9A
    {"SBC A, E", &CPU::SBC_R8},                                                       // 0x9B
    {"SBC A, H", &CPU::SBC_R8},                                                       // 0x9C
    {"SBC A, L", &CPU::SBC_R8},                                                       // 0x9D
    {"SBC A, (HL)", &CPU::SBC_MEM_HL},                                                // 0x9E
    {"SBC A, A", &CPU::SBC_R8},                                                       // 0x9F
    {"AND A, B", &CPU::AND_R8},                                                       // 0xA0
    {"AND A, C", &CPU::AND_R8},                                                       // 0xA1
    {"AND A, D", &CPU::AND_R8},                                                       // 0xA2
    {"AND A, E", &CPU::AND_R8},                                                       // 0xA3
    {"AND A, H", &CPU::AND_R8},                                                       // 0xA4
    {"AND A, L", &CPU::AND_R8},                                                       // 0xA5
    {"AND A, (HL)", &CPU::AND_MEM_HL},                                                // 0xA6
    {"AND A, A", &CPU::AND_R8},                                                       // 0xA7
    {"XOR A, B", &CPU::XOR_R8},                                                       // 0xA8
    {"XOR A, C", &CPU::XOR_R8},                                                       // 0xA9
    {"XOR A, D", &CPU::XOR_R8},                                                       // 0xAA
    {"XOR A, E", &CPU::XOR_R8},                                                       // 0xAB
    {"XOR A, H", &CPU::XOR_R8},                                                       // 0xAC
    {"XOR A, L", &CPU::XOR_R8},                                                       // 0xAD
    {"XOR A, (HL)", &CPU::XOR_MEM_HL},                                                // 0xAE
    {"XOR A, A", &CPU::XOR_R8},                                                       // 0xAF
    {"OR A, B", &CPU::OR_R8},                                                         // 0xB0
    {"OR A, C", &CPU::OR_R8},                                                         // 0xB1
    {"OR A, D", &CPU::OR_R8},                                                         // 0xB2
    {"OR A, E", &CPU::OR_R8},                                                         // 0xB3
    {"OR A, H", &CPU::OR_R8},                                                         // 0xB4
    {"OR A, L", &CPU::OR_R8},                                                         // 0xB5
    {"OR A, (HL)", &CPU::OR_MEM_HL},                                                  // 0xB6
    {"OR A, A", &CPU::OR_R8},                                                         // 0xB7
    {"CP A, B", &CPU::CP_R8},                                                         // 0xB8
    {"CP A, C", &CPU::CP_R8},                                                         // 0xB9
    {"CP A, D", &CPU::CP_R8},                                                         // 0xBA
    {"CP A, E", &CPU::CP_R8},                                                         // 0xBB
    {"CP A, H", &CPU::CP_R8},                                                         // 0xBC
    {"CP A, L", &CPU::CP_R8},                                                         // 0xBD
    {"CP A, (HL)", &CPU::CP_MEM_HL},                                                  // 0xBE
    {"CP A, A", &CPU::CP_R8},                                                         // 0xBF
    {"RET NZ", &CPU::RET_CC},                                                         // 0xC0
    {"POP BC", &CPU::POP_R16},                                                        // 0xC1
    {"JP NZ, (${:04X})", &CPU::JP_CC_IMM16, AddressingMode::IMMEDIATE_EXTENDED},      // 0xC2
    {"JP (${:04X})", &CPU::JP_IMM16, AddressingMode::IMMEDIATE_EXTENDED},             // 0xC3
    {"CALL NZ, (${:04X})", &CPU::CALL_CC_IMM16, AddressingMode::IMMEDIATE_EXTENDED},  // 0xC4
    {"PUSH BC", &CPU::PUSH_R16},                                                      // 0xC5
    {"ADD A, ${:02X}", &CPU::ADD_IMM8, AddressingMode::IMMEDIATE},                    // 0xC6
    {"RST $00", &CPU::RST_VEC},                                                       // 0xC7
    {"RET Z", &CPU::RET_CC},                                                          // 0xC8
    {"RET", &CPU::RET},                                                               // 0xC9
    {"JP Z, (${:04X})", &CPU::JP_CC_IMM16, AddressingMode::IMMEDIATE_EXTENDED},       // 0xCA
    {"PREFIX", &CPU::PREFIX},                                                         // 0xCB
    {"CALL Z, (${:04X})", &CPU::CALL_CC_IMM16, AddressingMode::IMMEDIATE_EXTENDED},   // 0xCC
    {"CALL (${:04X})", &CPU::CALL_IMM16, AddressingMode::IMMEDIATE_EXTENDED},         // 0xCD
    {"ADC A, ${:02X}", &CPU::ADC_IMM8, AddressingMode::IMMEDIATE},                    // 0xCE
    {"RST $08", &CPU::RST_VEC},                                                       // 0xCF
    {"RET NC", &CPU::RET_CC},                                                         // 0xD0
    {"POP DE", &CPU::POP_R16},                                                        // 0xD1
    {"JP NC, (${:04X})", &CPU::JP_CC_IMM16, AddressingMode::IMMEDIATE_EXTENDED},      // 0xD2
    {},                                                                               // 0xD3
    {"CALL NC, (${:04X})", &CPU::CALL_CC_IMM16, AddressingMode::IMMEDIATE_EXTENDED},  // 0xD4
    {"PUSH DE", &CPU::PUSH_R16},                                                      // 0xD5
    {"SUB A, ${:02X}", &CPU::SUB_IMM8, AddressingMode::IMMEDIATE},                    // 0xD6
    {"RST $10", &CPU::RST_VEC},                                                       // 0xD7
    {"RET C", &CPU::RET_CC},                                                          // 0xD8
    {/* RETI */},                                                                     // 0xD9
    {"JP C, (${:04X})", &CPU::JP_CC_IMM16, AddressingMode::IMMEDIATE_EXTENDED},       // 0xDA
    {},                                                                               // 0xDB
    {"CALL C, (${:04X})", &CPU::CALL_CC_IMM16, AddressingMode::IMMEDIATE_EXTENDED},   // 0xDC
    {},                                                                               // 0xDD
    {"SBC A, ${:02X}", &CPU::SBC_IMM8, AddressingMode::IMMEDIATE},                    // 0xDE
    {"RST $18", &CPU::RST_VEC},                                                       // 0xDF
    {"LDH [${:02X}], A", &CPU::LDH_MEM_16_A, AddressingMode::IMMEDIATE},              // 0xE0
    {"POP HL", &CPU::POP_R16},                                                        // 0xE1
    {"LDH [C], A", &CPU::LDH_MEM_C_A},                                                // 0xE2
    {},                                                                               // 0xE3
    {},                                                                               // 0xE4
    {"PUSH HL", &CPU::PUSH_R16},                                                      // 0xE5
    {"AND ${:02X}", &CPU::AND_IMM8, AddressingMode::IMMEDIATE},                       // 0xE6
    {"RST $20", &CPU::RST_VEC},                                                       // 0xE7
    {"ADD SP, ${:d}", &CPU::ADD_SP_IMM8, AddressingMode::RELATIVE},                   // 0xE8
    {"JP HL", &CPU::JP_HL},                                                           // 0xE9
    {"LD (${:04X}), A", &CPU::LD_MEM_16_A, AddressingMode::IMMEDIATE_EXTENDED},       // 0xEA
    {},                                                                               // 0xEB
    {},                                                                               // 0xEC
    {},                                                                               // 0xED
    {"XOR ${:02X}", &CPU::XOR_IMM8, AddressingMode::IMMEDIATE},                       // 0xEE
    {"RST $28", &CPU::RST_VEC},                                                       // 0xEF
    {"LDH A, (${:02X})", &CPU::LDH_A_MEM_16, AddressingMode::IMMEDIATE},              // 0xF0
    {"POP AF", &CPU::POP_R16},                                                        // 0xF1
    {"LDH A, [C]", &CPU::LDH_A_MEM_C},                                                // 0xF2
    {"NOP", &CPU::NOP},                                                               // 0xF3
    {},                                                                               // 0xF4
    {"PUSH AF", &CPU::PUSH_R16},                                                      // 0xF5
    {"OR ${:02X}", &CPU::OR_IMM8, AddressingMode::IMMEDIATE},                         // 0xF6
    {"RST $30", &CPU::RST_VEC},                                                       // 0xF7
    {"LD HL, SP + ${:d}", &CPU::LD_HL_SP_PLUS_IMM8, AddressingMode::RELATIVE},        // 0xF8
    {"LD SP, HL", &CPU::LD_SP_HL},                                                    // 0xF9
    {"LD A, (${:04X})", &CPU::LD_A_MEM_16, AddressingMode::IMMEDIATE_EXTENDED},       // 0xFA
    {"NOP", &CPU::NOP},                                                               // 0xFB
    {},                                                                               // 0xFC
    {},                                                                               // 0xFD
    {"CP ${:02X}", &CPU::CP_IMM8, AddressingMode::IMMEDIATE},                         // 0xFE
    {"RST $38", &CPU::RST_VEC},                                                       // 0xFF
}};

const CPU::InstructionLookupTable CPU::prefixed_instruction_lookup{{
    {"RLC B", &CPU::RLC_R8},            // 0x00
    {"RLC C", &CPU::RLC_R8},            // 0x01
    {"RLC D", &CPU::RLC_R8},            // 0x02
    {"RLC E", &CPU::RLC_R8},            // 0x03
    {"RLC H", &CPU::RLC_R8},            // 0x04
    {"RLC L", &CPU::RLC_R8},            // 0x05
    {"RLC (HL)", &CPU::RLC_MEM_HL},     // 0x06
    {"RLC A", &CPU::RLC_R8},            // 0x07
    {"RRC B", &CPU::RRC_R8},            // 0x08
    {"RRC C", &CPU::RRC_R8},            // 0x09
    {"RRC D", &CPU::RRC_R8},            // 0x0A
    {"RRC E", &CPU::RRC_R8},            // 0x0B
    {"RRC H", &CPU::RRC_R8},            // 0x0C
    {"RRC L", &CPU::RRC_R8},            // 0x0D
    {"RRC (HL)", &CPU::RRC_MEM_HL},     // 0x0E
    {"RRC A", &CPU::RRC_R8},            // 0x0F
    {"RL B", &CPU::RL_R8},              // 0x10
    {"RL C", &CPU::RL_R8},              // 0x11
    {"RL D", &CPU::RL_R8},              // 0x12
    {"RL E", &CPU::RL_R8},              // 0x13
    {"RL H", &CPU::RL_R8},              // 0x14
    {"RL L", &CPU::RL_R8},              // 0x15
    {"RL (HL)", &CPU::RL_MEM_HL},       // 0x16
    {"RL A", &CPU::RL_R8},              // 0x17
    {"RR B", &CPU::RR_R8},              // 0x18
    {"RR C", &CPU::RR_R8},              // 0x19
    {"RR D", &CPU::RR_R8},              // 0x1A
    {"RR E", &CPU::RR_R8},              // 0x1B
    {"RR H", &CPU::RR_R8},              // 0x1C
    {"RR L", &CPU::RR_R8},              // 0x1D
    {"RR (HL)", &CPU::RR_MEM_HL},       // 0x1E
    {"RR A", &CPU::RR_R8},              // 0x1F
    {"SLA B", &CPU::SLA_R8},            // 0x20
    {"SLA C", &CPU::SLA_R8},            // 0x21
    {"SLA D", &CPU::SLA_R8},            // 0x22
    {"SLA E", &CPU::SLA_R8},            // 0x23
    {"SLA H", &CPU::SLA_R8},            // 0x24
    {"SLA L", &CPU::SLA_R8},            // 0x25
    {"SLA (HL)", &CPU::SLA_MEM_HL},     // 0x26
    {"SLA A", &CPU::SLA_R8},            // 0x27
    {"SRA B", &CPU::SRA_R8},            // 0x28
    {"SRA C", &CPU::SRA_R8},            // 0x29
    {"SRA D", &CPU::SRA_R8},            // 0x2A
    {"SRA E", &CPU::SRA_R8},            // 0x2B
    {"SRA H", &CPU::SRA_R8},            // 0x2C
    {"SRA L", &CPU::SRA_R8},            // 0x2D
    {"SRA (HL)", &CPU::SRA_MEM_HL},     // 0x2E
    {"SRA A", &CPU::SRA_R8},            // 0x2F
    {"SWAP B", &CPU::SWAP_R8},          // 0x30
    {"SWAP C", &CPU::SWAP_R8},          // 0x31
    {"SWAP D", &CPU::SWAP_R8},          // 0x32
    {"SWAP E", &CPU::SWAP_R8},          // 0x33
    {"SWAP H", &CPU::SWAP_R8},          // 0x34
    {"SWAP L", &CPU::SWAP_R8},          // 0x35
    {"SWAP (HL)", &CPU::SWAP_MEM_HL},   // 0x36
    {"SWAP A", &CPU::SWAP_R8},          // 0x37
    {"SRL B", &CPU::SRL_R8},            // 0x38
    {"SRL C", &CPU::SRL_R8},            // 0x39
    {"SRL D", &CPU::SRL_R8},            // 0x3A
    {"SRL E", &CPU::SRL_R8},            // 0x3B
    {"SRL H", &CPU::SRL_R8},            // 0x3C
    {"SRL L", &CPU::SRL_R8},            // 0x3D
    {"SRL (HL)", &CPU::SRL_MEM_HL},     // 0x3E
    {"SRL A", &CPU::SRL_R8},            // 0x3F
    {"BIT 0, B", &CPU::BIT_R8},         // 0x40
    {"BIT 0, C", &CPU::BIT_R8},         // 0x41
    {"BIT 0, D", &CPU::BIT_R8},         // 0x42
    {"BIT 0, E", &CPU::BIT_R8},         // 0x43
    {"BIT 0, H", &CPU::BIT_R8},         // 0x44
    {"BIT 0, L", &CPU::BIT_R8},         // 0x45
    {"BIT 0, (HL)", &CPU::BIT_MEM_HL},  // 0x46
    {"BIT 0, A", &CPU::BIT_R8},         // 0x47
    {"BIT 1, B", &CPU::BIT_R8},         // 0x48
    {"BIT 1, C", &CPU::BIT_R8},         // 0x49
    {"BIT 1, D", &CPU::BIT_R8},         // 0x4A
    {"BIT 1, E", &CPU::BIT_R8},         // 0x4B
    {"BIT 1, H", &CPU::BIT_R8},         // 0x4C
    {"BIT 1, L", &CPU::BIT_R8},         // 0x4D
    {"BIT 1, (HL)", &CPU::BIT_MEM_HL},  // 0x4E
    {"BIT 1, A", &CPU::BIT_R8},         // 0x4F
    {"BIT 2, B", &CPU::BIT_R8},         // 0x50
    {"BIT 2, C", &CPU::BIT_R8},         // 0x51
    {"BIT 2, D", &CPU::BIT_R8},         // 0x52
    {"BIT 2, E", &CPU::BIT_R8},         // 0x53
    {"BIT 2, H", &CPU::BIT_R8},         // 0x54
    {"BIT 2, L", &CPU::BIT_R8},         // 0x55
    {"BIT 2, (HL)", &CPU::BIT_MEM_HL},  // 0x56
    {"BIT 2, A", &CPU::BIT_R8},         // 0x57
    {"BIT 3, B", &CPU::BIT_R8},         // 0x58
    {"BIT 3, C", &CPU::BIT_R8},         // 0x59
    {"BIT 3, D", &CPU::BIT_R8},         // 0x5A
    {"BIT 3, E", &CPU::BIT_R8},         // 0x5B
    {"BIT 3, H", &CPU::BIT_R8},         // 0x5C
    {"BIT 3, L", &CPU::BIT_R8},         // 0x5D
    {"BIT 3, (HL)", &CPU::BIT_MEM_HL},  // 0x5E
    {"BIT 3, A", &CPU::BIT_R8},         // 0x5F
    {"BIT 4, B", &CPU::BIT_R8},         // 0x60
    {"BIT 4, C", &CPU::BIT_R8},         // 0x61
    {"BIT 4, D", &CPU::BIT_R8},         // 0x62
    {"BIT 4, E", &CPU::BIT_R8},         // 0x63
    {"BIT 4, H", &CPU::BIT_R8},         // 0x64
    {"BIT 4, L", &CPU::BIT_R8},         // 0x65
    {"BIT 4, (HL)", &CPU::BIT_MEM_HL},  // 0x66
    {"BIT 4, A", &CPU::BIT_R8},         // 0x67
    {"BIT 5, B", &CPU::BIT_R8},         // 0x68
    {"BIT 5, C", &CPU::BIT_R8},         // 0x69
    {"BIT 5, D", &CPU::BIT_R8},         // 0x6A
    {"BIT 5, E", &CPU::BIT_R8},         // 0x6B
    {"BIT 5, H", &CPU::BIT_R8},         // 0x6C
    {"BIT 5, L", &CPU::BIT_R8},         // 0x6D
    {"BIT 5, (HL)", &CPU::BIT_MEM_HL},  // 0x6E
    {"BIT 5, A", &CPU::BIT_R8},         // 0x6F
    {"BIT 6, B", &CPU::BIT_R8},         // 0x70
    {"BIT 6, C", &CPU::BIT_R8},         // 0x71
    {"BIT 6, D", &CPU::BIT_R8},         // 0x72
    {"BIT 6, E", &CPU::BIT_R8},         // 0x73
    {"BIT 6, H", &CPU::BIT_R8},         // 0x74
    {"BIT 6, L", &CPU::BIT_R8},         // 0x75
    {"BIT 6, (HL)", &CPU::BIT_MEM_HL},  // 0x76
    {"BIT 6, A", &CPU::BIT_R8},         // 0x77
    {"BIT 7, B", &CPU::BIT_R8},         // 0x78
    {"BIT 7, C", &CPU::BIT_R8},         // 0x79
    {"BIT 7, D", &CPU::BIT_R8},         // 0x7A
    {"BIT 7, E", &CPU::BIT_R8},         // 0x7B
    {"BIT 7, H", &CPU::BIT_R8},         // 0x7C
    {"BIT 7, L", &CPU::BIT_R8},         // 0x7D
    {"BIT 7, (HL)", &CPU::BIT_MEM_HL},  // 0x7E
    {"BIT 7, A", &CPU::BIT_R8},         // 0x7F
    {"RES 0, B", &CPU::RES_R8},         // 0x80
    {"RES 0, C", &CPU::RES_R8},         // 0x81
    {"RES 0, D", &CPU::RES_R8},         // 0x82
    {"RES 0, E", &CPU::RES_R8},         // 0x83
    {"RES 0, H", &CPU::RES_R8},         // 0x84
    {"RES 0, L", &CPU::RES_R8},         // 0x85
    {"RES 0, (HL)", &CPU::RES_MEM_HL},  // 0x86
    {"RES 0, A", &CPU::RES_R8},         // 0x87
    {"RES 1, B", &CPU::RES_R8},         // 0x88
    {"RES 1, C", &CPU::RES_R8},         // 0x89
    {"RES 1, D", &CPU::RES_R8},         // 0x8A
    {"RES 1, E", &CPU::RES_R8},         // 0x8B
    {"RES 1, H", &CPU::RES_R8},         // 0x8C
    {"RES 1, L", &CPU::RES_R8},         // 0x8D
    {"RES 1, (HL)", &CPU::RES_MEM_HL},  // 0x8E
    {"RES 1, A", &CPU::RES_R8},         // 0x8F
    {"RES 2, B", &CPU::RES_R8},         // 0x90
    {"RES 2, C", &CPU::RES_R8},         // 0x91
    {"RES 2, D", &CPU::RES_R8},         // 0x92
    {"RES 2, E", &CPU::RES_R8},         // 0x93
    {"RES 2, H", &CPU::RES_R8},         // 0x94
    {"RES 2, L", &CPU::RES_R8},         // 0x95
    {"RES 2, (HL)", &CPU::RES_MEM_HL},  // 0x96
    {"RES 2, A", &CPU::RES_R8},         // 0x97
    {"RES 3, B", &CPU::RES_R8},         // 0x98
    {"RES 3, C", &CPU::RES_R8},         // 0x99
    {"RES 3, D", &CPU::RES_R8},         // 0x9A
    {"RES 3, E", &CPU::RES_R8},         // 0x9B
    {"RES 3, H", &CPU::RES_R8},         // 0x9C
    {"RES 3, L", &CPU::RES_R8},         // 0x9D
    {"RES 3, (HL)", &CPU::RES_MEM_HL},  // 0x9E
    {"RES 3, A", &CPU::RES_R8},         // 0x9F
    {"RES 4, B", &CPU::RES_R8},         // 0xA0
    {"RES 4, C", &CPU::RES_R8},         // 0xA1
    {"RES 4, D", &CPU::RES_R8},         // 0xA2
    {"RES 4, E", &CPU::RES_R8},         // 0xA3
    {"RES 4, H", &CPU::RES_R8},         // 0xA4
    {"RES 4, L", &CPU::RES_R8},         // 0xA5
    {"RES 4, (HL)", &CPU::RES_MEM_HL},  // 0xA6
    {"RES 4, A", &CPU::RES_R8},         // 0xA7
    {"RES 5, B", &CPU::RES_R8},         // 0xA8
    {"RES 5, C", &CPU::RES_R8},         // 0xA9
    {"RES 5, D", &CPU::RES_R8},         // 0xAA
    {"RES 5, E", &CPU::RES_R8},         // 0xAB
    {"RES 5, H", &CPU::RES_R8},         // 0xAC
    {"RES 5, L", &CPU::RES_R8},         // 0xAD
    {"RES 5, (HL)", &CPU::RES_MEM_HL},  // 0xAE
    {"RES 5, A", &CPU::RES_R8},         // 0xAF
    {"RES 6, B", &CPU::RES_R8},         // 0xB0
    {"RES 6, C", &CPU::RES_R8},         // 0xB1
    {"RES 6, D", &CPU::RES_R8},         // 0xB2
    {"RES 6, E", &CPU::RES_R8},         // 0xB3
    {"RES 6, H", &CPU::RES_R8},         // 0xB4
    {"RES 6, L", &CPU::RES_R8},         // 0xB5
    {"RES 6, (HL)", &CPU::RES_MEM_HL},  // 0xB6
    {"RES 6, A", &CPU::RES_R8},         // 0xB7
    {"RES 7, B", &CPU::RES_R8},         // 0xB8
    {"RES 7, C", &CPU::RES_R8},         // 0xB9
    {"RES 7, D", &CPU::RES_R8},         // 0xBA
    {"RES 7, E", &CPU::RES_R8},         // 0xBB
    {"RES 7, H", &CPU::RES_R8},         // 0xBC
    {"RES 7, L", &CPU::RES_R8},         // 0xBD
    {"RES 7, (HL)", &CPU::RES_MEM_HL},  // 0xBE
    {"RES 7, A", &CPU::RES_R8},         // 0xBF
    {"SET 0, B", &CPU::SET_R8},         // 0xC0
    {"SET 0, C", &CPU::SET_R8},         // 0xC1
    {"SET 0, D", &CPU::SET_R8},         // 0xC2
    {"SET 0, E", &CPU::SET_R8},         // 0xC3
    {"SET 0, H", &CPU::SET_R8},         // 0xC4
    {"SET 0, L", &CPU::SET_R8},         // 0xC5
    {"SET 0, (HL)", &CPU::SET_MEM_HL},  // 0xC6
    {"SET 0, A", &CPU::SET_R8},         // 0xC7
    {"SET 1, B", &CPU::SET_R8},         // 0xC8
    {"SET 1, C", &CPU::SET_R8},         // 0xC9
    {"SET 1, D", &CPU::SET_R8},         // 0xCA
    {"SET 1, E", &CPU::SET_R8},         // 0xCB
    {"SET 1, H", &CPU::SET_R8},         // 0xCC
    {"SET 1, L", &CPU::SET_R8},         // 0xCD
    {"SET 1, (HL)", &CPU::SET_MEM_HL},  // 0xCE
    {"SET 1, A", &CPU::SET_R8},         // 0xCF
    {"SET 2, B", &CPU::SET_R8},         // 0xD0
    {"SET 2, C", &CPU::SET_R8},         // 0xD1
    {"SET 2, D", &CPU::SET_R8},         // 0xD2
    {"SET 2, E", &CPU::SET_R8},         // 0xD3
    {"SET 2, H", &CPU::SET_R8},         // 0xD4
    {"SET 2, L", &CPU::SET_R8},         // 0xD5
    {"SET 2, (HL)", &CPU::SET_MEM_HL},  // 0xD6
    {"SET 2, A", &CPU::SET_R8},         // 0xD7
    {"SET 3, B", &CPU::SET_R8},         // 0xD8
    {"SET 3, C", &CPU::SET_R8},         // 0xD9
    {"SET 3, D", &CPU::SET_R8},         // 0xDA
    {"SET 3, E", &CPU::SET_R8},         // 0xDB
    {"SET 3, H", &CPU::SET_R8},         // 0xDC
    {"SET 3, L", &CPU::SET_R8},         // 0xDD
    {"SET 3, (HL)", &CPU::SET_MEM_HL},  // 0xDE
    {"SET 3, A", &CPU::SET_R8},         // 0xDF
    {"SET 4, B", &CPU::SET_R8},         // 0xE0
    {"SET 4, C", &CPU::SET_R8},         // 0xE1
    {"SET 4, D", &CPU::SET_R8},         // 0xE2
    {"SET 4, E", &CPU::SET_R8},         // 0xE3
    {"SET 4, H", &CPU::SET_R8},         // 0xE4
    {"SET 4, L", &CPU::SET_R8},         // 0xE5
    {"SET 4, (HL)", &CPU::SET_MEM_HL},  // 0xE6
    {"SET 4, A", &CPU::SET_R8},         // 0xE7
    {"SET 5, B", &CPU::SET_R8},         // 0xE8
    {"SET 5, C", &CPU::SET_R8},         // 0xE9
    {"SET 5, D", &CPU::SET_R8},         // 0xEA
    {"SET 5, E", &CPU::SET_R8},         // 0xEB
    {"SET 5, H", &CPU::SET_R8},         // 0xEC
    {"SET 5, L", &CPU::SET_R8},         // 0xED
    {"SET 5, (HL)", &CPU::SET_MEM_HL},  // 0xEE
    {"SET 5, A", &CPU::SET_R8},         // 0xEF
    {"SET 6, B", &CPU::SET_R8},         // 0xF0
    {"SET 6, C", &CPU::SET_R8},         // 0xF1
    {"SET 6, D", &CPU::SET_R8},         // 0xF2
    {"SET 6, E", &CPU::SET_R8},         // 0xF3
    {"SET 6, H", &CPU::SET_R8},         // 0xF4
    {"SET 6, L", &CPU::SET_R8},         // 0xF5
    {"SET 6, (HL)", &CPU::SET_MEM_HL},  // 0xF6
    {"SET 6, A", &CPU::SET_R8},         // 0xF7
    {"SET 7, B", &CPU::SET_R8},         // 0xF8
    {"SET 7, C", &CPU::SET_R8},         // 0xF9
    {"SET 7, D", &CPU::SET_R8},         // 0xFA
    {"SET 7, E", &CPU::SET_R8},         // 0xFB
    {"SET 7, H", &CPU::SET_R8},         // 0xFC
    {"SET 7, L", &CPU::SET_R8},         // 0xFD
    {"SET 7, (HL)", &CPU::SET_MEM_HL},  // 0xFE
    {"SET 7, A", &CPU::SET_R8}          // 0xFF
}};

void CPU::NOP()
{
    (void) this->opcode;
}

void CPU::STOP() {}

void CPU::HALT() {}

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

void CPU::LD_SP_HL()
{
    this->micro_ops.emplace([this] { this->reg.u16.SP = this->reg.u16.HL; });
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
            this->bus.write(this->tmp.WZ, this->reg.u8.A);
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

void CPU::LD_MEM_16_A()
{
    this->micro_ops.emplace([this] { this->tmp.Z = this->bus.read(this->reg.u16.PC++); });
    this->micro_ops.emplace([this] { this->tmp.W = this->bus.read(this->reg.u16.PC++); });
    this->micro_ops.emplace([this] { this->bus.write(this->tmp.WZ, this->reg.u8.A); });
}

void CPU::LD_A_MEM_16()
{
    this->micro_ops.emplace([this] { this->tmp.Z = this->bus.read(this->reg.u16.PC++); });
    this->micro_ops.emplace([this] { this->tmp.W = this->bus.read(this->reg.u16.PC++); });
    this->micro_ops.emplace([this] { this->reg.u8.A = this->bus.read(this->tmp.WZ); });
}

void CPU::LD_HL_SP_PLUS_IMM8()
{
    this->micro_ops.emplace([this] { this->tmp.Z = this->bus.read(this->reg.u16.PC++); });
    this->micro_ops.emplace([this]
                            { this->reg.u16.HL = this->ADD_16(this->reg.u16.HL, static_cast<int8_t>(this->tmp.Z)); });
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

    result32 = a + b;
    this->set_carry((result32 & 0x10000) == 0x10000);
    this->set_half_carry(((a & 0xFFF) + (b & 0xFFF) & 0x1000) == 0x1000);
    this->set_subtract(false);

    return static_cast<uint16_t>(result32);
}

uint16_t CPU::ADD_16(const uint16_t a, const int8_t b)
{
    uint32_t result32{};

    result32 = a + b;
    this->set_carry((result32 & 0x10000) == 0x10000);
    this->set_half_carry(((a & 0xFFF) + (b & 0xFFF) & 0x1000) == 0x1000);
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
    result16 = a + b + add_carry;
    result8  = static_cast<uint8_t>(result16);
    this->set_half_carry(((a & 0xF) + (b & 0XF) + add_carry & 0x10) == 0x10);
    this->set_carry((result16 & 0x100) == 0x100);
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
    result = a - b - sub_carry;

    this->set_zero(result == 0);
    this->set_subtract(true);
    this->set_half_carry((a & 0x0F) < (b & 0x0F) + sub_carry);
    this->set_carry(a < b + sub_carry);

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
        this->set_half_carry((value & 0x0F) == 0);
    }
    else
    {
        value -= 1;
        this->set_half_carry((value & 0x0F) == 0x0F);
    }
    this->set_subtract(type == StepType::DECREMENT);
    this->set_zero(value == 0);
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
    this->prefixed = true;
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
    auto          has_new_carry = false;
    const uint8_t msb           = val & 0x80;
    const uint8_t lsb           = val & 0x01;

    if (rotate_direction == RotateDirection::LEFT)
    {
        has_new_carry = msb == 0x80;
        val <<= 1;
        if (rotate_through_carry)
        {
            if (this->carry())
            {
                val |= 0x01;
            }
        }
    }
    else if (rotate_direction == RotateDirection::RIGHT)
    {
        has_new_carry = lsb == 0x01;
        val >>= 1;
        if (rotate_through_carry)
        {
            if (this->carry())
            {
                val |= 0x80;
            }
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

auto CPU::SHIFT(uint8_t val, const ShiftDirection shift_direction, const ShiftType shift_type) noexcept
{
    const uint8_t msb = val & 0x80;
    const uint8_t lsb = val & 0x01;

    if (shift_direction == ShiftDirection::LEFT)
    {
        /* Logical and arithmetic left shift are equivalent. */
        (void) shift_type;

        this->set_carry(msb == 0x80);
        val <<= 1;
    }
    else if (shift_direction == ShiftDirection::RIGHT)
    {
        this->set_carry(lsb == 0x01);
        val >>= 1;
        if (shift_type == ShiftType::ARITHMETIC)
        {
            val |= msb;
        }
    }
    this->set_zero(val == 0);
    this->set_subtract(false);
    this->set_half_carry(false);

    return val;
}

void CPU::SRA_R8()
{
    const auto src{this->get_register8(Register8Position::RIGHTMOST)};
    this->reg.u8.*src = this->SHIFT(this->reg.u8.*src, ShiftDirection::RIGHT, ShiftType::ARITHMETIC);
}

void CPU::SRA_MEM_HL()
{
    this->micro_ops.emplace([this] { this->tmp.Z = this->bus.read(this->reg.u16.HL); });
    this->micro_ops.emplace(
        [this]
        {
            this->tmp.Z = this->SHIFT(this->tmp.Z, ShiftDirection::RIGHT, ShiftType::ARITHMETIC);
            this->bus.write(this->reg.u16.HL, this->tmp.Z);
        });
}

void CPU::SLA_R8()
{
    const auto src{this->get_register8(Register8Position::RIGHTMOST)};
    this->reg.u8.*src = this->SHIFT(this->reg.u8.*src, ShiftDirection::LEFT, ShiftType::ARITHMETIC);
}

void CPU::SLA_MEM_HL()
{
    this->micro_ops.emplace([this] { this->tmp.Z = this->bus.read(this->reg.u16.HL); });
    this->micro_ops.emplace(
        [this]
        {
            this->tmp.Z = this->SHIFT(this->tmp.Z, ShiftDirection::LEFT, ShiftType::ARITHMETIC);
            this->bus.write(this->reg.u16.HL, this->tmp.Z);
        });
}

void CPU::SRL_R8()
{
    const auto src{this->get_register8(Register8Position::RIGHTMOST)};
    this->reg.u8.*src = this->SHIFT(this->reg.u8.*src, ShiftDirection::RIGHT, ShiftType::LOGICAL);
}

void CPU::SRL_MEM_HL()
{
    this->micro_ops.emplace([this] { this->tmp.Z = this->bus.read(this->reg.u16.HL); });
    this->micro_ops.emplace(
        [this]
        {
            this->tmp.Z = this->SHIFT(this->tmp.Z, ShiftDirection::RIGHT, ShiftType::LOGICAL);
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