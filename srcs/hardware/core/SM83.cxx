#include "hardware/core/SM83.hxx"

/**
 * @note https://gbdev.io/gb-opcodes/optables/
 * https://rgbds.gbdev.io/docs/v0.8.0/gbz80.7
 * https://dn790000.ca.archive.org/0/items/GameBoyProgManVer1.1/GameBoyProgManVer1.1.pdf
 */

#include <Utils.hxx>
#include <bitset>
#include <cassert>
#include <format>
#include <iostream>
#include <utility>

SM83::SM83(Addressable& bus, const std::function<void()>& on_machine_cycle)
    : onMachineCycleCb(on_machine_cycle), bus(bus)
{
    A  = 0x01;
    F  = 0xB0;
    B  = 0x00;
    C  = 0x13;
    D  = 0x00;
    E  = 0xD8;
    H  = 0x01;
    L  = 0x4D;
    SP = 0xFFFE;
    PC = 0x0100;

    instruction_buffer.reserve(16);
}

void SM83::write(const uint16_t address, const uint8_t value)
{
    switch (address)
    {
        case MemoryMap::IE:
            IE = value;
            break;
        case MemoryMap::IORegisters::IF:
            IF = 0xE0 | value;
            break;
        default:
            throw std::logic_error("Invalid SM83 Write");
    }
}

uint8_t SM83::read(const uint16_t address) const
{
    switch (address)
    {
        case MemoryMap::IE:
            return IE;
        case MemoryMap::IORegisters::IF:
            return 0xE0 | IF;
        default:
            throw std::logic_error("Invalid SM83 Write");
    }
}

Addressable::AddressableRange SM83::get_addressable_range() const noexcept
{
    return {MemoryMap::IE, MemoryMap::IORegisters::IF};
}

void SM83::tick()
{
    switch (state)
    {
        case State::NORMAL:
        {
            if (requestIme != 0)
            {
                requestIme -= 1;
                if (requestIme == 0)
                {
                    IME = true;
                }
            }

            fetchInstruction();
            decodeExecuteInstruction();

            instruction_buffer.clear();
            break;
        }
        case State::STOPPED:
        case State::HALTED:
            onMachineCycleCb();
            if ((IE & IF) != 0)
            {
                state = State::NORMAL;
            }
            break;
        case State::HALTED_BUG:
            fetchInstruction();
            PC--;
            decodeExecuteInstruction();
            break;
    }

    interrupts();
}

void SM83::applyView(const View& view)
{
    A = view.registers.A;
    F = view.registers.F;
    B = view.registers.B;
    C = view.registers.C;
    D = view.registers.D;
    E = view.registers.E;
    H = view.registers.H;
    L = view.registers.L;

    AF(view.registers.AF);
    BC(view.registers.BC);
    DE(view.registers.DE);
    HL(view.registers.HL);

    SP = view.registers.SP;
    PC = view.registers.PC;
}

SM83::View SM83::getView() const
{
    View view{};

    view.registers.A = A;
    view.registers.F = F;
    view.registers.B = B;
    view.registers.C = C;
    view.registers.D = D;
    view.registers.E = E;
    view.registers.H = H;
    view.registers.L = L;

    view.registers.AF = AF();
    view.registers.BC = BC();
    view.registers.DE = DE();
    view.registers.HL = HL();

    view.registers.SP = SP;
    view.registers.PC = PC;

    return view;
}

void SM83::attachDebugger(IDebugger& debugger)
{
    this->debugger = &debugger;
}

void SM83::detachDebugger()
{
    debugger = nullptr;
}

void SM83::print_state()
{
    std::string formatted_state{std::format(
        "A:{:02X} F:{:02X} B:{:02X} C:{:02X} D:{:02X} E:{:02X} H:{:02X} L:{:02X} SP:{:04X} PC:{:04X} IE:{:02X} IF:{:02X} IME:{:s}",
        A, F, B, C, D, E, H, L, SP, PC, IE, IF, IME)};
    std::print(std::cout, "{:<85s} - ", formatted_state);
}

void SM83::fetchInstruction()
{
    if (debugger)
    {
        debugger->onInstructionFetched();
    }

    IR = fetch_memory(PC++);
    instruction_buffer.push_back(IR);
}

