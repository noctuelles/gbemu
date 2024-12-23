#ifndef CPU_H
#define CPU_H

#include <array>
#include <format>
#include <functional>
#include <queue>
#include <stack>
#include <stdexcept>

#include "Bus.hxx"

#ifndef TEST_FRIENDS
#define TEST_FRIENDS
#endif
class CPU
{
  public:
    struct Instruction
    {
        std::string_view format;
        void (CPU::*op)();

        constexpr Instruction();
        constexpr Instruction(std::string_view format, void (CPU::*op)());

        constexpr static Instruction LD_R8_R8();
        constexpr static Instruction LD_R16_IMM16();
        constexpr static Instruction LD_R8_IMM8();
        constexpr static Instruction LD_R8_MEM_HL();
        constexpr static Instruction LD_MEM_HL_R8();
        constexpr static Instruction LD_MEM_HL_IMM8();

        constexpr static Instruction AND_R8();
        constexpr static Instruction XOR_R8();
        constexpr static Instruction OR_R8();
        constexpr static Instruction ILL();
        constexpr static Instruction PREFIX();

        /* Arithmetic */

        constexpr static Instruction ADC_A_R8();
        constexpr static Instruction ADC_A_MEM_HL();
        constexpr static Instruction ADD_A_R8();
        constexpr static Instruction ADD_A_IMM8();
        constexpr static Instruction ADD_A_MEM_HL();
        constexpr static Instruction ADD_HL_R16();

        constexpr static Instruction INC_R8();
        constexpr static Instruction DEC_R8();
        constexpr static Instruction INC_R16();
        constexpr static Instruction DEC_R16();
        constexpr static Instruction INC_MEM_HL();
        constexpr static Instruction DEC_MEM_HL();

        /* Control flow */

        constexpr static Instruction JP_IMM16();
        constexpr static Instruction JP_HL();
        constexpr static Instruction JP_CC_IMM16();

        constexpr static Instruction CALL_IMM16();
        constexpr static Instruction CALL_CC_IMM16();

        constexpr static Instruction RET();
        constexpr static Instruction RET_CC();

        constexpr static Instruction JR_IMM8();
        constexpr static Instruction JR_CC_IMM8();

        /* Stack manipulation */

        constexpr static Instruction PUSH_R16();
        constexpr static Instruction POP_R16();

        /* Prefixed instructions */

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

        bool operator==(const Instruction& prefix) const = default;
    };

    using InstructionLookupTable = std::array<Instruction, 0x100>;

    /**
     * @brief Instruction lookup table.
     */
    const static InstructionLookupTable inst_lookup;
    const static InstructionLookupTable cb_prefixed_inst_lookup;

    union Register
    {
        struct U16
        {
            uint16_t AF, BC, DE, HL, SP, PC;
        } u16;
        struct U8
        {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
            uint8_t F, A, C, B, E, D, L, H;
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
            uint8_t A, F, B, C, D, E, H, L;
#endif
        } u8;
    } __attribute__((packed));

    using Register8  = uint8_t   Register::U8::*;
    using Register16 = uint16_t Register::U16::*;

    class BadRegister final : public std::exception
    {
        [[nodiscard]] const char* what() const noexcept override;
    };

    struct IllegalInstruction final : std::runtime_error
    {
        explicit IllegalInstruction(uint8_t opcode)
            : std::runtime_error(std::format("Illegal instruction ({:#x})", opcode))
        {
        }
    };

    class InstructionNotImplemented final : public std::exception
    {
    };

    explicit CPU(Bus& bus);
    ~CPU();

    void fetch_data();
    void write_data();
    void tick();

    [[nodiscard]] Register get_register() const noexcept;

  private:
    enum class Register16Placeholder : uint8_t
    {
        BC = 0b00,
        DE = 0b01,
        HL = 0b10,
        SP = 0b11,
    };

    enum class Register16StackPlaceholder : uint8_t
    {
        BC = 0b00,
        DE = 0b01,
        HL = 0b10,
        AF = 0b11,
    };

    enum class Register16MemoryPlaceholder : uint8_t
    {
        BC       = 0b00,
        DE       = 0b01,
        HL_PLUS  = 0b10,
        HL_MINUS = 0b11,
    };

    enum class Register8Placeholder : uint8_t
    {
        A = 0b111,
        B = 0b000,
        C = 0b001,
        D = 0b010,
        E = 0b011,
        H = 0b100,
        L = 0b101
    };

    enum class ConditionalPlaceholder : uint8_t
    {
        NZ = 0b00,
        Z  = 0b01,
        NC = 0b10,
        C  = 0b11,
    };

    enum class BitIndexPlaceholder : uint8_t
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

    enum class StepType : bool
    {
        INCREMENT,
        DECREMENT,
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

    enum class Register8Position : uint8_t
    {
        LEFTMOST  = 3U,
        RIGHTMOST = 0U,
    };

    enum class CPUState : bool
    {
        FETCH_DECODE,
        EXECUTE
    };

    [[nodiscard]] uint8_t    get_b3() const;
    [[nodiscard]] Register8  get_register8(Register8Position shift) const;
    [[nodiscard]] Register16 get_register16() const;
    [[nodiscard]] Register16 get_register16_memory() const;
    [[nodiscard]] Register16 get_register16_stack() const;
    [[nodiscard]] bool       check_condition_is_met() const;

