#include "CPU.hxx"

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

const char* CPU::BadRegister::what() const noexcept
{
    return "Bad Register";
}

CPU::CPU(Bus& bus) : bus(bus)
{
    this->reg.u8.A   = 0x01;
    this->reg.u8.F   = 0xB0;
    this->reg.u8.B   = 0x00;
    this->reg.u8.C   = 0x13;
    this->reg.u8.D   = 0x00;
    this->reg.u8.E   = 0xD8;
    this->reg.u8.H   = 0x01;
    this->reg.u8.L   = 0x4D;
    this->reg.u16.SP = 0xFFFE;
    this->reg.u16.PC = 0x0100;
}

void CPU::tick()
{
    if (++this->ticks < 4)
    {
        return;
    }
    this->ticks = 0;
    this->reg.u8.F &= 0xF0;

    switch (this->state)
    {
        case State::FETCH_DECODE:
            if (!this->handle_interrupt())
            {
                this->opcode = this->bus.read(this->reg.u16.PC);
                if (this->prefixed)
                {
                    this->instruction = prefixed_instruction_lookup[opcode];
                    this->prefixed    = false;
                }
                else
                {
                    this->instruction              = instruction_lookup[opcode];
                    this->disassembled_instruction = *this->disassemble(this->reg.u16.PC).begin();
                }
                if (this->should_trigger_halt_bug)
                {
                    this->should_trigger_halt_bug = false;
                }
                else
                {
                    this->reg.u16.PC++;
                }
                this->instruction.executor(this);
                this->is_instruction = true;
            }

            if (!this->micro_ops.empty())
            {
                this->state = State::EXECUTE;
            }
            else
            {
                if (this->opcode == 0xCB)
                {
                    break;
                }
                this->instruction_done();
            }
            break;
        case State::EXECUTE:
            this->micro_ops.front()();
            this->micro_ops.pop();
            if (this->micro_ops.empty())
            {
                if (this->is_instruction)
                {
                    this->instruction_done();
                }
                this->state = State::FETCH_DECODE;
            }
            break;
        case State::STOPPED:
        [[fallthrough]]
        case State::HALTED:
            break;
    };
}

auto CPU::disassemble(const uint16_t start, const std::optional<uint16_t> stop) const -> DisassembledInstruction
{
    DisassembledInstruction disassembled_instructions{};
    auto                    addr{start};

    do
    {
        DisassembledInstruction::mapped_type name{};
        Instruction                          instruction{};
        auto                                 base_addr{addr};
        auto                                 opcode{this->bus.read(addr++)};

        if (opcode == 0xCB)
        {
            opcode      = this->bus.read(addr++);
            instruction = prefixed_instruction_lookup[opcode];
        }
        else
        {
            instruction = instruction_lookup[opcode];
        }

        if (auto mode = instruction.addressing_mode)
        {
            switch (*mode)
            {
                case AddressingMode::RELATIVE:
                {
                    const auto value  = static_cast<int8_t>(this->bus.read(addr++));
                    const auto target = addr + value;

                    name = std::vformat(instruction.name, std::make_format_args(target));
                    break;
                }
                case AddressingMode::IMMEDIATE:
                {
                    const auto value = this->bus.read(addr++);
                    name             = std::vformat(instruction.name, std::make_format_args(value));
                    break;
                }
                case AddressingMode::IMMEDIATE_EXTENDED:
                {
                    const auto lsb   = this->bus.read(addr++);
                    const auto msb   = this->bus.read(addr++);
                    const auto value = u16(msb, lsb);

                    name = std::vformat(instruction.name, std::make_format_args(value));
                    break;
                }
            };
        }
        else
        {
            name = instruction.name;
        }

        disassembled_instructions.insert({base_addr, name});
    } while (stop && addr < *stop);

    return disassembled_instructions;
}

CPU::Register CPU::get_register() const noexcept
{
    return this->reg;
}

std::string_view CPU::get_register8_name(const Register8 reg)
{
    if (reg == &Register::U8::A)
        return "A";
    else if (reg == &Register::U8::B)
        return "B";
    else if (reg == &Register::U8::C)
        return "C";
    else if (reg == &Register::U8::D)
        return "D";
    else if (reg == &Register::U8::E)
        return "E";
    else if (reg == &Register::U8::H)
        return "H";
    else if (reg == &Register::U8::L)
        return "L";
    else
        throw BadRegister();
}

uint8_t CPU::get_b3() const
{
    return (this->opcode >> 3) & 0b00000111;
}

uint8_t CPU::get_tgt3() const
{
    return (this->opcode & 0b00111000);
}

CPU::Register8 CPU::get_register8(const Register8Position shift) const
{
    switch (static_cast<Register8Placeholder>((this->opcode >> static_cast<uint8_t>(shift)) & 0b00000111U))
    {
        case Register8Placeholder::A:
            return &Register::U8::A;
        case Register8Placeholder::B:
            return &Register::U8::B;
        case Register8Placeholder::C:
            return &Register::U8::C;
        case Register8Placeholder::D:
            return &Register::U8::D;
        case Register8Placeholder::E:
            return &Register::U8::E;
        case Register8Placeholder::H:
            return &Register::U8::H;
        case Register8Placeholder::L:
            return &Register::U8::L;
        default:
            throw BadRegister();
    }
}

CPU::Register16 CPU::get_register16() const
{
    switch (static_cast<Register16Placeholder>((this->opcode & 0b00110000U) >> 4))
    {
        case Register16Placeholder::BC:
            return &Register::U16::BC;
        case Register16Placeholder::DE:
            return &Register::U16::DE;
        case Register16Placeholder::HL:
            return &Register::U16::HL;
        case Register16Placeholder::SP:
            return &Register::U16::SP;
        default:
            throw BadRegister();
    }
}