uint8_t SM83::fetch_memory(const uint16_t address) const
{
    onMachineCycleCb();
    return bus.read(address);
}

uint8_t SM83::fetch_operand()
{
    const auto byte{fetch_memory(PC++)};
    instruction_buffer.push_back(byte);
    return byte;
}

void SM83::write_memory(const uint16_t address, const uint8_t value) const
{
    onMachineCycleCb();
    return bus.write(address, value);
}

uint16_t SM83::AF() const
{
    return utils::to_word(A, F);
}

uint16_t SM83::BC() const
{
    return utils::to_word(B, C);
}

uint16_t SM83::DE() const
{
    return utils::to_word(D, E);
}

uint16_t SM83::HL() const
{
    return utils::to_word(H, L);
}

void SM83::AF(const uint16_t value)
{
    utils::to_bytes(value, A, F);
}
void SM83::BC(const uint16_t value)
{
    utils::to_bytes(value, B, C);
}

void SM83::DE(const uint16_t value)
{
    utils::to_bytes(value, D, E);
}

void SM83::HL(const uint16_t value)
{
    utils::to_bytes(value, H, L);
}

uint8_t SM83::add(const uint8_t lhs, const uint8_t rhs, const bool carry)
{
    const auto add_carry{static_cast<uint8_t>(carry ? get_flag(Flags::Carry) : 0)};
    const auto result{static_cast<uint16_t>(lhs + rhs + add_carry)};

    set_flag(Flags::Zero, (result & 0xFF) == 0);
    set_flag(Flags::Subtract, false);
    set_flag(Flags::HalfCarry, (lhs & 0x0F) + (rhs & 0x0F) + add_carry > 0x0F);
    set_flag(Flags::Carry, result > 0xFF);

    return static_cast<uint8_t>(result & 0xFF);
}

uint16_t SM83::add(const uint16_t lhs, const uint16_t rhs)
{
    const int32_t result{lhs + rhs};

    set_flag(Flags::Subtract, false);
    set_flag(Flags::HalfCarry, (lhs & 0x0FFF) + (rhs & 0x0FFF) > 0x0FFF);
    set_flag(Flags::Carry, result > 0xFFFF);

    return static_cast<uint16_t>(result);
}

uint16_t SM83::add(const uint16_t lhs, const uint8_t rhs)
{
    const auto sign{(rhs & 0x80) != 0};
    auto       lhs_lsb{utils::word_lsb(lhs)};
    auto       lhs_msb{utils::word_msb(lhs)};

    lhs_lsb = add(lhs_lsb, rhs);

    onMachineCycleCb();

    if (get_flag(Flags::Carry) && !sign)
    {
        lhs_msb += 1;
    }
    else if (!get_flag(Flags::Carry) && sign)
    {
        lhs_msb -= 1;
    }

    set_flag(Flags::Zero, false);

    onMachineCycleCb();

    return utils::to_word(lhs_msb, lhs_lsb);
}

void SM83::daa()
{
    uint8_t adj{};

    if ((!get_flag(Flags::Subtract) && (A & 0x0F) > 0x09) || get_flag(Flags::HalfCarry))
    {
        adj |= 0x06;
    }
    if ((!get_flag(Flags::Subtract) && A > 0x99) || get_flag(Flags::Carry))
    {
        adj |= 0x60;
        set_flag(Flags::Carry, true);
    }
    if (!get_flag(Flags::Subtract))
    {
        A += adj;
    }
    else
    {
        A -= adj;
    }

    set_flag(Flags::HalfCarry, false);
    set_flag(Flags::Zero, A == 0);
}

uint8_t SM83::sub(const uint8_t lhs, const uint8_t rhs, const bool borrow)
{
    const auto sub_borrow{static_cast<uint8_t>(borrow ? get_flag(Flags::Carry) : 0)};
    const auto result{static_cast<uint16_t>(lhs - rhs - sub_borrow)};

    set_flag(Flags::Zero, (result & 0xFF) == 0);
    set_flag(Flags::Subtract, true);
    set_flag(Flags::HalfCarry, (lhs & 0x0F) < (rhs & 0x0F) + sub_borrow);
    set_flag(Flags::Carry, lhs < rhs + sub_borrow);

    return static_cast<uint8_t>(result & 0xFF);
}

