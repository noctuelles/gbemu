#ifndef CPU_H
#define CPU_H

#include <array>
#include <functional>
#include <map>
#include <optional>
#include <stdexcept>

#include "Bus.hxx"

class SM83
{
  public:
    using byte         = uint8_t;
    using signed_byte  = int8_t;
    using word         = uint16_t;
    using signed_dword = int32_t;

    using DisassembledInstruction = std::map<uint16_t, std::string>;

    enum class Flags : byte
    {
        Carry     = 0x10,
        HalfCarry = 0x20,
        Subtract  = 0x40,
        Zero      = 0x80,
    };

    enum class Conditionals
    {
        NZ,
        Z,
        NC,
        C,
    };

    enum class ResetVector : word
    {
        First   = 0x08,
        Second  = 0x10,
        Third   = 0x18,
        Fourth  = 0x20,
        Fifth   = 0x28,
        Sixth   = 0x30,
        Seventh = 0x38,
    };

    enum class AddressingMode
    {
        IMMEDIATE,
        IMMEDIATE_EXTENDED,
        RELATIVE,
    };

    enum class State
    {
        NORMAL,
        HALTED,
        STOPPED,
    };

    explicit SM83(Bus& bus);

    void tick();

    [[nodiscard]] auto disassemble(uint16_t start, std::optional<uint16_t> stop = std::nullopt) const
        -> DisassembledInstruction;

  private:
    void machine_cycle();

    void fetch_decode_execute(bool extended_set = false);

    void fetch_operand();
    byte fetch_memory(word address);
    void write_memory(word address, byte value);

    [[nodiscard]] word AF() const;
    [[nodiscard]] word BC() const;
    [[nodiscard]] word DE() const;
    [[nodiscard]] word HL() const;

    void AF(word value);
    void BC(word value);
    void DE(word value);
    void HL(word value);

    /* ALU */
    [[nodiscard]] byte add(byte lhs, byte rhs, bool carry = false);
    [[nodiscard]] word add(word lhs, word rhs);
    [[nodiscard]] word add(word lhs, byte rhs);

    [[nodiscard]] byte sub(byte lhs, byte rhs, bool borrow = false);
    [[nodiscard]] byte bitwise_and(byte lhs, byte rhs);
    [[nodiscard]] byte bitwise_or(byte lhs, byte rhs);
    [[nodiscard]] byte bitwise_xor(byte lhs, byte rhs);

    byte rotate_left(byte op, bool circular = false);
    byte rotate_right(byte op, bool circular = false);

    void        bit(byte op, std::size_t bit);
    static byte res(byte op, std::size_t bit);
    static byte set(byte op, std::size_t bit);
    byte        swap(byte op);

    /* IDU */
    [[nodiscard]] static word inc(word value);
    [[nodiscard]] static word dec(word value);
    [[nodiscard]] byte        inc(byte value);
    [[nodiscard]] byte        dec(byte value);

    /* Control */

    void jr();
    void jp();

    void call();
    void call_cc(Conditionals conditional);

    void ret();
    void ret_cc(Conditionals conditional);

    void jp_cc(Conditionals conditional);
    void jr_cc(Conditionals conditional);

    void rst(ResetVector rst_vector);

    /* Stack */

    void push(byte msb, byte lsb);
    void push(word value);

    void pop(byte& msb, byte& lsb);
    void pop(word& value);

    /* Misc */

    void               set_flag(Flags flag, bool value);
    [[nodiscard]] bool get_flag(Flags flag) const;
    [[nodiscard]] bool is_condition_met(Conditionals conditional) const;

    /**
     * @brief Registers
     */
    byte A{};
    byte F{};
    byte B{};
    byte C{};
    byte D{};
    byte E{};
    byte H{};
    byte L{};
    word SP{};
    word PC{};

    /**
     * @brief State of the CPU.
     */
    State state{State::NORMAL};

    /**
     * @brief Represents the current operation code being executed.
     */
    byte ir{};

    /**
     * @brief Interupt Master Enable flag.
     */
    bool ime{};

    byte request_ime{};

    Bus& bus;

    friend class CPUTesting;

    friend class LD_R8_R8_Value_Test;
    friend class LD_R8_R8_Flags_Test;
    friend class LD_MEM_HL_R8_Value_Test;
    friend class LD_R8_MEM_HL_Value_Test;
    friend class LD_R8_IMM8_Value_Test;
    friend class LD_MEM_HL_IMM8_Value_Test;
    friend class LD_MEM_16_SP_Value_Test;
    friend class LD_MEM_16_A_Value_Test;
    friend class LD_A_MEM_16_Value_Test;
    friend class LD_R16_IMM16_Value_Test;
    friend class LDH_IMM8_A_Value_Test;
    friend class LDH_A_IMM8_Value_Test;
    friend class LDH_MEM_C_A_Value_Test;
    friend class LDH_A_MEM_C_Value_Test;

    friend class RR_R8_Value_Test;
    friend class RRC_R8_Value_Test;
    friend class RL_R8_Value_Test;
    friend class RLC_R8_Value_Test;
};

#endif