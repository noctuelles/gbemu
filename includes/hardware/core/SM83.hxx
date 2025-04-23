#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include <functional>
#include <map>
#include <optional>
#include <span>
#include <string>
#include <vector>

#include "hardware/Addressable.hxx"

namespace Test
{
    class SM83;
}

class SM83 final : public Component
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
        /**
         * @brief The halt bug is a hardware bug in the SM83 CPU that happens when halt is executed with IME==0 and IE &
         * IF
         * != 0. The bug is usually explained as duplicating the next
         * byte in the instruction pipeline.
         */
        HALTED_BUG,
        STOPPED,
    };

    class Disassembler
    {
      public:
        using InstructionDump          = std::pair<uint16_t, std::vector<uint8_t>>;
        using DisassembledInstructions = std::map<InstructionDump, std::string>;

        explicit Disassembler(std::span<uint8_t> memory);

        [[nodiscard]] auto disassemble(uint16_t start, std::optional<uint16_t> stop = {},
                                       std::optional<uint16_t> base_addr = {}) const -> DisassembledInstructions;

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

    void                           write(uint16_t address, uint8_t value) override;
    [[nodiscard]] uint8_t          read(uint16_t address) override;
    [[nodiscard]] AddressableRange get_addressable_range() const noexcept override;

    void tick() override;

    void print_state();

  private:
    /**
     * @brief Load an instruction into the IR register.
     */
    void fetch_instruction();

    /**
     * @brief Execute the instruction stored in the IR register.
     */
    void decode_execute_instruction(bool extended_set = false);

    [[nodiscard]] uint8_t fetch_memory(uint16_t address) const;
    [[nodiscard]] uint8_t fetch_operand();
    void                  write_memory(uint16_t address, uint8_t value) const;

    /**
     * @brief AF register getter.
     */
    [[nodiscard]] uint16_t AF() const;
    /**
     * @brief BC register getter.
     */
    [[nodiscard]] uint16_t BC() const;
    /**
     * @brief DE register getter.
     */
    [[nodiscard]] uint16_t DE() const;
    /**
     * @brief HL register getter.
     */
    [[nodiscard]] uint16_t HL() const;

    /**
     * @brief Sets the contents of the A and F registers using the provided 16-bit value.
     * @param value The 16-bit value to be split and assigned to the A and F registers.
     */
    void AF(uint16_t value);
    /**
     * @brief Sets the BC register pair by splitting a 16-bit value into two 8-bit values.
     * @param value The 16-bit value to be split and stored in the B and C registers.
     */
    void BC(uint16_t value);
    /**
     * @brief Sets the DE register using the specified 16-bit value.
     * @param value The 16-bit value to be split and stored into the D and E registers.
     */
    void DE(uint16_t value);
    /**
     * @brief Sets the HL register pair using the provided 16-bit value.
     * @param value A 16-bit value split into two 8-bit values for the H and L registers.
     */
    void HL(uint16_t value);

    /**
     * @brief Performs an 8-bit addition with optional carry flag support.
     *
     * @param lhs The left-hand operand for the addition.
     * @param rhs The right-hand operand for the addition.
     * @param carry Specifies whether to include the carry flag in the operation.
     * @return The 8-bit result of the addition.
     *
     * This function evaluates the sum of two 8-bit operands and optionally includes
     * a carry flag. It also updates relevant processor flags, such as Zero, Subtract,
     * Half-Carry, and Carry flags, based on the result of the operation.
     */
    [[nodiscard]] uint8_t add(uint8_t lhs, uint8_t rhs, bool carry = false);

    /**
     * @brief Perform 16-bit addition and update relevant CPU flags.
     *
     * @param lhs The left-hand operand for the addition.
     * @param rhs The right-hand operand for the addition.
     * @return The 16-bit result of the addition.
     */
    [[nodiscard]] uint16_t add(uint16_t lhs, uint16_t rhs);

    /**
     * @brief Performs addition on a 16-bit and an 8-bit value, incorporating CPU flags for adjustments.
     *
     * @param lhs The 16-bit unsigned integer operand (left-hand side).
     * @param rhs The 8-bit integer operand (right-hand side).
     * @return The resulting 16-bit unsigned integer after performing the addition operation.
     */
    [[nodiscard]] uint16_t add(uint16_t lhs, uint8_t rhs);

    /**
     * @brief Adjusts the accumulator register A for binary-coded decimal (BCD) arithmetic.
     *
     * This method modifies the accumulator (A) and flags based on the current values,
     * ensuring the result complies with BCD representation. It applies adjustments
     * for subtraction, half-carry, and carry conditions and updates the Zero, Carry,
     * and HalfCarry flags accordingly.
     */
    void daa();

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
     * @brief Interrupt Enable.
     */
    uint8_t IE{};
    /**
     * @brief Interrupt Flags.
     */
    uint8_t IF{};
    /**
     * @brief Instruction Register.
     */
    uint8_t IR{};

    std::vector<uint8_t> instruction_buffer{};

    /**
     * @brief Callback function when a machine cycle is executed by the CPU.
     */
    std::function<void()> machine_cycle;

    /**
     * @brief State of the CPU.
     */
    State state{State::NORMAL};

    /**
     * @brief Represents the current operation code being executed.
     */

    /**
     * @brief Interrupt Master Enable flag.
     */
    bool IME{};

    uint8_t request_ime{};

    Addressable& bus;

    friend class MooneyeAcceptance;
    friend class Test::SM83;
};

#endif