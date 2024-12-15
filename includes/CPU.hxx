#ifndef CPU_H
#define CPU_H

#include <array>
#include <functional>
#include <string>

#include "Bus.hxx"

#define REG16_PAIR_GET(r1, r2) (static_cast<uint16_t>((r1 << 8) | r2))
#define REG16_PAIR_SET(val, r1, r2) (r1 = static_cast<uint8_t>(val >> 8), r2 = static_cast<uint8_t>(val & 0xFF))

class CPU
{
  public:
    struct Instruction
    {
        size_t cycles;
        void (CPU::*op)();

        constexpr Instruction();
        constexpr Instruction(size_t cycles, void (CPU::*op)());

        constexpr static Instruction LD_R8_R8();
        constexpr static Instruction LD_R16_IMM16();
        constexpr static Instruction LD_R8_IMM8();
        constexpr static Instruction LD_R8_MEM_HL();
        constexpr static Instruction LD_MEM_HL_R8();

        constexpr static Instruction AND_R8();
        constexpr static Instruction XOR_R8();
        constexpr static Instruction OR_R8();
        constexpr static Instruction ILL();
        constexpr static Instruction PREFIX();

        constexpr static Instruction RRC_R8();
        constexpr static Instruction RRC_MEM_HL();
        constexpr static Instruction RLC_R8();
        constexpr static Instruction RLC_MEM_HL();
        constexpr static Instruction RR_R8();
        constexpr static Instruction RR_MEM_HL();
        constexpr static Instruction RL_R8();
        constexpr static Instruction RL_MEM_HL();

        constexpr static Instruction SLA_R8();
        constexpr static Instruction SLA_MEM_HL();
        constexpr static Instruction SRL_R8();
        constexpr static Instruction SRL_MEM_HL();
        constexpr static Instruction SRA_R8();
        constexpr static Instruction SRA_MEM_HL();

        constexpr static Instruction SWAP_R8();
        constexpr static Instruction SWAP_MEM_HL();

        constexpr static Instruction BIT_R8();
        constexpr static Instruction BIT_MEM_HL();

        constexpr static Instruction RES_R8();
        constexpr static Instruction RES_MEM_HL();

        constexpr static Instruction SET_R8();
        constexpr static Instruction SET_MEM_HL();
        bool                         operator==(const Instruction& prefix) const = default;
    };

    using InstructionLookupTable = std::array<Instruction, 0x100>;

    /**
     * @brief Instruction lookup table.
     */
    const static InstructionLookupTable inst_lookup;
    const static InstructionLookupTable cb_prefixed_inst_lookup;

    struct Register
    {
        uint8_t A, F;
        uint8_t B, C;
        uint8_t D, E;
        uint8_t H, L;

        uint16_t PC;
        uint16_t SP;
    };

    using Register8  = uint8_t   Register::*;
    using Register16 = uint16_t Register::*;

    class BadRegister final : public std::exception
    {
    };

    class IllegalInstruction final : public std::exception
    {
    };

    class InstructionNotImplemented final : public std::exception
    {
    };

    explicit CPU(Bus& bus);
    ~CPU();

    void cycle();

    [[nodiscard]] Register get_register() const noexcept;

  private:
    enum class OperandRegister16 : uint8_t
    {
        BC = 0b00,
        DE = 0b01,
        HL = 0b10,
        SP = 0b11,
    };

    enum class OperandRegister8 : uint8_t
    {
        A = 0b111,
        B = 0b000,
        C = 0b001,
        D = 0b010,
        E = 0b011,
        H = 0b100,
        L = 0b101
    };

    enum class InstructionBit : uint8_t
    {
        ZERO  = 0b000,
        ONE   = 0b001,
        TWO   = 0b010,
        THREE = 0b011,
        FOUR  = 0b100,
        FIVE  = 0b101,
        SIX   = 0b110,
        SEVEN = 0b111,
    };

    enum class ShiftDirection : bool
    {
        RIGHT,
        LEFT
    };

    enum class RotateDirection : bool
    {
        RIGHT,
        LEFT
    };

    enum class ShiftType : bool
    {
        LOGICAL,
        ARITHMETIC
    };

    struct Flags
    {
        enum Value : uint8_t
        {
            ZERO       = 1 << 7,
            SUBTRACT   = 1 << 6,
            HALF_CARRY = 1 << 5,
            CARRY      = 1 << 4
        };
    };

