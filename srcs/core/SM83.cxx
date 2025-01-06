#include "SM83.hxx"

/**
 * @note https://gbdev.io/gb-opcodes/optables/
 * https://rgbds.gbdev.io/docs/v0.8.0/gbz80.7
 * https://dn790000.ca.archive.org/0/items/GameBoyProgManVer1.1/GameBoyProgManVer1.1.pdf
 */

#include <X11/extensions/randr.h>

#include <Utils.hxx>
#include <bitset>
#include <cassert>
#include <format>
#include <iostream>
#include <utility>

SM83::SM83(Bus& bus) : bus(bus)
{
    this->A  = 0x01;
    this->F  = 0xB0;
    this->B  = 0x00;
    this->C  = 0x13;
    this->D  = 0x00;
    this->E  = 0xD8;
    this->H  = 0x01;
    this->L  = 0x4D;
    this->SP = 0xFFFE;
    this->PC = 0x0100;
}

void SM83::tick()
{
    switch (state)
    {
        case State::NORMAL:
            if (request_ime != 0)
            {
                request_ime -= 1;
                if (request_ime == 0)
                {
                    this->ime = true;
                }
            }
            fetch_decode_execute();
            break;
        case State::STOPPED:
        case State::HALTED:
            break;
    }
}

SM83::byte SM83::fetch_memory(const word address)
{
    machine_cycle();
    return bus.read(address);
}

void SM83::write_memory(const word address, const byte value)
{
    machine_cycle();
    return bus.write(address, value);
}

SM83::word SM83::AF() const
{
    return utils::to_word(A, F);
}

SM83::word SM83::BC() const
{
    return utils::to_word(B, C);
}

SM83::word SM83::DE() const
{
    return utils::to_word(D, E);
}

SM83::word SM83::HL() const
{
    return utils::to_word(H, L);
}

void SM83::AF(const word value)
{
    utils::to_bytes(value, A, F);
}
void SM83::BC(const word value)
{
    utils::to_bytes(value, B, C);
}

void SM83::DE(const word value)
{
    utils::to_bytes(value, D, E);
}

void SM83::HL(const word value)
{
    utils::to_bytes(value, H, L);
}

SM83::byte SM83::add(const byte lhs, const byte rhs, const bool carry)
{
    const auto add_carry{static_cast<byte>(carry ? get_flag(Flags::Carry) : 0)};
    const auto result{static_cast<word>(lhs + rhs + add_carry)};

    set_flag(Flags::Zero, (result & 0xFF) == 0);
    set_flag(Flags::Subtract, false);
    set_flag(Flags::HalfCarry, (lhs & 0x0F) + (rhs & 0x0F) + add_carry > 0x0F);
    set_flag(Flags::Carry, result > 0xFF);

    return static_cast<byte>(result & 0xFF);
}

SM83::word SM83::add(const word lhs, const word rhs)
{
    const signed_dword result{lhs + rhs};

    set_flag(Flags::Subtract, false);
    set_flag(Flags::HalfCarry, (lhs & 0x0FFF) + (rhs & 0x0FFF) > 0x0FFF);
    set_flag(Flags::Carry, result > 0xFFFF);

    return static_cast<word>(result);
}

SM83::word SM83::add(const word lhs, const byte rhs)
{
    const auto sign{(rhs & 0x80) != 0};
    auto       lhs_lsb{utils::word_lsb(lhs)};
    auto       lhs_msb{utils::word_msb(lhs)};

    lhs_lsb = add(lhs_lsb, rhs);

    machine_cycle();

    if (get_flag(Flags::Carry) && !sign)
    {
        lhs_msb += 1;
    }
    else if (!get_flag(Flags::Carry) && sign)
    {
        lhs_msb -= 1;
    }

    set_flag(Flags::Zero, false);

    machine_cycle();

    return utils::to_word(lhs_msb, lhs_lsb);
}

SM83::byte SM83::sub(const byte lhs, const byte rhs, const bool borrow)
{
    const auto sub_borrow{static_cast<byte>(borrow ? get_flag(Flags::Carry) : 0)};
    const auto result{static_cast<word>(lhs - rhs - sub_borrow)};

    set_flag(Flags::Zero, (result & 0xFF) == 0);
    set_flag(Flags::Subtract, true);
    set_flag(Flags::HalfCarry, (lhs & 0x0F) < (rhs & 0x0F) + sub_borrow);
    set_flag(Flags::Carry, lhs < rhs + sub_borrow);

    return static_cast<byte>(result & 0xFF);
}

