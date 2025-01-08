#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include <functional>
#include <map>
#include <optional>
#include <span>
#include <string>
#include <vector>

#include "Bus.hxx"

class SM83 : public Component
{
  public:
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

    enum class Interrupts : uint8_t
    {
        VBlank = 0x00,
        LCD    = 0x01,
        Timer  = 0x04,
        Serial = 0x08,
        Joypad = 0x10,
    };

    enum class State
    {
        NORMAL,
        HALTED,
        STOPPED,
    };

    class Disassembler
    {
      public:
        using InstructionDump          = std::pair<uint16_t, std::vector<uint8_t>>;
        using DisassembledInstructions = std::map<InstructionDump, std::string>;

        explicit Disassembler(std::span<uint8_t> memory);

        [[nodiscard]] auto disassemble(uint16_t start, std::optional<uint16_t> stop = std::nullopt) const
            -> DisassembledInstructions;

      private:
        const std::span<uint8_t> memory;

        enum class AddressingMode
        {
            IMMEDIATE,
            IMMEDIATE_SIGNED,
            IMMEDIATE_EXTENDED,
            RELATIVE,
        };

        struct Instruction
        {
            std::string_view              name{"ILL"};
            std::optional<AddressingMode> addressing{std::nullopt};
        };

        using InstructionLookup = std::array<Instruction, 0x100>;

        static const InstructionLookup instruction_lookup;
        static const InstructionLookup prefixed_instruction_lookup;
    };

    SM83(Addressable& bus, const std::function<void()>& on_machine_cycle);

    void                  write(uint16_t address, uint8_t value) override;
    [[nodiscard]] uint8_t read(uint16_t address) override;
    void                  tick() override;

    void print_state();

  private:
    void    fetch_decode_execute(bool extended_set = false);
    void    fetch_operand();
    uint8_t fetch_memory(uint16_t address) const;
    void    write_memory(uint16_t address, uint8_t value) const;

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

    void                  set_flag(Flags flag, bool value);
    [[nodiscard]] bool    get_flag(Flags flag) const;
    [[nodiscard]] bool    is_condition_met(Conditionals conditional) const;
    [[nodiscard]] uint8_t get_interrupt_request() const;
    void                  interrupts();

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
    uint8_t  IE{};
    uint8_t  IF{};

    std::function<void()> machine_cycle;

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

    Addressable& bus;
};

#endif