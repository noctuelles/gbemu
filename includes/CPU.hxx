#ifndef CPU_H
#define CPU_H

#include <array>
#include <functional>
#include <string>

#include "Bus.hxx"

#define REG16_PAIR_GET(r1, r2) (static_cast<uint16_t>((r1 << 8) | r2))
#define REG16_PAIR_SET(val, r1, r2) \
    (r1 = static_cast<uint8_t>(val >> 8), r2 = static_cast<uint8_t>(val & 0xFF))

class CPU
{
  public:
    using Reg = uint8_t CPU::*;
    using RegPair = std::pair<uint8_t CPU::*, uint8_t CPU::*>;

    struct Instruction
    {
        std::string name;
        size_t      cycles;
        void (CPU::*op)();

        static constexpr Instruction LD_R8_R8();
        static constexpr Instruction LD_R16_IMM16();
        static constexpr Instruction LD_R8_IMM8();
        static constexpr Instruction AND_R8();
        static constexpr Instruction XOR_R8();
        static constexpr Instruction OR_R8();
        static constexpr Instruction ILL();
    };

    template <typename RegType>
    struct RegisterDescription
    {
        std::string name;
        RegType     reg;
    };

    class BadRegisterException final : public std::exception
    {
    };

    class IllegalInstructionException final : public std::exception
    {
    };

    enum Flags
    {
        ZERO       = 1 << 7,
        SUBTRACT   = 1 << 6,
        HALF_CARRY = 1 << 5,
        CARRY      = 1 << 4
    };

    explicit CPU(Bus& bus);
    ~CPU();

    void cycle();

    const std::string& get_str_inst() const;

  private:
    void build_str_inst_r8(const std::string& reg);
    void build_str_inst_r8_xxx(const std::string& reg_dest, const std::string& val);

    void NOP();
    /**
     * @brief Illegal instruction.
     */
    void ILL();

    /**
     * @brief Load from 8-bit register to 8-bit register.
     */
    void LD_R8_R8();
    /**
     * @brief Load from 8-bit immediate to 8-bit register.
     */
    void LD_R8_IMM8();
    /**
     * @brief Load from 16-bit immediate to 16-bit register.
     */
    void LD_R16_IMM16();
    void LD_R8_MEM_HL();

    /**
     * @brief AND from 8-bit register to register A.
     */
    void AND_R8();
    /**
     * @brief AND from 8-bit register to register A.
     */
    void OR_R8();
    /**
     * @brief AND from 8-bit register to register A.
     */
    void XOR_R8();
    /**
     * @brief ADD from 8-bit register to register A.
     */
    void ADD_R8();
    /**
     * @brief Increment by one a 8-bit register.
     */
    void INC_R8();

    /**
     * @brief General Purpose Registers
     */

    uint8_t A, F;
    uint8_t B, C;
    uint8_t D, E;
    uint8_t H, L;

    uint16_t PC;
    uint16_t SP;

    /**
     * @brief Tracks the remaining machine cycles for the current instruction execution.
     */
    uint8_t cycles;
    /**
     * @brief Represents the current operation code being executed.
     */
    uint8_t opcode;

    /**
     * @brief Current instruction.
     */
    Instruction inst;
    /**
     * @brief String representation of the current instruction.
     */
    std::string str_inst;
    /**
     * @brief Instruction lookup table.
     */
    const static std::array<Instruction, 0x100> inst_lookup;
    /**
     * @brief Lookup table for 8-bit register.
     */
    const static std::array<RegisterDescription<Reg>, 0x08> r8_lookup;
    /**
     * @brief Lookup table for 8-bit register.
     */
    const static std::array<RegisterDescription<RegPair>, 0x04> r16_lookup;

    Bus& bus;
};

#endif