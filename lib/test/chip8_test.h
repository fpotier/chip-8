#pragma once

#include "chip8.h"

class chip8_test
{
public:
    static void write_vram(chip8& cpu, size_t x, size_t y, bool value);

    static void clear(chip8& cpu);
    static void ret(chip8& cpu);
    static void jmp(chip8& cpu, uint16_t NNN);
    static void call(chip8& cpu, uint16_t NNN);
    static void skieq(chip8& cpu, uint8_t X, uint8_t NN);
    static void skine(chip8& cpu, uint8_t X, uint8_t NN);
    static void skreq(chip8& cpu, uint8_t X, uint8_t Y);
    static void movi(chip8& cpu, uint8_t X, uint8_t NN);
    static void addi(chip8& cpu, uint8_t X, uint8_t NN);
    static void mov(chip8& cpu, uint8_t X, uint8_t Y);
    static void _or(chip8& cpu, uint8_t X, uint8_t Y);
    static void _and(chip8& cpu, uint8_t X, uint8_t Y);
    static void _xor(chip8& cpu, uint8_t X, uint8_t Y);
    static void add(chip8& cpu, uint8_t X, uint8_t Y);
    static void sub(chip8& cpu, uint8_t X, uint8_t Y);
    static void srl(chip8& cpu, uint8_t X, uint8_t Y);
    static void lsub(chip8& cpu, uint8_t X, uint8_t Y);
    static void sll(chip8& cpu, uint8_t X, uint8_t Y);
    static void skrne(chip8& cpu, uint8_t X, uint8_t Y);
    static void movri(chip8& cpu, uint16_t NNN);
    static void jmpv0(chip8& cpu, uint16_t NNN, uint8_t X);
    static void rand(chip8& cpu, uint8_t X, uint8_t NN);
    static void draw(chip8& cpu, uint8_t X, uint8_t Y, uint8_t N);
    static void skkp(chip8& cpu, uint8_t X);
    static void skknp(chip8& cpu, uint8_t X);
    static void loadd(chip8& cpu, uint8_t X);
    static void wkey(chip8& cpu, uint8_t X);
    static void movd(chip8& cpu, uint8_t X);
    static void movs(chip8& cpu, uint8_t X);
    static void addri(chip8& cpu, uint8_t X);
    static void spri(chip8& cpu, uint8_t X);
    static void bcd(chip8& cpu, uint8_t X);
    static void store(chip8& cpu, uint8_t X);
    static void load(chip8& cpu, uint8_t X);
};
