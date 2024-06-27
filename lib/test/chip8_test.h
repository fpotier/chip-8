#pragma once

#include "chip8.h"

class chip8_test
{
public:
    static void write_vram(Chip8& cpu, size_t x, size_t y, bool value);

    static void clear(Chip8& cpu);
    static void ret(Chip8& cpu);
    static void jmp(Chip8& cpu, uint16_t NNN);
    static void call(Chip8& cpu, uint16_t NNN);
    static void skieq(Chip8& cpu, uint8_t X, uint8_t NN);
    static void skine(Chip8& cpu, uint8_t X, uint8_t NN);
    static void skreq(Chip8& cpu, uint8_t X, uint8_t Y);
    static void movi(Chip8& cpu, uint8_t X, uint8_t NN);
    static void addi(Chip8& cpu, uint8_t X, uint8_t NN);
    static void mov(Chip8& cpu, uint8_t X, uint8_t Y);
    static void _or(Chip8& cpu, uint8_t X, uint8_t Y);
    static void _and(Chip8& cpu, uint8_t X, uint8_t Y);
    static void _xor(Chip8& cpu, uint8_t X, uint8_t Y);
    static void add(Chip8& cpu, uint8_t X, uint8_t Y);
    static void sub(Chip8& cpu, uint8_t X, uint8_t Y);
    static void srl(Chip8& cpu, uint8_t X, uint8_t Y);
    static void lsub(Chip8& cpu, uint8_t X, uint8_t Y);
    static void sll(Chip8& cpu, uint8_t X, uint8_t Y);
    static void skrne(Chip8& cpu, uint8_t X, uint8_t Y);
    static void movri(Chip8& cpu, uint16_t NNN);
    static void jmpv0(Chip8& cpu, uint16_t NNN, uint8_t X);
    static void rand(Chip8& cpu, uint8_t X, uint8_t NN);
    static void draw(Chip8& cpu, uint8_t X, uint8_t Y, uint8_t N);
    static void skkp(Chip8& cpu, uint8_t X);
    static void skknp(Chip8& cpu, uint8_t X);
    static void loadd(Chip8& cpu, uint8_t X);
    static void wkey(Chip8& cpu, uint8_t X);
    static void movd(Chip8& cpu, uint8_t X);
    static void movs(Chip8& cpu, uint8_t X);
    static void addri(Chip8& cpu, uint8_t X);
    static void spri(Chip8& cpu, uint8_t X);
    static void bcd(Chip8& cpu, uint8_t X);
    static void store(Chip8& cpu, uint8_t X);
    static void load(Chip8& cpu, uint8_t X);
};
