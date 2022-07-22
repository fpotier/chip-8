#pragma once

#include <cstdint>
#include <string>

enum class opcode_id: uint8_t;

struct opcode_info
{
    opcode_id id;
    uint16_t  raw_opcode;

    uint8_t  X;
    uint8_t  Y;
    uint8_t  N;
    uint8_t  NN;
    uint16_t NNN;

    static opcode_info decode(uint16_t opcode);
    static opcode_info decode(uint8_t opcode_h, uint8_t opcode_l);
    std::string string_repr();
};


enum class opcode_id: uint8_t
{
    UNKNOWN_OPCODE = 0,
    CLEAR =  1,
    RET   =  2,
    JMP   =  3,
    CALL  =  4,
    SKIEQ =  5,
    SKINE =  6,
    SKREQ =  7,
    MOVI  =  8,
    ADDI  =  9,
    MOV   = 10,
    OR    = 11,
    AND   = 12,
    XOR   = 13,
    ADD   = 14,
    SUB   = 15,
    SRL   = 16,
    LSUB  = 17,
    SLL   = 18,
    SKRNE = 19,
    MOVRI = 20,
    JMPV0 = 21,
    RAND  = 22,
    DRAW  = 23,
    SKKP  = 24,
    SKKNP = 25,
    LOADD = 26,
    WKEY  = 27,
    MOVD  = 28,
    MOVS  = 29,
    ADDRI = 30,
    SPRI  = 31,
    BCD   = 32,
    STORE = 33,
    LOAD  = 34,
};

static std::string opcode_str[] = {
    "UNKNOWN OPCODE",
    "clear",
    "ret",
    "jmp",
    "call",
    "skieq",
    "skine",
    "skreq",
    "movi",
    "addi",
    "mov",
    "or",
    "and",
    "xor",
    "add",
    "sub",
    "srl",
    "lsub",
    "sll",
    "skrne",
    "movri",
    "jmpv0",
    "rand",
    "draw",
    "skkp",
    "skknp",
    "loadd",
    "wkey",
    "movd",
    "movs",
    "addri",
    "spri",
    "bcd",
    "store",
    "load",
};
