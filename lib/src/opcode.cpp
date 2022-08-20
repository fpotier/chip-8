#include <cassert>
#include <fmt/core.h>
#include <string>

#include "opcode.h"

std::string opcode::string_repr()
{
    std::string instruction = opcode_str[static_cast<uint8_t>(id)];
    switch (id)
    {
        case opcode_id::CLEAR:
        case opcode_id::RET:
            return fmt::format("{}", instruction);
        case opcode_id::JMP:
        case opcode_id::CALL:
        case opcode_id::MOVRI:
        case opcode_id::JMPV0:
            return fmt::format("{} {:#5x}", instruction, NNN);
        case opcode_id::SKIEQ:
        case opcode_id::SKINE:
        case opcode_id::MOVI:
        case opcode_id::ADDI:
        case opcode_id::RAND:
            return fmt::format("{} V{}, {:#4x}", instruction, X, NN);
        case opcode_id::SKREQ:
        case opcode_id::MOV:
        case opcode_id::OR:
        case opcode_id::AND:
        case opcode_id::XOR:
        case opcode_id::ADD:
        case opcode_id::SUB:
        case opcode_id::LSUB:
        case opcode_id::SKRNE:
            return fmt::format("{} V{}, V{}", instruction, X, Y);
        case opcode_id::SRL:
        case opcode_id::SLL:
        case opcode_id::SKKP:
        case opcode_id::SKKNP:
        case opcode_id::LOADD:
        case opcode_id::WKEY:
        case opcode_id::MOVD:
        case opcode_id::MOVS:
        case opcode_id::ADDRI:
        case opcode_id::SPRI:
        case opcode_id::BCD:
        case opcode_id::STORE:
        case opcode_id::LOAD:
            return fmt::format("{} V{}", instruction, X);
        case opcode_id::DRAW:
            return fmt::format("{} V{}, V{}, {:#3x}", instruction, X, Y, N);
        case opcode_id::UNKNOWN_OPCODE:
            return fmt::format("{} {:#06x}", instruction, raw_opcode);
    }
    assert(false);
    return "";
}

opcode opcode::decode(uint16_t raw_opcode)
{
    opcode op_info;
    op_info.raw_opcode = raw_opcode;
    op_info.X = (raw_opcode >> 8) & 0x0F;
    op_info.Y = (raw_opcode >> 4) & 0x0F;
    op_info.NNN = raw_opcode & 0x0FFF;
    op_info.NN = raw_opcode & 0x00FF;
    op_info.N = raw_opcode & 0x000F;

    switch (raw_opcode & 0xF000)
    {
        case 0x0000:
            switch (raw_opcode & 0x00FF)
            {
                case 0x00E0: op_info.id = opcode_id::CLEAR; break;
                case 0x00EE: op_info.id = opcode_id::RET;   break;
                default: op_info.id = opcode_id::UNKNOWN_OPCODE;
            }
            break;
        case 0x1000: op_info.id = opcode_id::JMP;   break;
        case 0x2000: op_info.id = opcode_id::CALL;  break;
        case 0x3000: op_info.id = opcode_id::SKIEQ; break;
        case 0x4000: op_info.id = opcode_id::SKINE; break;
        case 0x5000: op_info.id = opcode_id::SKREQ; break;
        case 0x6000: op_info.id = opcode_id::MOVI;  break;
        case 0x7000: op_info.id = opcode_id::ADDI;  break;
        case 0x8000:
            switch (raw_opcode & 0x800F)
            {
                case 0x8000: op_info.id = opcode_id::MOV;  break;
                case 0x8001: op_info.id = opcode_id::OR;   break;
                case 0x8002: op_info.id = opcode_id::AND;  break;
                case 0x8003: op_info.id = opcode_id::XOR;  break;
                case 0x8004: op_info.id = opcode_id::ADD;  break;
                case 0x8005: op_info.id = opcode_id::SUB;  break;
                case 0x8006: op_info.id = opcode_id::SRL;  break;
                case 0x8007: op_info.id = opcode_id::LSUB; break;
                case 0x800E: op_info.id = opcode_id::SLL;  break;
                default: op_info.id = opcode_id::UNKNOWN_OPCODE;
            }
            break;
        case 0x9000: op_info.id = opcode_id::SKRNE; break;
        case 0xA000: op_info.id = opcode_id::MOVRI; break;
        case 0xB000: op_info.id = opcode_id::JMPV0; break;
        case 0xC000: op_info.id = opcode_id::RAND;  break;
        case 0xD000: op_info.id = opcode_id::DRAW;  break;
        case 0xE000:
            switch (raw_opcode & 0x00FF)
            {
                case 0x9E: op_info.id = opcode_id::SKKP;  break;
                case 0xA1: op_info.id = opcode_id::SKKNP; break;
                default: op_info.id = opcode_id::UNKNOWN_OPCODE;
            }
            break;
        case 0xF000:
            switch (raw_opcode & 0x00FF)
            {
                case 0x07: op_info.id = opcode_id::LOADD; break;
                case 0x0A: op_info.id = opcode_id::WKEY;  break;
                case 0x15: op_info.id = opcode_id::MOVD;  break;
                case 0x18: op_info.id = opcode_id::MOVS;  break;
                case 0x1E: op_info.id = opcode_id::ADDRI; break;
                case 0x29: op_info.id = opcode_id::SPRI;  break;
                case 0x33: op_info.id = opcode_id::BCD;   break;
                case 0x55: op_info.id = opcode_id::STORE; break;
                case 0x65: op_info.id = opcode_id::LOAD;  break;
                default: op_info.id = opcode_id::UNKNOWN_OPCODE;
            }
            break;
            default: op_info.id = opcode_id::UNKNOWN_OPCODE;
    }

    return op_info;
}

opcode opcode::decode(uint8_t raw_opcode_h, uint8_t raw_opcode_l)
{
    return opcode::decode(raw_opcode_h << 8 | raw_opcode_l);
}
