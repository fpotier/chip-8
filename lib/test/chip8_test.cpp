#include "chip8_test.h"

void chip8_test::write_vram(chip8& cpu, size_t x, size_t y, bool value)
{
    cpu.vram[chip8::screen_width * x + y] = value;
}

void chip8_test::clear(chip8& cpu)
{
    cpu.clear();
}

void chip8_test::ret(chip8& cpu)
{
    cpu.ret();
}

void chip8_test::jmp(chip8& cpu, uint16_t NNN)
{
    cpu.jmp(NNN);
}

void chip8_test::call(chip8& cpu, uint16_t NNN)
{
    cpu.call(NNN);
}

void chip8_test::skieq(chip8& cpu, uint8_t X, uint8_t NN)
{
    cpu.skieq(X, NN);
}

void chip8_test::skine(chip8& cpu, uint8_t X, uint8_t NN)
{
    cpu.skine(X, NN);
}

void chip8_test::skreq(chip8& cpu, uint8_t X, uint8_t Y)
{
    cpu.skreq(X, Y);
}

void chip8_test::movi(chip8& cpu, uint8_t X, uint8_t NN)
{
    cpu.movi(X, NN);
}

void chip8_test::addi(chip8& cpu, uint8_t X, uint8_t NN)
{
    cpu.addi(X, NN);
}

void chip8_test::mov(chip8& cpu, uint8_t X, uint8_t Y)
{
    cpu.mov(X, Y);
}

void chip8_test::_or(chip8& cpu, uint8_t X, uint8_t Y)
{
    cpu._or(X, Y);
}

void chip8_test::_and(chip8& cpu, uint8_t X, uint8_t Y)
{
    cpu._and(X, Y);
}

void chip8_test::_xor(chip8& cpu, uint8_t X, uint8_t Y)
{
    cpu._xor(X, Y);
}

void chip8_test::add(chip8& cpu, uint8_t X, uint8_t Y)
{
    cpu.add(X, Y);
}

void chip8_test::sub(chip8& cpu, uint8_t X, uint8_t Y)
{
    cpu.sub(X, Y);
}

void chip8_test::srl(chip8& cpu, uint8_t X)
{
    cpu.srl(X);
}

void chip8_test::lsub(chip8& cpu, uint8_t X, uint8_t Y)
{
    cpu.lsub(X, Y);
}

void chip8_test::sll(chip8& cpu, uint8_t X)
{
    cpu.sll(X);
}

void chip8_test::skrne(chip8& cpu, uint8_t X, uint8_t Y)
{
    cpu.skrne(X, Y);
}

void chip8_test::movri(chip8& cpu, uint16_t NNN)
{
    cpu.movri(NNN);
}

void chip8_test::jmpv0(chip8& cpu, uint16_t NNN)
{
    cpu.jmpv0(NNN);
}

void chip8_test::rand(chip8& cpu, uint8_t X, uint8_t NN)
{
    cpu.rand(X, NN);
}

void chip8_test::draw(chip8& cpu, uint8_t X, uint8_t Y, uint8_t N)
{
    cpu.draw(X, Y, N);
}

void chip8_test::skkp(chip8& cpu, uint8_t X)
{
    cpu.skkp(X);
}

void chip8_test::skknp(chip8& cpu, uint8_t X)
{
    cpu.skknp(X);
}

void chip8_test::loadd(chip8& cpu, uint8_t X)
{
    cpu.loadd(X);
}

void chip8_test::wkey(chip8& cpu, uint8_t X)
{
    cpu.wkey(X);
}

void chip8_test::movd(chip8& cpu, uint8_t X)
{
    cpu.movd(X);
}

void chip8_test::movs(chip8& cpu, uint8_t X)
{
    cpu.movs(X);
}

void chip8_test::addri(chip8& cpu, uint8_t X)
{
    cpu.addri(X);
}

void chip8_test::spri(chip8& cpu, uint8_t X)
{
    cpu.spri(X);
}

void chip8_test::bcd(chip8& cpu, uint8_t X)
{
    cpu.bcd(X);
}

void chip8_test::store(chip8& cpu, uint8_t X)
{
    cpu.store(X);
}

void chip8_test::load(chip8& cpu, uint8_t X)
{
    cpu.load(X);
}