    static Register8 get_register8(OperandRegister8 reg);

    void set_register_r16_imm16(OperandRegister16 reg, uint16_t value);
    void set_register_r8_imm8(OperandRegister8 reg, uint8_t value);
    void set_register_r8_r8(OperandRegister8 reg_dest, OperandRegister8 reg_src);

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
    /**
     * @brief Load from memory pointed by HL to 8-bit register.
     */
    void LD_R8_MEM_HL();
    /**
     * @brief Load from 8-bit register to memory pointed by HL.
     */
    void LD_MEM_HL_R8();

    void LD_A_MEM_16();

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
     * @brief Increment by one an 8-bit register.
     */
    void INC_R8();

    /**
     * @brief CB Prefix.
     */
    void PREFIX();

    /**
     * @brief Perform an 8-bit rotate through the carry flag.
     * @return Operation result.
     */
    auto ROTATE(uint8_t val, RotateDirection rotate_direction, bool rotate_through_carry) noexcept;
    /**
     * @brief Rotate register r8 left.
     */
    void RLC_R8();
    /**
     * @brief Rotate the byte pointed to by HL left.
     */
    void RLC_MEM_HL();
    /**
     * @brief Rotate register r8 right.
     */
    void RRC_R8();
    /**
     * @brief  Rotate the byte pointed to by HL right.
     */
    void RRC_MEM_HL();
    /**
     * @brief Rotate bits in register r8 left, through the carry flag.
     */
    void RL_R8();
    /**
     * @brief Rotate the byte pointed to by HL left, through the carry flag.
     */
    void RL_MEM_HL();
    /**
     * @brief Rotate register r8 right, through the carry flag.
     */
    void RR_R8();
    /**
     * @brief Rotate the byte pointed to by HL right, through the carry flag.
     */
    void RR_MEM_HL();

    /**
     * @brief Perform an 8 bit shift.
     * @return Operation result.
     */
    auto SHIFT(uint8_t val, ShiftType shift_type, ShiftDirection shift_direction) noexcept;
    /**
     * @brief Shift Right Arithmetically register r8.
     */
    void SRA_R8();
    /**
     * @brief Shift Right Arithmetically the byte pointed to by HL.
     */
    void SRA_MEM_HL();
    /**
     * @brief Shift Left Arithmetically register r8.
     */
    void SLA_R8();
    /**
     * @brief Shift Left Arithmetically the byte pointed to by HL.
     */
    void SLA_MEM_HL();
    /**
     * @brief Shift Right Logically register r8.
     */
    void SRL_R8();
    /**
     * @brief Shift Right Logically the byte pointed to by HL.
     */
    void SRL_MEM_HL();

    /**
     * @brief Test bit in an 8-bit register, set the zero flag if bit not set.
     */
    void BIT_R8();
    /**
     * @brief Test bit in the byte pointed by HL, set the zero flag if bit not set.
     */
    void BIT_MEM_HL();

    /**
     * @brief Swap the upper 4 bits in register r8 and the lower 4 ones.
     */
    void SWAP_R8();

    /**
     * @brief Swap the upper 4 bits in the byte pointed by HL and the lower 4 ones.
     */
    void SWAP_MEM_HL();

    /**
     * @brief Set bit u3 in register r8 to 0. Bit 0 is the rightmost one, bit 7 the leftmost one.
     */
    void RES_R8();

    /**
     * @brief Set bit u3 in the byte pointed by HL to 0. Bit 0 is the rightmost one, bit 7 the
     * leftmost one.
     */
    void RES_MEM_HL();

    /**
     * @brief Set bit u3 in the byte pointed by HL to 1. Bit 0 is the rightmost one, bit 7 the
     * leftmost one.
     */
    void SET_R8();

    /**
     * @brief Set bit u3 in the byte pointed by HL to 1. Bit 0 is the rightmost one, bit 7 the
     * leftmost one.
     */
    void SET_MEM_HL();

    /**
     * @brief General Purpose Registers
     */
    Register reg;

    /**
     * @brief Tracks the remaining machine cycles for the current instruction execution.
     */
    uint8_t cycles;
    /**
     * @brief Represents the current operation code being executed.
     */
    uint8_t opcode;

    bool cb_prefixed;

    /**
     * @brief Current instruction.
     */
    Instruction inst;

    Bus& bus;
};

#endif