uint8_t SM83::bitwise_and(const uint8_t lhs, const uint8_t rhs)
{
    const uint8_t result = lhs & rhs;

    set_flag(Flags::Zero, result == 0);
    set_flag(Flags::Subtract, false);
    set_flag(Flags::HalfCarry, true);
    set_flag(Flags::Carry, false);

    return result;
}

uint8_t SM83::bitwise_or(const uint8_t lhs, const uint8_t rhs)
{
    const uint8_t result = lhs | rhs;

    set_flag(Flags::Zero, result == 0);
    set_flag(Flags::Subtract, false);
    set_flag(Flags::HalfCarry, false);
    set_flag(Flags::Carry, false);

    return result;
}

uint8_t SM83::bitwise_xor(const uint8_t lhs, const uint8_t rhs)
{
    const uint8_t result = lhs ^ rhs;
    set_flag(Flags::Zero, result == 0);
    set_flag(Flags::Subtract, false);
    set_flag(Flags::HalfCarry, false);
    set_flag(Flags::Carry, false);
    return result;
}

uint8_t SM83::rotate_left(uint8_t op, const bool circular)
{
    const auto new_carry{(op & 0x80) != 0};

    op <<= 1;
    if ((circular && new_carry) || (!circular && get_flag(Flags::Carry)))
    {
        op |= 0x01;
    }

    set_flag(Flags::Zero, op == 0);
    set_flag(Flags::Subtract, false);
    set_flag(Flags::HalfCarry, false);
    set_flag(Flags::Carry, new_carry);

    return op;
}

uint8_t SM83::rotate_right(uint8_t op, const bool circular)
{
    const auto new_carry{(op & 0x01) != 0};

    op >>= 1;
    if ((circular && new_carry) || (!circular && get_flag(Flags::Carry)))
    {
        op |= 0x80;
    }

    set_flag(Flags::Zero, op == 0);
    set_flag(Flags::Subtract, false);
    set_flag(Flags::HalfCarry, false);
    set_flag(Flags::Carry, new_carry);

    return op;
}

uint8_t SM83::shift_right(uint8_t op, const bool arithmetic)
{
    const auto new_carry{(op & 0x01) != 0};
    const auto sign{(op & 0x80) != 0};

    op = op >> 1 | (sign && arithmetic ? 0x80 : 0);

    set_flag(Flags::Zero, op == 0);
    set_flag(Flags::Subtract, false);
    set_flag(Flags::HalfCarry, false);
    set_flag(Flags::Carry, new_carry);

    return op;
}

uint8_t SM83::shift_left(uint8_t op)
{
    const auto new_carry{(op & 0x80) != 0};

    op <<= 1;

    set_flag(Flags::Zero, op == 0);
    set_flag(Flags::Subtract, false);
    set_flag(Flags::HalfCarry, false);
    set_flag(Flags::Carry, new_carry);

    return op;
}

void SM83::bit(const uint8_t op, const std::size_t bit)
{
    set_flag(Flags::Zero, (op & 1 << bit) == 0);
    set_flag(Flags::Subtract, false);
    set_flag(Flags::HalfCarry, true);
}

uint8_t SM83::res(const uint8_t op, const std::size_t bit)
{
    return op & ~(1 << bit);
}

uint8_t SM83::set(const uint8_t op, const std::size_t bit)
{
    return op | 1 << bit;
}

uint8_t SM83::swap(const uint8_t op)
{
    const auto result{static_cast<uint8_t>((op & 0x0F) << 4 | (op & 0xF0) >> 4)};

    set_flag(Flags::Zero, result == 0);
    set_flag(Flags::Subtract, false);
    set_flag(Flags::HalfCarry, false);
    set_flag(Flags::Carry, false);

    return result;
}

uint16_t SM83::inc(const uint16_t value)
{
    return value + 1;
}

uint16_t SM83::dec(const uint16_t value)
{
    return value - 1;
}

uint8_t SM83::inc(uint8_t value)
{
    value += 1;

    set_flag(Flags::Zero, value == 0);
    set_flag(Flags::Subtract, false);
    set_flag(Flags::HalfCarry, (value & 0x0F) == 0);

    return value;
}

