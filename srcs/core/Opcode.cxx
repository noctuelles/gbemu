//
// Created by plouvel on 12/27/24.
//

#include <format>

#include "SM83.hxx"
#include "Utils.hxx"

void SM83::machine_cycle() {}

/**
 * Executes the fetch, decode, and execute cycle for a SM83 instruction.
 *
 * This method performs the fetching of the instruction from memory, decoding
 * the instruction to determine its intended operation, and executing the
 * corresponding action. Depending on the provided instruction set mode, it
 * may execute standard or extended operations.
 *
 * @param extended_set Determines whether to use the extended instruction set.
 *                     Pass `true` to activate instructions from the extended set (0xCB prefixed),
 *                     otherwise pass `false` to execute standard instructions.
 */
void SM83::fetch_decode_execute(const bool extended_set)  // NOLINT
{
    ir = fetch_memory(PC++);
    if (!extended_set)
    {
        switch (ir)
        {
            case 0x00:
                break;
            case 0x01:
                C = fetch_memory(PC++);
                B = fetch_memory(PC++);
                break;
            case 0x02:
                write_memory(BC(), A);
                break;
            case 0x03:
                BC(BC() + 1);
                break;
            case 0x04:
                B = inc(B);
                break;
            case 0x05:
                B = dec(B);
                break;
            case 0x06:
                B = fetch_memory(PC++);
                break;
            case 0x07:
                A = rotate_left(A, true);
                set_flag(Flags::Zero, false);
                break;
            case 0x08:
            {
                const auto lsb{fetch_memory(PC++)};
                const auto msb{fetch_memory(PC++)};
                const auto address{utils::to_word(msb, lsb)};

                write_memory(address, utils::word_lsb(SP));
                write_memory(address + 1, utils::word_msb(SP));
            }
            break;
            case 0x09:
                HL(add(HL(), BC()));
                break;
            case 0x0A:
                A = fetch_memory(BC());
                break;
            case 0x0B:
                BC(BC() - 1);
                break;
            case 0x0C:
                C = inc(C);
                break;
            case 0x0D:
                C = dec(C);
                break;
            case 0x0E:
                C = fetch_memory(PC++);
                break;
            case 0x0F:
                A = rotate_right(A, true);
                set_flag(Flags::Zero, false);
                break;
            case 0x10:
                state = State::STOPPED;
                break;
            case 0x11:
                E = fetch_memory(PC++);
                D = fetch_memory(PC++);
                break;
            case 0x12:
                write_memory(DE(), A);
                break;
            case 0x13:
                DE(DE() + 1);
                break;
            case 0x14:
                D = inc(D);
                break;
            case 0x15:
                D = dec(D);
                break;
            case 0x16:
                D = fetch_memory(PC++);
                break;
            case 0x17:
                A = rotate_left(A, false);
                set_flag(Flags::Zero, false);
                break;
            case 0x18:
                jr();
                break;
            case 0x19:
                HL(add(HL(), DE()));
                break;
            case 0x1A:
                A = fetch_memory(DE());
                break;
            case 0x1B:
                DE(DE() - 1);
                break;
            case 0x1C:
                E = inc(E);
                break;
            case 0x1D:
                E = dec(E);
                break;
            case 0x1E:
                E = fetch_memory(PC++);
                break;
            case 0x1F:
                A = rotate_right(A, false);
                set_flag(Flags::Zero, false);
                break;
            case 0x20:
                jr_cc(Conditionals::NZ);
                break;
            case 0x21:
                L = fetch_memory(PC++);
                H = fetch_memory(PC++);
                break;
            case 0x22:
                write_memory(HL(), A);
                HL(HL() + 1);
                break;
            case 0x23:
                HL(HL() + 1);
                break;
            case 0x24:
                H = inc(H);
                break;
            case 0x25:
                H = dec(H);
                break;
            case 0x26:
                H = fetch_memory(PC++);
                break;
            case 0x27:
                daa();
                break;
            case 0x28:
                jr_cc(Conditionals::Z);
                break;
            case 0x29:
                HL(add(HL(), HL()));
                break;
            case 0x2A:
                A = fetch_memory(HL());
                HL(HL() + 1);
                break;
            case 0x2B:
                HL(HL() - 1);
                break;
            case 0x2C:
                L = inc(L);
                break;
            case 0x2D:
                L = dec(L);
                break;
            case 0x2E:
                L = fetch_memory(PC++);
                break;
            case 0x2F:
                A = ~A;
                set_flag(Flags::Subtract, true);
                set_flag(Flags::HalfCarry, true);
                break;
            case 0x30:
                jr_cc(Conditionals::NC);
                break;
            case 0x31:
                SP = fetch_memory(PC++);
                SP |= fetch_memory(PC++) << 8;
                break;
            case 0x32:
                write_memory(HL(), A);
                HL(HL() - 1);
                break;
            case 0x33:
                SP += 1;
                break;
            case 0x34:
                write_memory(HL(), inc(fetch_memory(HL())));
                break;
            case 0x35:
                write_memory(HL(), dec(fetch_memory(HL())));
                break;
            case 0x36:
                write_memory(HL(), fetch_memory(PC++));
                break;
            case 0x37:
                set_flag(Flags::Subtract, false);
                set_flag(Flags::HalfCarry, false);
                set_flag(Flags::Carry, true);
                break;
            case 0x38:
                jr_cc(Conditionals::C);
                break;
            case 0x39:
                HL(add(HL(), SP));
                break;
            case 0x3A:
                A = fetch_memory(HL());
                HL(HL() - 1);
                break;
            case 0x3B:
                SP -= 1;
                break;
            case 0x3C:
                A = inc(A);
                break;
            case 0x3D:
                A = dec(A);
                break;
            case 0x3E:
                A = fetch_memory(PC++);
                break;
            case 0x3F:
                set_flag(Flags::Subtract, false);
                set_flag(Flags::HalfCarry, false);
                set_flag(Flags::Carry, !get_flag(Flags::Carry));
                break;
            case 0x40:
                break;
            case 0x41:
                B = C;
                break;
            case 0x42:
                B = D;
                break;
            case 0x43:
                B = E;
                break;
            case 0x44:
                B = H;
                break;
            case 0x45:
                B = L;
                break;
            case 0x46:
                B = fetch_memory(HL());
                break;
            case 0x47:
                B = A;
                break;
            case 0x48:
                C = B;
                break;
            case 0x49:
                break;
            case 0x4A:
                C = D;
                break;
            case 0x4B:
                C = E;
                break;
            case 0x4C:
                C = H;
                break;
            case 0x4D:
                C = L;
                break;
            case 0x4E:
                C = fetch_memory(HL());
                break;
            case 0x4F:
                C = A;
                break;
            case 0x50:
                D = B;
                break;
            case 0x51:
                D = C;
                break;
            case 0x52:
                break;
            case 0x53:
                D = E;
                break;
            case 0x54:
                D = H;
                break;
            case 0x55:
                D = L;
                break;
            case 0x56:
                D = fetch_memory(HL());
                break;
            case 0x57:
                D = A;
                break;
            case 0x58:
                E = B;
                break;
            case 0x59:
                E = C;
                break;
            case 0x5A:
                E = D;
                break;
            case 0x5B:
                break;
            case 0x5C:
                E = H;
                break;
            case 0x5D:
                E = L;
                break;
            case 0x5E:
                E = fetch_memory(HL());
                break;
            case 0x5F:
                E = A;
                break;
            case 0x60:
                H = B;
                break;
            case 0x61:
                H = C;
                break;
            case 0x62:
                H = D;
                break;
            case 0x63:
                H = E;
                break;
            case 0x64:
                break;
            case 0x65:
                H = L;
                break;
            case 0x66:
                H = fetch_memory(HL());
                break;
            case 0x67:
                H = A;
                break;
            case 0x68:
                L = B;
                break;
            case 0x69:
                L = C;
                break;
            case 0x6A:
                L = D;
                break;
            case 0x6B:
                L = E;
                break;
            case 0x6C:
                L = H;
                break;
            case 0x6D:
                break;
            case 0x6E:
                L = fetch_memory(HL());
                break;
            case 0x6F:
                L = A;
                break;
            case 0x70:
                write_memory(HL(), B);
                break;
            case 0x71:
                write_memory(HL(), C);
                break;
            case 0x72:
                write_memory(HL(), D);
                break;
            case 0x73:
                write_memory(HL(), E);
                break;
            case 0x74:
                write_memory(HL(), H);
                break;
            case 0x75:
                write_memory(HL(), L);
                break;
            case 0x76:
                state = State::HALTED;
                break;
            case 0x77:
                write_memory(HL(), A);
                break;
            case 0x78:
                A = B;
                break;
            case 0x79:
                A = C;
                break;
            case 0x7A:
                A = D;
                break;
            case 0x7B:
                A = E;
                break;
            case 0x7C:
                A = H;
                break;
            case 0x7D:
                A = L;
                break;
            case 0x7E:
                A = fetch_memory(HL());
                break;
            case 0x7F:
                break;
            case 0x80:
                A = add(A, B);
                break;
            case 0x81:
                A = add(A, C);
                break;
            case 0x82:
                A = add(A, D);
                break;
            case 0x83:
                A = add(A, E);
                break;
            case 0x84:
                A = add(A, H);
                break;
            case 0x85:
                A = add(A, L);
                break;
            case 0x86:
                A = add(A, fetch_memory(HL()));
                break;
            case 0x87:
                A = add(A, A);
                break;
            case 0x88:
                A = add(A, B, true);
                break;
            case 0x89:
                A = add(A, C, true);
                break;
            case 0x8A:
                A = add(A, D, true);
                break;
            case 0x8B:
                A = add(A, E, true);
                break;
            case 0x8C:
                A = add(A, H, true);
                break;
            case 0x8D:
                A = add(A, L, true);
                break;
            case 0x8E:
                A = add(A, fetch_memory(HL()), true);
                break;
            case 0x8F:
                A = add(A, A, true);
                break;
            case 0x90:
                A = sub(A, B);
                break;
            case 0x91:
                A = sub(A, C);
                break;
            case 0x92:
                A = sub(A, D);
                break;
            case 0x93:
                A = sub(A, E);
                break;
            case 0x94:
                A = sub(A, H);
                break;
            case 0x95:
                A = sub(A, L);
                break;
            case 0x96:
                A = sub(A, fetch_memory(HL()));
                break;
            case 0x97:
                A = sub(A, A);
                break;
            case 0x98:
                A = sub(A, B, true);
                break;
            case 0x99:
                A = sub(A, C, true);
                break;
            case 0x9A:
                A = sub(A, D, true);
                break;
            case 0x9B:
                A = sub(A, E, true);
                break;
            case 0x9C:
                A = sub(A, H, true);
                break;
            case 0x9D:
                A = sub(A, L, true);
                break;
            case 0x9E:
                A = sub(A, fetch_memory(HL()), true);
                break;
            case 0x9F:
                A = sub(A, A, true);
                break;
            case 0xA0:
                A = bitwise_and(A, B);
                break;
            case 0xA1:
                A = bitwise_and(A, C);
                break;
            case 0xA2:
                A = bitwise_and(A, D);
                break;
            case 0xA3:
                A = bitwise_and(A, E);
                break;
            case 0xA4:
                A = bitwise_and(A, H);
                break;
            case 0xA5:
                A = bitwise_and(A, L);
                break;
            case 0xA6:
                A = bitwise_and(A, fetch_memory(HL()));
                break;
            case 0xA7:
                A = bitwise_and(A, A);
                break;
            case 0xA8:
                A = bitwise_xor(A, B);
                break;
            case 0xA9:
                A = bitwise_xor(A, C);
                break;
            case 0xAA:
                A = bitwise_xor(A, D);
                break;
            case 0xAB:
                A = bitwise_xor(A, E);
                break;
            case 0xAC:
                A = bitwise_xor(A, H);
                break;
            case 0xAD:
                A = bitwise_xor(A, L);
                break;
            case 0xAE:
                A = bitwise_xor(A, fetch_memory(HL()));
                break;
            case 0xAF:
                A = bitwise_xor(A, A);
                break;
            case 0xB0:
                A = bitwise_or(A, B);
                break;
            case 0xB1:
                A = bitwise_or(A, C);
                break;
            case 0xB2:
                A = bitwise_or(A, D);
                break;
            case 0xB3:
                A = bitwise_or(A, E);
                break;
            case 0xB4:
                A = bitwise_or(A, H);
                break;
            case 0xB5:
                A = bitwise_or(A, L);
                break;
            case 0xB6:
                A = bitwise_or(A, fetch_memory(HL()));
                break;
            case 0xB7:
                A = bitwise_or(A, A);
                break;
            case 0xB8:
                (void) sub(A, B);
                break;
            case 0xB9:
                (void) sub(A, C);
                break;
            case 0xBA:
                (void) sub(A, D);
                break;
            case 0xBB:
                (void) sub(A, E);
                break;
            case 0xBC:
                (void) sub(A, H);
                break;
            case 0xBD:
                (void) sub(A, L);
                break;
            case 0xBE:
                (void) sub(A, fetch_memory(HL()));
                break;
            case 0xBF:
                (void) sub(A, A);
                break;
            case 0xC0:
                ret_cc(Conditionals::NZ);
                break;
            case 0xC1:
                pop(B, C);
                break;
            case 0xC2:
                jp_cc(Conditionals::NZ);
                break;
            case 0xC3:
                jp();
                break;
            case 0xC4:
                call_cc(Conditionals::NZ);
                break;
            case 0xC5:
                push(B, C);
                break;
            case 0xC6:
                A = add(A, fetch_memory(PC++));
                break;
            case 0xC7:
                rst(ResetVector::h00);
                break;
            case 0xC8:
                ret_cc(Conditionals::Z);
                break;
            case 0xC9:
                ret();
                break;
            case 0xCA:
                jp_cc(Conditionals::Z);
                break;
            case 0xCB:
                fetch_decode_execute(true);
                break;
            case 0xCC:
                call_cc(Conditionals::Z);
                break;
            case 0xCD:
                call();
                break;
            case 0xCE:
                A = add(A, fetch_memory(PC++), true);
                break;
            case 0xCF:
                rst(ResetVector::h08);
                break;
            case 0xD0:
                ret_cc(Conditionals::NC);
                break;
            case 0xD1:
                pop(D, E);
                break;
            case 0xD2:
                jp_cc(Conditionals::NC);
                break;
            case 0xD4:
                call_cc(Conditionals::NC);
                break;
            case 0xD5:
                push(D, E);
                break;
            case 0xD6:
                A = sub(A, fetch_memory(PC++));
                break;
            case 0xD7:
                rst(ResetVector::h10);
                break;
            case 0xD8:
                ret_cc(Conditionals::C);
                break;
            case 0xD9:
                this->ime = true;
                ret();
                break;
            case 0xDA:
                jp_cc(Conditionals::C);
                break;
            case 0xDC:
                call_cc(Conditionals::C);
                break;
            case 0xDE:
                A = sub(A, fetch_memory(PC++), true);
                break;
            case 0xDF:
                rst(ResetVector::h18);
                break;
            case 0xE0:
                write_memory(0xFF00 | fetch_memory(PC++), A);
                break;
            case 0xE1:
                pop(H, L);
                break;
            case 0xE2:
                write_memory(0xFF00 | C, A);
                break;
            case 0xE5:
                push(H, L);
                break;
            case 0xE6:
                A = bitwise_and(A, fetch_memory(PC++));
                break;
            case 0xE7:
                rst(ResetVector::h20);
                break;
            case 0xE8:
                SP = add(SP, fetch_memory(PC++));
                break;
            case 0xE9:
                PC = HL();
                break;
            case 0xEA:
            {
                const auto lsb = fetch_memory(PC++);
                const auto msb = fetch_memory(PC++);
                write_memory(utils::to_word(msb, lsb), A);
            }
            break;
            case 0xEE:
                A = bitwise_xor(A, fetch_memory(PC++));
                break;
            case 0xEF:
                rst(ResetVector::h28);
                break;
            case 0xF0:
                A = fetch_memory(0xFF00 | fetch_memory(PC++));
                break;
            case 0xF1:
                pop(A, F);
                F &= 0xF0;
                break;
            case 0xF2:
                A = fetch_memory(0xFF00 | C);
                break;
            case 0xF3:
                this->ime         = false;
                this->request_ime = 0;
                break;
            case 0xF5:
                push(A, F);
                break;
            case 0xF6:
                A = bitwise_or(A, fetch_memory(PC++));
                break;
            case 0xF7:
                rst(ResetVector::h30);
                break;
            case 0xF8:
                HL(add(SP, fetch_memory(PC++)));
                break;
            case 0xF9:
                SP = HL();
                machine_cycle();
                break;
            case 0xFA:
            {
                const auto lsb{fetch_memory(PC++)};
                const auto msb{fetch_memory(PC++)};
                A = fetch_memory(utils::to_word(msb, lsb));
            }
            break;
            case 0xFB:
                this->request_ime = 2;
                break;
            case 0xFE:
                (void) sub(A, fetch_memory(PC++));
                break;
            case 0xFF:
                rst(ResetVector::h38);
                break;
            [[unlikely]] default:
                throw std::runtime_error(std::format("Illegal opcode: {:02X}", ir));
        };
    }
    else
    {
        switch (ir)
        {
            case 0x00:
                B = rotate_left(B, true);
                break;
            case 0x01:
                C = rotate_left(C, true);
                break;
            case 0x02:
                D = rotate_left(D, true);
                break;
            case 0x03:
                E = rotate_left(E, true);
                break;
            case 0x04:
                H = rotate_left(H, true);
                break;
            case 0x05:
                L = rotate_left(L, true);
                break;
            case 0x06:
                write_memory(HL(), rotate_left(fetch_memory(HL()), true));
                break;
            case 0x07:
                A = rotate_left(A, true);
                break;
            case 0x08:
                B = rotate_right(B, true);
                break;
            case 0x09:
                C = rotate_right(C, true);
                break;
            case 0x0A:
                D = rotate_right(D, true);
                break;
            case 0x0B:
                E = rotate_right(E, true);
                break;
            case 0x0C:
                H = rotate_right(H, true);
                break;
            case 0x0D:
                L = rotate_right(L, true);
                break;
            case 0x0E:
                write_memory(HL(), rotate_right(fetch_memory(HL()), true));
                break;
            case 0x0F:
                A = rotate_right(A, true);
                break;
            case 0x10:
                B = rotate_left(B);
                break;
            case 0x11:
                C = rotate_left(C);
                break;
            case 0x12:
                D = rotate_left(D);
                break;
            case 0x13:
                E = rotate_left(E);
                break;
            case 0x14:
                H = rotate_left(H);
                break;
            case 0x15:
                L = rotate_left(L);
                break;
            case 0x16:
                write_memory(HL(), rotate_left(fetch_memory(HL())));
                break;
            case 0x17:
                A = rotate_left(A);
                break;
            case 0x18:
                B = rotate_right(B);
                break;
            case 0x19:
                C = rotate_right(C);
                break;
            case 0x1A:
                D = rotate_right(D);
                break;
            case 0x1B:
                E = rotate_right(E);
                break;
            case 0x1C:
                H = rotate_right(H);
                break;
            case 0x1D:
                L = rotate_right(L);
                break;
            case 0x1E:
                write_memory(HL(), rotate_right(fetch_memory(HL())));
                break;
            case 0x1F:
                A = rotate_right(A);
                break;
            case 0x20:
                B = shift_left(B);
                break;
            case 0x21:
                C = shift_left(C);
                break;
            case 0x22:
                D = shift_left(D);
                break;
            case 0x23:
                E = shift_left(E);
                break;
            case 0x24:
                H = shift_left(H);
                break;
            case 0x25:
                L = shift_left(L);
                break;
            case 0x26:
                write_memory(HL(), shift_left(fetch_memory(HL())));
                break;
            case 0x27:
                A = shift_left(A);
                break;
            case 0x28:
                B = shift_right(B, true);
                break;
            case 0x29:
                C = shift_right(C, true);
                break;
            case 0x2A:
                D = shift_right(D, true);
                break;
            case 0x2B:
                E = shift_right(E, true);
                break;
            case 0x2C:
                H = shift_right(H, true);
                break;
            case 0x2D:
                L = shift_right(L, true);
                break;
            case 0x2E:
                write_memory(HL(), shift_right(fetch_memory(HL()), true));
                break;
            case 0x2F:
                A = shift_right(A, true);
                break;
            case 0x30:
                B = swap(B);
                break;
            case 0x31:
                C = swap(C);
                break;
            case 0x32:
                D = swap(D);
                break;
            case 0x33:
                E = swap(E);
                break;
            case 0x34:
                H = swap(H);
                break;
            case 0x35:
                L = swap(L);
                break;
            case 0x36:
                write_memory(HL(), swap(fetch_memory(HL())));
                break;
            case 0x37:
                A = swap(A);
                break;
            case 0x38:
                B = shift_right(B);
                break;
            case 0x39:
                C = shift_right(C);
                break;
            case 0x3A:
                D = shift_right(D);
                break;
            case 0x3B:
                E = shift_right(E);
                break;
            case 0x3C:
                H = shift_right(H);
                break;
            case 0x3D:
                L = shift_right(L);
                break;
            case 0x3E:
                write_memory(HL(), shift_right(fetch_memory(HL())));
                break;
            case 0x3F:
                A = shift_right(A);
                break;
            case 0x40:
                bit(B, 0);
                break;
            case 0x41:
                bit(C, 0);
                break;
            case 0x42:
                bit(D, 0);
                break;
            case 0x43:
                bit(E, 0);
                break;
            case 0x44:
                bit(H, 0);
                break;
            case 0x45:
                bit(L, 0);
                break;
            case 0x46:
                bit(fetch_memory(HL()), 0);
                break;
            case 0x47:
                bit(A, 0);
                break;
            case 0x48:
                bit(B, 1);
                break;
            case 0x49:
                bit(C, 1);
                break;
            case 0x4A:
                bit(D, 1);
                break;
            case 0x4B:
                bit(E, 1);
                break;
            case 0x4C:
                bit(H, 1);
                break;
            case 0x4D:
                bit(L, 1);
                break;
            case 0x4E:
                bit(fetch_memory(HL()), 1);
                break;
            case 0x4F:
                bit(A, 1);
                break;
            case 0x50:
                bit(B, 2);
                break;
            case 0x51:
                bit(C, 2);
                break;
            case 0x52:
                bit(D, 2);
                break;
            case 0x53:
                bit(E, 2);
                break;
            case 0x54:
                bit(H, 2);
                break;
            case 0x55:
                bit(L, 2);
                break;
            case 0x56:
                bit(fetch_memory(HL()), 2);
                break;
            case 0x57:
                bit(A, 2);
                break;
            case 0x58:
                bit(B, 3);
                break;
            case 0x59:
                bit(C, 3);
                break;
            case 0x5A:
                bit(D, 3);
                break;
            case 0x5B:
                bit(E, 3);
                break;
            case 0x5C:
                bit(H, 3);
                break;
            case 0x5D:
                bit(L, 3);
                break;
            case 0x5E:
                bit(fetch_memory(HL()), 3);
                break;
            case 0x5F:
                bit(A, 3);
                break;
            case 0x60:
                bit(B, 4);
                break;
            case 0x61:
                bit(C, 4);
                break;
            case 0x62:
                bit(D, 4);
                break;
            case 0x63:
                bit(E, 4);
                break;
            case 0x64:
                bit(H, 4);
                break;
            case 0x65:
                bit(L, 4);
                break;
            case 0x66:
                bit(fetch_memory(HL()), 4);
                break;
            case 0x67:
                bit(A, 4);
                break;
            case 0x68:
                bit(B, 5);
                break;
            case 0x69:
                bit(C, 5);
                break;
            case 0x6A:
                bit(D, 5);
                break;
            case 0x6B:
                bit(E, 5);
                break;
            case 0x6C:
                bit(H, 5);
                break;
            case 0x6D:
                bit(L, 5);
                break;
            case 0x6E:
                bit(fetch_memory(HL()), 5);
                break;
            case 0x6F:
                bit(A, 5);
                break;
            case 0x70:
                bit(B, 6);
                break;
            case 0x71:
                bit(C, 6);
                break;
            case 0x72:
                bit(D, 6);
                break;
            case 0x73:
                bit(E, 6);
                break;
            case 0x74:
                bit(H, 6);
                break;
            case 0x75:
                bit(L, 6);
                break;
            case 0x76:
                bit(fetch_memory(HL()), 6);
                break;
            case 0x77:
                bit(A, 6);
                break;
            case 0x78:
                bit(B, 7);
                break;
            case 0x79:
                bit(C, 7);
                break;
            case 0x7A:
                bit(D, 7);
                break;
            case 0x7B:
                bit(E, 7);
                break;
            case 0x7C:
                bit(H, 7);
                break;
            case 0x7D:
                bit(L, 7);
                break;
            case 0x7E:
                bit(fetch_memory(HL()), 7);
                break;
            case 0x7F:
                bit(A, 7);
                break;

            case 0x80:
                B = res(B, 0);
                break;
            case 0x81:
                C = res(C, 0);
                break;
            case 0x82:
                D = res(D, 0);
                break;
            case 0x83:
                E = res(E, 0);
                break;
            case 0x84:
                H = res(H, 0);
                break;
            case 0x85:
                L = res(L, 0);
                break;
            case 0x86:
                write_memory(HL(), res(fetch_memory(HL()), 0));
                break;
            case 0x87:
                A = res(A, 0);
                break;
            case 0x88:
                B = res(B, 1);
                break;
            case 0x89:
                C = res(C, 1);
                break;
            case 0x8A:
                D = res(D, 1);
                break;
            case 0x8B:
                E = res(E, 1);
                break;
            case 0x8C:
                H = res(H, 1);
                break;
            case 0x8D:
                L = res(L, 1);
                break;
            case 0x8E:
                write_memory(HL(), res(fetch_memory(HL()), 1));
                break;
            case 0x8F:
                A = res(A, 1);
                break;
            case 0x90:
                B = res(B, 2);
                break;
            case 0x91:
                C = res(C, 2);
                break;
            case 0x92:
                D = res(D, 2);
                break;
            case 0x93:
                E = res(E, 2);
                break;
            case 0x94:
                H = res(H, 2);
                break;
            case 0x95:
                L = res(L, 2);
                break;
            case 0x96:
                write_memory(HL(), res(fetch_memory(HL()), 2));
                break;
            case 0x97:
                A = res(A, 2);
                break;
            case 0x98:
                B = res(B, 3);
                break;
            case 0x99:
                C = res(C, 3);
                break;
            case 0x9A:
                D = res(D, 3);
                break;
            case 0x9B:
                E = res(E, 3);
                break;
            case 0x9C:
                H = res(H, 3);
                break;
            case 0x9D:
                L = res(L, 3);
                break;
            case 0x9E:
                write_memory(HL(), res(fetch_memory(HL()), 3));
                break;
            case 0x9F:
                A = res(A, 3);
                break;

            case 0xA0:
                B = res(B, 4);
                break;
            case 0xA1:
                C = res(C, 4);
                break;
            case 0xA2:
                D = res(D, 4);
                break;
            case 0xA3:
                E = res(E, 4);
                break;
            case 0xA4:
                H = res(H, 4);
                break;
            case 0xA5:
                L = res(L, 4);
                break;
            case 0xA6:
                write_memory(HL(), res(fetch_memory(HL()), 4));
                break;
            case 0xA7:
                A = res(A, 4);
                break;
            case 0xA8:
                B = res(B, 5);
                break;
            case 0xA9:
                C = res(C, 5);
                break;
            case 0xAA:
                D = res(D, 5);
                break;
            case 0xAB:
                E = res(E, 5);
                break;
            case 0xAC:
                H = res(H, 5);
                break;
            case 0xAD:
                L = res(L, 5);
                break;
            case 0xAE:
                write_memory(HL(), res(fetch_memory(HL()), 5));
                break;
            case 0xAF:
                A = res(A, 5);
                break;

            case 0xB0:
                B = res(B, 6);
                break;
            case 0xB1:
                C = res(C, 6);
                break;
            case 0xB2:
                D = res(D, 6);
                break;
            case 0xB3:
                E = res(E, 6);
                break;
            case 0xB4:
                H = res(H, 6);
                break;
            case 0xB5:
                L = res(L, 6);
                break;
            case 0xB6:
                write_memory(HL(), res(fetch_memory(HL()), 6));
                break;
            case 0xB7:
                A = res(A, 6);
                break;
            case 0xB8:
                B = res(B, 7);
                break;
            case 0xB9:
                C = res(C, 7);
                break;
            case 0xBA:
                D = res(D, 7);
                break;
            case 0xBB:
                E = res(E, 7);
                break;
            case 0xBC:
                H = res(H, 7);
                break;
            case 0xBD:
                L = res(L, 7);
                break;
            case 0xBE:
                write_memory(HL(), res(fetch_memory(HL()), 7));
                break;
            case 0xBF:
                A = res(A, 7);
                break;
            case 0xC0:
                B = set(B, 0);
                break;
            case 0xC1:
                C = set(C, 0);
                break;
            case 0xC2:
                D = set(D, 0);
                break;
            case 0xC3:
                E = set(E, 0);
                break;
            case 0xC4:
                H = set(H, 0);
                break;
            case 0xC5:
                L = set(L, 0);
                break;
            case 0xC6:
                write_memory(HL(), set(fetch_memory(HL()), 0));
                break;
            case 0xC7:
                A = set(A, 0);
                break;
            case 0xC8:
                B = set(B, 1);
                break;
            case 0xC9:
                C = set(C, 1);
                break;
            case 0xCA:
                D = set(D, 1);
                break;
            case 0xCB:
                E = set(E, 1);
                break;
            case 0xCC:
                H = set(H, 1);
                break;
            case 0xCD:
                L = set(L, 1);
                break;
            case 0xCE:
                write_memory(HL(), set(fetch_memory(HL()), 1));
                break;
            case 0xCF:
                A = set(A, 1);
                break;

            case 0xD0:
                B = set(B, 2);
                break;
            case 0xD1:
                C = set(C, 2);
                break;
            case 0xD2:
                D = set(D, 2);
                break;
            case 0xD3:
                E = set(E, 2);
                break;
            case 0xD4:
                H = set(H, 2);
                break;
            case 0xD5:
                L = set(L, 2);
                break;
            case 0xD6:
                write_memory(HL(), set(fetch_memory(HL()), 2));
                break;
            case 0xD7:
                A = set(A, 2);
                break;
            case 0xD8:
                B = set(B, 3);
                break;
            case 0xD9:
                C = set(C, 3);
                break;
            case 0xDA:
                D = set(D, 3);
                break;
            case 0xDB:
                E = set(E, 3);
                break;
            case 0xDC:
                H = set(H, 3);
                break;
            case 0xDD:
                L = set(L, 3);
                break;
            case 0xDE:
                write_memory(HL(), set(fetch_memory(HL()), 3));
                break;
            case 0xDF:
                A = set(A, 3);
                break;
            case 0xE0:
                B = set(B, 4);
                break;
            case 0xE1:
                C = set(C, 4);
                break;
            case 0xE2:
                D = set(D, 4);
                break;
            case 0xE3:
                E = set(E, 4);
                break;
            case 0xE4:
                H = set(H, 4);
                break;
            case 0xE5:
                L = set(L, 4);
                break;
            case 0xE6:
                write_memory(HL(), set(fetch_memory(HL()), 4));
                break;
            case 0xE7:
                A = set(A, 4);
                break;
            case 0xE8:
                B = set(B, 5);
                break;
            case 0xE9:
                C = set(C, 5);
                break;
            case 0xEA:
                D = set(D, 5);
                break;
            case 0xEB:
                E = set(E, 5);
                break;
            case 0xEC:
                H = set(H, 5);
                break;
            case 0xED:
                L = set(L, 5);
                break;
            case 0xEE:
                write_memory(HL(), set(fetch_memory(HL()), 5));
                break;
            case 0xEF:
                A = set(A, 5);
                break;
            case 0xF0:
                B = set(B, 6);
                break;
            case 0xF1:
                C = set(C, 6);
                break;
            case 0xF2:
                D = set(D, 6);
                break;
            case 0xF3:
                E = set(E, 6);
                break;
            case 0xF4:
                H = set(H, 6);
                break;
            case 0xF5:
                L = set(L, 6);
                break;
            case 0xF6:
                write_memory(HL(), set(fetch_memory(HL()), 6));
                break;
            case 0xF7:
                A = set(A, 6);
                break;
            case 0xF8:
                B = set(B, 7);
                break;
            case 0xF9:
                C = set(C, 7);
                break;
            case 0xFA:
                D = set(D, 7);
                break;
            case 0xFB:
                E = set(E, 7);
                break;
            case 0xFC:
                H = set(H, 7);
                break;
            case 0xFD:
                L = set(L, 7);
                break;
            case 0xFE:
                write_memory(HL(), set(fetch_memory(HL()), 7));
                break;
            case 0xFF:
                A = set(A, 7);
                break;
            [[unlikely]] default:
                throw std::runtime_error(std::format("Opcode not implemented {:02X}", ir));
        };
    }
}