std::pair<CPU::Register16MemoryPlaceholder, CPU::Register16> CPU::get_register16_memory() const
{
    const auto r16placeholder{static_cast<Register16MemoryPlaceholder>((this->opcode & 0b00110000U) >> 4)};
    Register16 reg{};

    switch (r16placeholder)
    {
        case Register16MemoryPlaceholder::BC:
            reg = &Register::U16::BC;
            break;
        case Register16MemoryPlaceholder::DE:
            reg = &Register::U16::DE;
            break;
        case Register16MemoryPlaceholder::HL_PLUS:
        case Register16MemoryPlaceholder::HL_MINUS:
            reg = &Register::U16::HL;
            break;
        default:
            throw BadRegister();
    }

    return std::make_pair(r16placeholder, reg);
}

CPU::Register16 CPU::get_register16_stack() const
{
    switch (static_cast<Register16StackPlaceholder>((this->opcode & 0b00110000U) >> 4))
    {
        case Register16StackPlaceholder::BC:
            return &Register::U16::BC;
        case Register16StackPlaceholder::DE:
            return &Register::U16::DE;
        case Register16StackPlaceholder::HL:
            return &Register::U16::HL;
        case Register16StackPlaceholder::AF:
            return &Register::U16::AF;
        default:
            throw BadRegister();
    }
}

bool CPU::check_condition_is_met() const
{
    switch (static_cast<ConditionalPlaceholder>((this->opcode >> 3) & 0b00000011U))
    {
        case ConditionalPlaceholder::C:
            return this->carry() == true;
        case ConditionalPlaceholder::Z:
            return this->zero() == true;
        case ConditionalPlaceholder::NC:
            return this->carry() == false;
        case ConditionalPlaceholder::NZ:
            return this->zero() == false;
        default:
            throw BadRegister();
    }
}
void CPU::print_state() const
{
    std::print(
        std::cout,
        "A:{:02X} F:{:02X} B:{:02X} C:{:02X} D:{:02X} E:{:02X} H:{:02X} L:{:02X} SP:{:04X} PC:{:04X} PCMEM:{:02X},{:02X},{:02X},{:02X}\n",
        this->reg.u8.A, this->reg.u8.F, this->reg.u8.B, this->reg.u8.C, this->reg.u8.D, this->reg.u8.E, this->reg.u8.H,
        this->reg.u8.L, static_cast<uint16_t>(this->reg.u16.SP), static_cast<uint16_t>(this->reg.u16.PC),
        this->bus.read(this->reg.u16.PC), this->bus.read(this->reg.u16.PC + 1), this->bus.read(this->reg.u16.PC + 2),
        this->bus.read(this->reg.u16.PC + 3));
}

void CPU::instruction_done()
{
    this->handle_interrupt();

    if (this->ei_counter != 0)
    {
        this->ei_counter--;
        if (this->ei_counter == 0)
        {
            this->ime = true;
        }
    }
}

void CPU::handle_interrupt()
{
    if (!this->ime)
    {
        return;
    }

    const auto interrupt_enable{this->bus.read(0xFFFF)};
    auto       interrupt_flag(this->bus.read(0xFF0F));
    const auto interrupt_request{static_cast<uint8_t>(interrupt_enable & interrupt_flag)};
    const auto zero_count{std::countr_zero(interrupt_request)};
    uint16_t   interrupt_vector{};

    if (zero_count == 8)
    {
        return;
    }

    interrupt_vector = 0x40 + zero_count * 8;

    this->ime = false;
    interrupt_flag &= ~(1 << zero_count);
    this->bus.write(0xFF0F, interrupt_flag);

    this->micro_ops.emplace([] {});
    this->micro_ops.emplace([this] { this->reg.u16.SP--; });
    this->micro_ops.emplace([this] { this->bus.write(this->reg.u16.SP--, u16_msb(this->reg.u16.PC)); });
    this->micro_ops.emplace([this] { this->bus.write(this->reg.u16.SP, u16_lsb(this->reg.u16.PC)); });
    this->micro_ops.emplace([this, interrupt_vector] { this->reg.u16.PC = interrupt_vector; });

    this->state          = State::EXECUTE;
    this->is_instruction = false;
}

void CPU::set_carry(const bool carry)
{
    this->reg.u8.F = this->reg.u8.F & ~Flags::CARRY | (carry ? Flags::CARRY : 0);
}

bool CPU::carry() const
{
    return (this->reg.u8.F & Flags::CARRY) != 0;
}

void CPU::set_half_carry(const bool half_carry)
{
    this->reg.u8.F = this->reg.u8.F & ~Flags::HALF_CARRY | (half_carry ? Flags::HALF_CARRY : 0);
}

bool CPU::half_carry() const
{
    return (this->reg.u8.F & Flags::HALF_CARRY) != 0;
}

void CPU::set_subtract(const bool subtract)
{
    this->reg.u8.F = this->reg.u8.F & ~Flags::SUBTRACT | (subtract ? Flags::SUBTRACT : 0);
}

bool CPU::subtract() const
{
    return (this->reg.u8.F & Flags::SUBTRACT) != 0;
}

void CPU::set_zero(const bool zero)
{
    this->reg.u8.F = this->reg.u8.F & ~Flags::ZERO | (zero ? Flags::ZERO : 0);
}

bool CPU::zero() const
{
    return (this->reg.u8.F & Flags::ZERO) != 0;
}