uint8_t SM83::dec(uint8_t value)
{
    value -= 1;

    set_flag(Flags::Zero, value == 0);
    set_flag(Flags::Subtract, true);
    set_flag(Flags::HalfCarry, (value & 0x0F) == 0x0F);

    return value;
}

void SM83::jr()
{
    const auto e8{static_cast<int8_t>(fetch_operand())};

    PC += e8;
    onMachineCycleCb();
}

void SM83::jp()
{
    const auto lsb{fetch_operand()};
    const auto msb{fetch_operand()};

    PC = utils::to_word(msb, lsb);
    onMachineCycleCb();
}

void SM83::call()
{
    const auto lsb{fetch_operand()};
    const auto msb{fetch_operand()};

    push(PC);
    PC = utils::to_word(msb, lsb);
}

void SM83::call_cc(Conditionals conditional)
{
    if (is_condition_met(conditional))
    {
        call();
    }
    else
    {
        (void) fetch_operand();
        (void) fetch_operand();
    }
}

void SM83::ret()
{
    const auto lsb{fetch_memory(SP++)};
    const auto msb{fetch_memory(SP++)};

    PC = utils::to_word(msb, lsb);
    onMachineCycleCb();
}

void SM83::ret_cc(const Conditionals conditional)
{
    onMachineCycleCb();
    if (is_condition_met(conditional))
    {
        ret();
    }
}

void SM83::jp_cc(Conditionals conditional)
{
    if (is_condition_met(conditional))
    {
        jp();
    }
    else
    {
        (void) fetch_operand();
        (void) fetch_operand();
    }
}

void SM83::jr_cc(const Conditionals conditional)
{
    if (is_condition_met(conditional))
    {
        jr();
    }
    else
    {
        (void) fetch_operand();
    }
}

void SM83::rst(const ResetVector rst_vector)
{
    push(PC);
    PC = std::to_underlying(rst_vector);
}

void SM83::push(const uint8_t msb, const uint8_t lsb)
{
    onMachineCycleCb();
    write_memory(--SP, msb);
    write_memory(--SP, lsb);
}

void SM83::push(const uint16_t value)
{
    const auto lsb{utils::word_lsb(value)};
    const auto msb{utils::word_msb(value)};

    push(msb, lsb);
}

void SM83::pop(uint8_t& msb, uint8_t& lsb)
{
    lsb = fetch_memory(SP++);
    msb = fetch_memory(SP++);
}

void SM83::pop(uint16_t& value)
{
    const auto lsb{fetch_memory(SP++)};
    const auto msb{fetch_memory(SP++)};
    value = utils::to_word(msb, lsb);
}

void SM83::set_flag(const Flags flag, const bool value)
{
    const auto bit{std::to_underlying(flag)};
    F = (F & ~bit) | (value ? bit : 0);
}

bool SM83::get_flag(const Flags flag) const
{
    return (F & std::to_underlying(flag)) != 0;
}

bool SM83::is_condition_met(const Conditionals conditional) const
{
    switch (conditional)
    {
        case Conditionals::NZ:
            return !this->get_flag(Flags::Zero);
        case Conditionals::Z:
            return this->get_flag(Flags::Zero);
        case Conditionals::NC:
            return !this->get_flag(Flags::Carry);
        case Conditionals::C:
            return this->get_flag(Flags::Carry);
        [[unlikely]] default:
            throw std::logic_error("Conditional not implemented");
    }
}

uint8_t SM83::get_interrupt_request() const
{
    return static_cast<uint8_t>(IE & IF & 0x1F);
}

void SM83::interrupts()
{
    if (!IME)
    {
        return;
    }

    const auto bit_zero_count{std::countr_zero(get_interrupt_request())};
    uint16_t   interrupt_vector{};

    if (bit_zero_count == 8)
    {
        return;
    }

    IME              = false;
    interrupt_vector = 0x40 + bit_zero_count * 8;
    IF &= ~(1 << bit_zero_count);

    onMachineCycleCb();
    onMachineCycleCb();
    write_memory(--SP, utils::word_msb(PC));
    write_memory(--SP, utils::word_lsb(PC));
    onMachineCycleCb();

    PC = interrupt_vector;
}
