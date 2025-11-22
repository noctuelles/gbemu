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

SM83::SM83(EmulationState& emulationState, Addressable& bus, Ticking& timer, Ticking& ppu)
    : emulationState(emulationState), bus(bus), timer(timer), ppu(ppu)
{
    A  = 0x01;
    C  = 0x13;
    E  = 0xD8;
    H  = 0x01;
    L  = 0x4D;
    PC = 0x100;
    SP = 0xFFFE;

    instructionBuffer.reserve(16);
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
        case MemoryMap::IORegisters::DMA:
            oamDmaSourceAddress = static_cast<uint16_t>(value << 8);
            requestOamDma       = 3;
            break;
        default:
            throw std::logic_error(std::format("Invalid SM83 Write"));
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
        case MemoryMap::IORegisters::DMA:
            return static_cast<uint8_t>(oamDmaSourceAddress >> 8);
        default:
            std::cout << "Invalid SM83 Read" << std::endl;
            throw std::logic_error("Invalid SM83 Write");
    }
}

Addressable::AddressableRange SM83::getAddressableRange() const noexcept
{
    return {MemoryMap::IE, MemoryMap::IORegisters::IF, MemoryMap::IORegisters::DMA};
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

            // Disassembler disassembler{instructionBuffer};
            // const auto inst{disassembler.disassemble(0, 1)};

            // for (const auto& line : inst)
            // {
            //     for (const auto byte: line.first.second)
            //     {
            //         std::cout << std::format("{:02X} ", byte);
            //     }
            //     std::cout << ": " << line.second << std::endl;
            // }

            instructionBuffer.clear();
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

void SM83::onMachineCycleCb()
{
    if (requestOamDma > 0)
    {
        requestOamDma -= 1;
        if (requestOamDma == 0)
        {
            emulationState.isInOamDma = true;
            oamDmaElapsedMachineCycles       = 0;
        }
    }

    if (emulationState.isInOamDma)
    {
        oamDmaElapsedMachineCycles += 1;
        if (oamDmaElapsedMachineCycles == 160)
        {
            emulationState.isInOamDma = false;

            for (auto address{MemoryMap::OAM.first}; address <= MemoryMap::OAM.second; address += 1)
            {
                bus.write(address, bus.read(oamDmaSourceAddress + (address - MemoryMap::OAM.first)));
            }
        }
    }

    timer.tick();

    ppu.tick();
    ppu.tick();
    ppu.tick();
    ppu.tick();
}

void SM83::fetchInstruction()
{
    IR = fetchMemory(PC++);
    instructionBuffer.push_back(IR);
}

uint8_t SM83::fetchMemory(const uint16_t address)
{
    onMachineCycleCb();

    /* Let the CPU read the DMA source address even if it is in OAM DMA. */
    if (emulationState.isInOamDma && address == MemoryMap::IORegisters::DMA)
    {
        return static_cast<uint8_t>(oamDmaSourceAddress >> 8);
    }

    return bus.read(address);
}

uint8_t SM83::fetchOperand()
{
    const auto byte{fetchMemory(PC++)};
    instructionBuffer.push_back(byte);
    return byte;
}

void SM83::writeMemory(const uint16_t address, const uint8_t value)
{
    onMachineCycleCb();

    if (emulationState.isInOamDma && address == MemoryMap::IORegisters::DMA)
    {
        /* Do we need to restart OAM DMA ? */
        if (oamDmaElapsedMachineCycles != 0)
        {
            requestOamDma = 3;
        }

        oamDmaSourceAddress = static_cast<uint16_t>(value << 8);
    }

    return bus.write(address, value);
}

uint16_t SM83::AF() const
{
    return Utils::to_word(A, F);
}

uint16_t SM83::BC() const
{
    return Utils::to_word(B, C);
}

uint16_t SM83::DE() const
{
    return Utils::to_word(D, E);
}

uint16_t SM83::HL() const
{
    return Utils::to_word(H, L);
}

void SM83::AF(const uint16_t value)
{
    Utils::to_bytes(value, A, F);
}
void SM83::BC(const uint16_t value)
{
    Utils::to_bytes(value, B, C);
}

void SM83::DE(const uint16_t value)
{
    Utils::to_bytes(value, D, E);
}

void SM83::HL(const uint16_t value)
{
    Utils::to_bytes(value, H, L);
}

uint8_t SM83::add(const uint8_t lhs, const uint8_t rhs, const bool carry)
{
    const auto add_carry{static_cast<uint8_t>(carry ? getFlag(Flags::Carry) : 0)};
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
    auto       lhs_lsb{Utils::wordLsb(lhs)};
    auto       lhs_msb{Utils::wordMsb(lhs)};

    lhs_lsb = add(lhs_lsb, rhs);

    onMachineCycleCb();

    if (getFlag(Flags::Carry) && !sign)
    {
        lhs_msb += 1;
    }
    else if (!getFlag(Flags::Carry) && sign)
    {
        lhs_msb -= 1;
    }

    set_flag(Flags::Zero, false);

    onMachineCycleCb();

    return Utils::to_word(lhs_msb, lhs_lsb);
}

void SM83::daa()
{
    uint8_t adj{};

    if ((!getFlag(Flags::Subtract) && (A & 0x0F) > 0x09) || getFlag(Flags::HalfCarry))
    {
        adj |= 0x06;
    }
    if ((!getFlag(Flags::Subtract) && A > 0x99) || getFlag(Flags::Carry))
    {
        adj |= 0x60;
        set_flag(Flags::Carry, true);
    }
    if (!getFlag(Flags::Subtract))
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
    const auto sub_borrow{static_cast<uint8_t>(borrow ? getFlag(Flags::Carry) : 0)};
    const auto result{static_cast<uint16_t>(lhs - rhs - sub_borrow)};

    set_flag(Flags::Zero, (result & 0xFF) == 0);
    set_flag(Flags::Subtract, true);
    set_flag(Flags::HalfCarry, (lhs & 0x0F) < (rhs & 0x0F) + sub_borrow);
    set_flag(Flags::Carry, lhs < rhs + sub_borrow);

    return static_cast<uint8_t>(result & 0xFF);
}

uint8_t SM83::bitwiseAnd(const uint8_t lhs, const uint8_t rhs)
{
    const uint8_t result = lhs & rhs;

    set_flag(Flags::Zero, result == 0);
    set_flag(Flags::Subtract, false);
    set_flag(Flags::HalfCarry, true);
    set_flag(Flags::Carry, false);

    return result;
}

uint8_t SM83::bitwiseOr(const uint8_t lhs, const uint8_t rhs)
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
    if ((circular && new_carry) || (!circular && getFlag(Flags::Carry)))
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
    if ((circular && new_carry) || (!circular && getFlag(Flags::Carry)))
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
    const auto e8{static_cast<int8_t>(fetchOperand())};

    PC += e8;
    onMachineCycleCb();
}

