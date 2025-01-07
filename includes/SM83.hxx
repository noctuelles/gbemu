#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include <map>
#include <optional>
#include <string>

#include "Bus.hxx"

class SM83
{
  public:
    using DisassembledInstruction = std::map<uint16_t, std::string>;

    enum class Flags : uint8_t
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

    enum class ResetVector : uint16_t
    {
        h00 = 0x00,
        h08 = 0x08,
        h10 = 0x10,
        h18 = 0x18,
        h20 = 0x20,
        h28 = 0x28,
        h30 = 0x30,
        h38 = 0x38,
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
    void print_state();

    [[nodiscard]] auto disassemble(uint16_t start, std::optional<uint16_t> stop = std::nullopt) const
        -> DisassembledInstruction;

  private:
    void machine_cycle();

    void fetch_decode_execute(bool extended_set = false);

    void    fetch_operand();
    uint8_t fetch_memory(uint16_t address);
    void    write_memory(uint16_t address, uint8_t value);

    [[nodiscard]] uint16_t AF() const;
    [[nodiscard]] uint16_t BC() const;
    [[nodiscard]] uint16_t DE() const;
    [[nodiscard]] uint16_t HL() const;

    void AF(uint16_t value);
    void BC(uint16_t value);
    void DE(uint16_t value);
    void HL(uint16_t value);

    /* ALU */
    [[nodiscard]] uint8_t  add(uint8_t lhs, uint8_t rhs, bool carry = false);
    [[nodiscard]] uint16_t add(uint16_t lhs, uint16_t rhs);
    [[nodiscard]] uint16_t add(uint16_t lhs, uint8_t rhs);

    void                  daa();
    [[nodiscard]] uint8_t sub(uint8_t lhs, uint8_t rhs, bool borrow = false);
    [[nodiscard]] uint8_t bitwise_and(uint8_t lhs, uint8_t rhs);
    [[nodiscard]] uint8_t bitwise_or(uint8_t lhs, uint8_t rhs);
    [[nodiscard]] uint8_t bitwise_xor(uint8_t lhs, uint8_t rhs);
    [[nodiscard]] uint8_t rotate_left(uint8_t op, bool circular = false);
    [[nodiscard]] uint8_t rotate_right(uint8_t op, bool circular = false);
    [[nodiscard]] uint8_t shift_right(uint8_t op, bool arithmetic = false);
    [[nodiscard]] uint8_t shift_left(uint8_t op);
    void                  bit(uint8_t op, std::size_t bit);
    static uint8_t        res(uint8_t op, std::size_t bit);
    static uint8_t        set(uint8_t op, std::size_t bit);
    uint8_t               swap(uint8_t op);

    /* IDU */
    [[nodiscard]] static uint16_t inc(uint16_t value);
    [[nodiscard]] static uint16_t dec(uint16_t value);
    [[nodiscard]] uint8_t         inc(uint8_t value);
    [[nodiscard]] uint8_t         dec(uint8_t value);

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

    void push(uint8_t msb, uint8_t lsb);
    void push(uint16_t value);

    void pop(uint8_t& msb, uint8_t& lsb);
    void pop(uint16_t& value);

    /* Misc */

    void               set_flag(Flags flag, bool value);
    [[nodiscard]] bool get_flag(Flags flag) const;
    [[nodiscard]] bool is_condition_met(Conditionals conditional) const;

    /**
     * @brief Registers
     */
    uint8_t  A{};
    uint8_t  F{};
    uint8_t  B{};
    uint8_t  C{};
    uint8_t  D{};
    uint8_t  E{};
    uint8_t  H{};
    uint8_t  L{};
    uint16_t SP{};
    uint16_t PC{};

    /**
     * @brief State of the CPU.
     */
    State state{State::NORMAL};

    /**
     * @brief Represents the current operation code being executed.
     */
    uint8_t ir{};

    /**
     * @brief Interupt Master Enable flag.
     */
    bool ime{};

    uint8_t request_ime{};

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