    void               set_carry(bool carry);
    [[nodiscard]] bool carry() const;

    void               set_half_carry(bool half_carry);
    [[nodiscard]] bool half_carry() const;

    void               set_subtract(bool subtract);
    [[nodiscard]] bool subtract() const;

    void               set_zero(bool zero);
    [[nodiscard]] bool zero() const;

    /**
     * Push a 16 bit value into the stack.
     *
     * @param msb Most Significant Byte
     * @param lsb Least Significant Byte
     */
    void push_16(uint8_t msb, uint8_t lsb);

    /**
     * Pop a 16 bit value off the stack.
     * @return uint16_t value
     */
    uint16_t pop_16();

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

    /**
     * @brief Load from 16-bit address to 8-bit register A.
     */
    void LD_A_MEM_16();

    /**
     * @brief Load value in register A from the byte pointed to by register r16.
     */
    void LD_A_MEM_R16();

    /**
     * @brief Load from 8-bit immediate to memory pointed by HL.
     */
    void LD_MEM_HL_IMM8();

    /**
     * @brief Push register r16 into the stack.
     */
    void PUSH_R16();

    /**
     * @brief Pop register r16 onto the stack.
     */
    void POP_R16();

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

    uint16_t ADD_16(uint16_t a, uint16_t b);
    uint8_t  ADD_8(uint8_t a, uint8_t b, bool add_carry);
    /**
     * @brief Add the value in r8 to A.
     */
    void ADD_A_R8();

    /**
     * @brief Add the byte pointed to by HL to A.
     */
    void ADD_A_MEM_HL();

    /**
     * @brief Add the value imm8 to A.
     */
    void ADD_A_IMM8();

    /**
     * @brief Add the value in r16 to HL.
     */
    void ADD_HL_R16();

    /**
     * @brief Add the value in r8 plus the carry flag to A.
     */
    void ADC_A_R8();

    /**
     * @brief Add the byte pointed to by HL plus the carry flag to A.
     */
    void ADC_A_MEM_HL();

    uint8_t STEP_IMM8(uint8_t value, StepType type);
    /**
     * @brief Increment value in register r8 by 1.
     */
    void INC_R8();
    /**
     * @brief Decrement value in register r8 by 1.
     */
    void DEC_R8();
    /**
     * @brief Increment value in register r16 by 1.
     */
    void INC_R16();
    /**
     * @brief Decrement value in register r16 by 1.
     */
    void DEC_R16();

    /**
     * @brief Increment value pointer by register HL by 1.
     */
    void INC_MEM_HL();

    /**
     * @brief Decrement value pointer by register HL by 1.
     */
    void DEC_MEM_HL();

    /**
     * @brief CB Prefix.
     */
    void PREFIX();

    /**
     * @brief Jump to address n16; effectively, store n16 into PC.
     */
    void JP_IMM16();

    /**
     * @brief Jump to address in HL; effectively, load PC with value in register HL.
     */
    void JP_HL();

    /**
     * @brief Jump to address n16 if condition cc is met.
     */
    void JP_CC_IMM16();

    /**
     * @brief Call address n16. This pushes the address of the instruction after the CALL on the stack, such that RET
     * can pop it later; then, it executes an implicit JP n16.
     */
    void CALL_IMM16();

    /*
     * @brief Call address n16 if condition cc is met.
     */
    void CALL_CC_IMM16();

    /**
     * @brief Relative Jump to address n16. The address is encoded as a signed 8-bit offset from the address immediately
     * following the JR instruction, so the target address n16 must be between -128 and 127 bytes away.
     */
    void JR_IMM8();

    /**
     * @brief Relative Jump to address n16 if condition cc is met.
     */
    void JR_CC_IMM8();

    /**
     * @brief Return from subroutine. This is basically a POP PC (if such an instruction existed).
     */
    void RET();

    /**
     * @brief Return from subroutine if condition cc is met.
     */
    void RET_CC();

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

    void BIT(uint8_t val, uint8_t bit);
    /**
     * @brief Test bit in an 8-bit register, set the zero flag if bit not set.
     */
    void BIT_R8();
    /**
     * @brief Test bit in the byte pointed by HL, set the zero flag if bit not set.
     */
    void BIT_MEM_HL();

    auto SWAP(uint8_t val);
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
     * @brief Registers
     */
    Register reg;

    struct
    {
        union
        {
            uint16_t u16;
        };
        union
        {
            uint8_t u8_lsb, u8_msb;
        };
    } fetched_data;

    /**
     * @brief Micro operation queue. A micro operation is an operation whose T-cycle equals to 4, or 1 M-cycle.
     */
    std::queue<std::function<void()>> micro_ops{};

    /**
     * @brief State of the CPU.
     */
    CPUState state{CPUState::FETCH_DECODE};

    uint8_t ticks;
    /**
     * @brief Represents the current operation code being executed.
     */
    uint8_t opcode;
    /**
     * @brief If current instruction is cb_prefixed.
     */
    bool cb_prefixed;
    /**
     * @brief Current instruction.
     */
    Instruction inst;
    /**
     * @brief Interupt Master Enable flag.
     */
    bool ime;

    Bus& bus;

    TEST_FRIENDS;
};

#endif