void SM83::jp()
{
    const auto lsb{fetchOperand()};
    const auto msb{fetchOperand()};

    PC = Utils::to_word(msb, lsb);
    onMachineCycleCb();
}

void SM83::call()
{
    const auto lsb{fetchOperand()};
    const auto msb{fetchOperand()};

    push(PC);
    PC = Utils::to_word(msb, lsb);
}

void SM83::call_cc(Conditionals conditional)
{
    if (isConditionMet(conditional))
    {
        call();
    }
    else
    {
        (void) fetchOperand();
        (void) fetchOperand();
    }
}

void SM83::ret()
{
    const auto lsb{fetchMemory(SP++)};
    const auto msb{fetchMemory(SP++)};

    PC = Utils::to_word(msb, lsb);
    onMachineCycleCb();
}

void SM83::ret_cc(const Conditionals conditional)
{
    onMachineCycleCb();
    if (isConditionMet(conditional))
    {
        ret();
    }
}

void SM83::jp_cc(Conditionals conditional)
{
    if (isConditionMet(conditional))
    {
        jp();
    }
    else
    {
        (void) fetchOperand();
        (void) fetchOperand();
    }
}

void SM83::jr_cc(const Conditionals conditional)
{
    if (isConditionMet(conditional))
    {
        jr();
    }
    else
    {
        (void) fetchOperand();
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
    writeMemory(--SP, msb);
    writeMemory(--SP, lsb);
}

void SM83::push(const uint16_t value)
{
    const auto lsb{Utils::wordLsb(value)};
    const auto msb{Utils::wordMsb(value)};

    push(msb, lsb);
}

void SM83::pop(uint8_t& msb, uint8_t& lsb)
{
    lsb = fetchMemory(SP++);
    msb = fetchMemory(SP++);
}

void SM83::pop(uint16_t& value)
{
    const auto lsb{fetchMemory(SP++)};
    const auto msb{fetchMemory(SP++)};
    value = Utils::to_word(msb, lsb);
}

void SM83::set_flag(const Flags flag, const bool value)
{
    const auto bit{std::to_underlying(flag)};
    F = (F & ~bit) | (value ? bit : 0);
}

bool SM83::getFlag(const Flags flag) const
{
    return (F & std::to_underlying(flag)) != 0;
}

bool SM83::isConditionMet(const Conditionals conditional) const
{
    switch (conditional)
    {
        case Conditionals::NZ:
            return !this->getFlag(Flags::Zero);
        case Conditionals::Z:
            return this->getFlag(Flags::Zero);
        case Conditionals::NC:
            return !this->getFlag(Flags::Carry);
        case Conditionals::C:
            return this->getFlag(Flags::Carry);
        [[unlikely]] default:
            throw std::logic_error("Conditional not implemented");
    }
}

uint8_t SM83::getInterruptRequest() const
{
    return static_cast<uint8_t>(IE & IF & 0x1F);
}

void SM83::interrupts()
{
    if (!IME)
    {
        return;
    }

    const auto bitZeroCount{std::countr_zero(getInterruptRequest())};
    uint16_t   interruptVector{};

    if (bitZeroCount == 8)
    {
        return;
    }

    IME             = false;
    interruptVector = 0x40 + bitZeroCount * 8;
    IF &= ~(1 << bitZeroCount);

    onMachineCycleCb();
    onMachineCycleCb();
    writeMemory(--SP, Utils::wordMsb(PC));
    writeMemory(--SP, Utils::wordLsb(PC));
    onMachineCycleCb();

    PC = interruptVector;
}