SM83::byte SM83::bitwise_and(const byte lhs, const byte rhs)
{
    const byte result = lhs & rhs;
    set_flag(Flags::Zero, result == 0);
    set_flag(Flags::Subtract, false);
    set_flag(Flags::HalfCarry, true);
    set_flag(Flags::Carry, false);
    return result;
}

SM83::byte SM83::bitwise_or(const byte lhs, const byte rhs)
{
    const byte result = lhs | rhs;
    set_flag(Flags::Zero, result == 1);
    set_flag(Flags::Subtract, false);
    set_flag(Flags::HalfCarry, false);
    set_flag(Flags::Carry, false);
    return result;
}

SM83::byte SM83::bitwise_xor(const byte lhs, const byte rhs)
{
    const byte result = lhs ^ rhs;
    set_flag(Flags::Zero, result == 0);
    set_flag(Flags::Subtract, false);
    set_flag(Flags::HalfCarry, false);
    set_flag(Flags::Carry, false);
    return result;
}

SM83::byte SM83::rotate_left(byte op, bool circular)
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

SM83::byte SM83::rotate_right(byte op, bool circular)
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

void SM83::bit(const byte op, const std::size_t bit)
{
    set_flag(Flags::Zero, (op & 1 << bit) == 0);
    set_flag(Flags::Subtract, false);
    set_flag(Flags::HalfCarry, true);
}

SM83::byte SM83::res(const byte op, const std::size_t bit)
{
    return op & ~(1 << bit);
}

SM83::byte SM83::set(const byte op, const std::size_t bit)
{
    return op | 1 << bit;
}

SM83::byte SM83::swap(const byte op)
{
    const auto result{static_cast<byte>((op & 0x0F) << 4 | (op & 0xF0) >> 4)};

    set_flag(Flags::Zero, result == 0);
    set_flag(Flags::Subtract, false);
    set_flag(Flags::HalfCarry, false);
    set_flag(Flags::Carry, false);

    return result;
}

SM83::word SM83::inc(const word value)
{
    return value + 1;
}

SM83::word SM83::dec(const word value)
{
    return value - 1;
}

SM83::byte SM83::inc(byte value)
{
    value += 1;

    set_flag(Flags::Zero, value == 0);
    set_flag(Flags::Subtract, false);
    set_flag(Flags::HalfCarry, (value & 0x0F) == 0);

    return value;
}

SM83::byte SM83::dec(byte value)
{
    value += 1;

    set_flag(Flags::Zero, value == 0);
    set_flag(Flags::Subtract, true);
    set_flag(Flags::HalfCarry, (value & 0x0F) == 0x0F);

    return value;
}

void SM83::jr()
{
    const auto e8{static_cast<signed_byte>(fetch_memory(PC++))};

    PC += e8;
    machine_cycle();
}

void SM83::jp()
{
    const auto lsb{fetch_memory(PC++)};
    const auto msb{fetch_memory(PC++)};

    PC = utils::to_word(msb, lsb);
    machine_cycle();
}

void SM83::call()
{
    const auto lsb{fetch_memory(PC++)};
    const auto msb{fetch_memory(PC++)};

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
        (void) fetch_memory(PC++);
        (void) fetch_memory(PC++);
    }
}

void SM83::ret()
{
    const auto lsb{fetch_memory(SP++)};
    const auto msb{fetch_memory(SP++)};

    PC = utils::to_word(msb, lsb);
    machine_cycle();
}

void SM83::ret_cc(const Conditionals conditional)
{
    machine_cycle();
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
        (void) fetch_memory(PC++);
        (void) fetch_memory(PC++);
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
        (void) fetch_memory(PC++);
    }
}

void SM83::rst(const ResetVector rst_vector)
{
    push(PC);
    PC = std::to_underlying(rst_vector);
}

void SM83::push(const byte msb, const byte lsb)
{
    machine_cycle();
    write_memory(--SP, msb);
    write_memory(--SP, lsb);
}

void SM83::push(const word value)
{
    const auto lsb{utils::word_lsb(value)};
    const auto msb{utils::word_msb(value)};

    machine_cycle();
    write_memory(--SP, msb);
    write_memory(--SP, lsb);
}

void SM83::pop(byte& msb, byte& lsb)
{
    lsb = fetch_memory(SP++);
    msb = fetch_memory(SP++);
}

void SM83::pop(word& value)
{
    const auto lsb{fetch_memory(SP++)};
    const auto msb{fetch_memory(SP++)};
    value = utils::to_word(msb, lsb);
}

void SM83::set_flag(const Flags flag, const bool value)
{
    const auto bit{std::to_underlying(flag)};
    F = F & ~bit | (value ? bit : 0);
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