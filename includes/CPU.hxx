#ifndef CPU_H
#define CPU_H

#include <array>
#include <functional>
#include <map>
#include <optional>
#include <queue>
#include <stdexcept>

#include "Bus.hxx"

#ifndef TEST_FRIENDS
#define TEST_FRIENDS
#endif

class CPU
{
  public:
    using DisassembledInstruction = std::map<uint16_t, std::string>;

    enum class AddressingMode
    {
        IMMEDIATE,
        IMMEDIATE_EXTENDED,
        RELATIVE,
    };

    struct Instruction
    {
        std::string_view              name{"ILL"};
        std::function<void(CPU*)>     executor{&CPU::ILL};
        std::optional<AddressingMode> addressing_mode{std::nullopt};
    };

    using InstructionLookupTable = std::array<Instruction, 0x100>;

    /**
     * @brief Instruction lookup table.
     */
    const static InstructionLookupTable instruction_lookup;
    const static InstructionLookupTable prefixed_instruction_lookup;

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
        explicit IllegalInstruction(uint8_t opcode, uint16_t pc)
            : std::runtime_error("Illegal instruction " + std::to_string(opcode) + " " + std::to_string(pc))
        {
        }
    };

    class InstructionNotImplemented final : public std::exception
    {
    };

    explicit CPU(Bus& bus);

    void fetch_data();
    void write_data();
    void tick();

    [[nodiscard]] auto disassemble(uint16_t start, std::optional<uint16_t> stop = std::nullopt) const -> DisassembledInstruction;
    [[nodiscard]] Register get_register() const noexcept;

    static std::string_view get_register8_name(Register8 reg);

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
            ZERO       = 1U << 7,
            SUBTRACT   = 1U << 6,
            HALF_CARRY = 1U << 5,
            CARRY      = 1U << 4
        };
    };

    enum class Register8Position : uint8_t
    {
        LEFTMOST  = 3U,
        RIGHTMOST = 0U,
    };

    enum class BitwiseOperator : uint8_t
    {
        AND,
        OR,
        XOR
    };

    enum class CPUState : bool
    {
        FETCH_DECODE,
        EXECUTE
    };

    [[nodiscard]] uint8_t                                            get_b3() const;
    [[nodiscard]] uint8_t                                            get_tgt3() const;
    [[nodiscard]] Register8                                          get_register8(Register8Position shift) const;
    [[nodiscard]] Register16                                         get_register16() const;
    [[nodiscard]] std::pair<Register16MemoryPlaceholder, Register16> get_register16_memory() const;
    [[nodiscard]] Register16                                         get_register16_stack() const;
    [[nodiscard]] bool                                               check_condition_is_met() const;

    void               set_carry(bool carry);
    [[nodiscard]] bool carry() const;

    void               set_half_carry(bool half_carry);
    [[nodiscard]] bool half_carry() const;

    void               set_subtract(bool subtract);
    [[nodiscard]] bool subtract() const;

    void               set_zero(bool zero);
    [[nodiscard]] bool zero() const;

    /**
     * @brief No OPeration.
     */
    void NOP();

    /**
     * @brief Enter CPU very low power mode. Also used to switch between GBC double speed and normal speed CPU modes.
     */
    void STOP();

    /**
     * @brief Enter CPU low-power consumption mode until an interrupt occurs.
     */
    void HALT();

    /**
     * @brief ComPLement accumulator (A = ~A); also called bitwise NOT.
     */
    void CPL();

    /**
     * @brief Complement Carry Flag.
     */
    void CCF();

    /**
    * @brief Decimal Adjust Accumulator. Designed to be used after performing an arithmetic instruction (ADD, ADC, SUB,
    SBC) whose inputs were in Binary-Coded Decimal (BCD), adjusting the result to likewise be in BCD.
     */
    void DAA();

    /**
     * @brief Set Carry Flag.
     */
    void SCF();

    /**
     * @brief Illegal instruction.
     */
    void ILL();

    /**
     * @brief Disables interrupt handling by setting IME=0 and cancelling any scheduled effects of the EI instruction if
     * any.
     */
    void DI();

    /**
     * @brief Schedules interrupt handling to be enabled after the next machine cycle.
     */
    void EI();

    /**
     * @brief Load from 8-bit register to 8-bit register.
     */
    void LD_R8_R8();

    void LD();

    /**
     * @brief Load from 8-bit immediate to 8-bit register.
     */
    void LD_R8_IMM8();

    /**
     * @brief Load from 16-bit immediate to 16-bit register.
     */
    void LD_R16_IMM16();

    /**
     * @brief Copy register HL into register SP.
     */
    void LD_SP_HL();

    /**
     * @brief Store value in register A into the byte pointed to by register r16.
     */
    void LD_MEM_R16_A();

    /**
     * @brief Load value in register A from the byte pointed to by register r16.
     */
    void LD_A_MEM_R16();

    /**
     * @brief Load from memory pointed by HL to 8-bit register.
     */
    void LD_R8_MEM_HL();

    /**
     * @brief Load from 8-bit register to memory pointed by HL.
     */
    void LD_MEM_HL_R8();

    /**
     * @brief Load from 8-bit immediate to memory pointed by HL.
     */
    void LD_MEM_HL_IMM8();

    /**
     * @brief Store value in register A into the byte at address n16, provided the address is between $FF00 and $FFFF.
     */
    void LDH_MEM_16_A();

    /**
     * @brief Load value in register A from the byte at address n16, provided the address is between $FF00 and $FFFF.
     */
    void LDH_A_MEM_16();

    /**
     * @brief Store value in register A into the byte at address $FF00+C.
     */
    void LDH_MEM_C_A();

    /**
     * @brief Load value in register A from the byte at address $FF00+c.
     */
    void LDH_A_MEM_C();

    /**
     * @brief Copy SP & $FF at address n16 and SP >> 8 at address n16 + 1.
     */
    void LD_MEM_16_SP();

    /**
     * @brief Copy the value in register A into the byte at address n16.
     */
    void LD_MEM_16_A();

    /**
     * @brief Copy the byte at address n16 into register A.
     */
    void LD_A_MEM_16();

    /**
     * @brief Add the signed value e8 to SP and copy the result in HL.
     */
    void LD_HL_SP_PLUS_IMM8();

    /**
     * @brief Push register r16 into the stack.
     */
    void PUSH_R16();

    /**
     * @brief Pop register r16 onto the stack.
     */
    void POP_R16();

    void BITWISE(BitwiseOperator op, uint8_t operand);
    /**
     * @brief Set A to the bitwise AND between the value in r8 and A.
     */
    void AND_R8();

    /**
     * @brief Set A to the bitwise AND between the byte pointed to by HL and A.
     */
    void AND_MEM_HL();

    /**
     * @brief Set A to the bitwise AND between the value n8 and A.
     */
    void AND_IMM8();

    /**
     * @brief Set A to the bitwise OR between the value in r8 and A.
     */
    void OR_R8();

    /**
     * @brief Set A to the bitwise OR between the byte pointed to by HL and A.
     */
    void OR_MEM_HL();

    /**
     * @brief Set A to the bitwise OR between the value n8 and A.
     */
    void OR_IMM8();

    /**
     * @brief Set A to the bitwise XOR between the value in r8 and A.
     */
    void XOR_R8();

    /**
     * @brief Set A to the bitwise XOR between the byte pointed to by HL and A.
     */
    void XOR_MEM_HL();

    /**
     * @brief Set A to the bitwise XOR between the value n8 and A.
     */
    void XOR_IMM8();

    uint16_t ADD_16(uint16_t a, uint16_t b);
    uint16_t ADD_16(uint16_t a, int8_t b);
    uint8_t  ADD_8(uint8_t a, uint8_t b, bool add_carry);
    /**
     * @brief Add the value in r8 to A.
     */
    void ADD_R8();

    /**
     * @brief Add the byte pointed to by HL to A.
     */
    void ADD_MEM_HL();

    /**
     * @brief Add the value imm8 to A.
     */
    void ADD_IMM8();

    /**
     * @brief Add the value in r16 to HL.
     */
    void ADD_HL_R16();

    /**
     * @brief Add the signed value e8 to SP.
     */
    void ADD_SP_IMM8();

    /**
     * @brief Add the value in r8 plus the carry flag to A.
     */
    void ADC_R8();

    /**
     * @brief Add the byte pointed to by HL plus the carry flag to A.
     */
    void ADC_MEM_HL();

    /**
     * @brief Add the value n8 plus the carry flag to A.
     */
    void ADC_IMM8();

    uint8_t SUB_8(uint8_t a, uint8_t b, bool sub_carry);
    /**
     * @brief Subtract the value in r8 from A.
     */
    void SUB_R8();

    /**
     * @brief Subtract the byte pointed to by HL from A.
     */
    void SUB_MEM_HL();

    /**
     * @brief Subtract the value n8 from A.
     */
    void SUB_IMM8();

    /**
     * @brief Subtract the value in r8 and the carry flag from A.
     */
    void SBC_R8();

    /**
     * @brief Subtract the byte pointed to by HL and the carry flag from A.
     */
    void SBC_MEM_HL();

    /**
     * @brief Subtract the value n8 and the carry flag from A.
     */
    void SBC_IMM8();

    /**
     * @brief ComPare the value in A with the value in r8. This subtracts the value in r8 from A and sets flags
     * accordingly, but discards the result.
     */
    void CP_R8();

    /**
     * @brief ComPare the value in A with the byte pointed to by HL. This subtracts the byte pointed to by HL from A and
     * sets flags accordingly, but discards the result.
     */
    void CP_MEM_HL();

    /**
     * @brief ComPare the value in A with the value n8. This subtracts the value n8 from A and sets flags accordingly,
     * but discards the result.
     */
    void CP_IMM8();

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
     * @brief Call address vec. This is a shorter and faster equivalent to CALL for suitable values of vec.
     */
    void RST_VEC();

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
     * @brief Rotate register A left, through the carry flag.
     */
    void RLA();

    /**
     * @brief Rotate register A left.
     */
    void RLCA();

    /**
     * @brief Rotate register A right.
     */
    void RRCA();

    /**
     * @brief Rotate register A right, through the carry flag.
     */
    void RRA();

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
    auto SHIFT(uint8_t val, ShiftDirection shift_direction, ShiftType shift_type) noexcept;
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
    Register reg{};

    /**
     * @brief Temporary register.
     */
    union
    {
        uint16_t WZ;
        struct
        {
            uint8_t Z;
            uint8_t W;
        };
    } __attribute__((packed)) tmp{};

    /**
     * @brief Micro operation queue. A micro operation is an operation whose T-cycle equals to 4, or 1 M-cycle.
     */
    std::queue<std::function<void()>> micro_ops{};

    /**
     * @brief State of the CPU.
     */
    CPUState state{CPUState::FETCH_DECODE};

    /**
     * @brief Keep track of the ticks in the current M-cycle.
     */
    uint8_t ticks{};

    /**
     * @brief Represents the current operation code being executed.
     */
    uint8_t opcode{};

    /**
     * @brief If current instruction is cb_prefixed.
     */
    bool prefixed{false};

    /**
     * @brief Current instruction.
     */
    Instruction instruction{instruction_lookup[0]};

    std::pair<uint16_t, std::string> disassembled_instruction{};

    /**
     * @brief Interupt Master Enable flag.
     */
    bool ime{};

    Bus& bus;

    TEST_FRIENDS;
};

#endif