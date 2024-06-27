#include "chip8_test.h"

void chip8_test::write_vram(Chip8& cpu, size_t x, size_t y, bool value)
{
    cpu.vram[Chip8::SCREEN_WIDTH * x + y] = value;
}

void chip8_test::clear(Chip8& cpu)
{
    cpu.clear();
}

void chip8_test::ret(Chip8& cpu)
{
    cpu.ret();
}

void chip8_test::jmp(Chip8& cpu, uint16_t NNN)
{
    cpu.jmp(NNN);
}

void chip8_test::call(Chip8& cpu, uint16_t NNN)
{
    cpu.call(NNN);
}

void chip8_test::skieq(Chip8& cpu, uint8_t X, uint8_t NN)
{
    cpu.skieq(X, NN);
}

void chip8_test::skine(Chip8& cpu, uint8_t X, uint8_t NN)
{
    cpu.skine(X, NN);
}

void chip8_test::skreq(Chip8& cpu, uint8_t X, uint8_t Y)
{
    cpu.skreq(X, Y);
}

void chip8_test::movi(Chip8& cpu, uint8_t X, uint8_t NN)
{
    cpu.movi(X, NN);
}

void chip8_test::addi(Chip8& cpu, uint8_t X, uint8_t NN)
{
    cpu.addi(X, NN);
}

void chip8_test::mov(Chip8& cpu, uint8_t X, uint8_t Y)
{
    cpu.mov(X, Y);
}

void chip8_test::_or(Chip8& cpu, uint8_t X, uint8_t Y)
{
    cpu._or(X, Y);
}

void chip8_test::_and(Chip8& cpu, uint8_t X, uint8_t Y)
{
    cpu._and(X, Y);
}

void chip8_test::_xor(Chip8& cpu, uint8_t X, uint8_t Y)
{
    cpu._xor(X, Y);
}

void chip8_test::add(Chip8& cpu, uint8_t X, uint8_t Y)
{
    cpu.add(X, Y);
}

void chip8_test::sub(Chip8& cpu, uint8_t X, uint8_t Y)
{
    cpu.sub(X, Y);
}

void chip8_test::srl(Chip8& cpu, uint8_t X, uint8_t Y)
{
    cpu.srl(X, Y);
}

void chip8_test::lsub(Chip8& cpu, uint8_t X, uint8_t Y)
{
    cpu.lsub(X, Y);
}

void chip8_test::sll(Chip8& cpu, uint8_t X, uint8_t Y)
{
    cpu.sll(X, Y);
}

void chip8_test::skrne(Chip8& cpu, uint8_t X, uint8_t Y)
{
    cpu.skrne(X, Y);
}

void chip8_test::movri(Chip8& cpu, uint16_t NNN)
{
    cpu.movri(NNN);
}

void chip8_test::jmpv0(Chip8& cpu, uint16_t NNN, uint8_t X)
{
    cpu.jmpv0(NNN, X);
}

void chip8_test::rand(Chip8& cpu, uint8_t X, uint8_t NN)
{
    cpu.rand(X, NN);
}

void chip8_test::draw(Chip8& cpu, uint8_t X, uint8_t Y, uint8_t N)
{
    cpu.draw(X, Y, N);
}

void chip8_test::skkp(Chip8& cpu, uint8_t X)
{
    cpu.skkp(X);
}

void chip8_test::skknp(Chip8& cpu, uint8_t X)
{
    cpu.skknp(X);
}

void chip8_test::loadd(Chip8& cpu, uint8_t X)
{
    cpu.loadd(X);
}

void chip8_test::wkey(Chip8& cpu, uint8_t X)
{
    cpu.wkey(X);
}

void chip8_test::movd(Chip8& cpu, uint8_t X)
{
    cpu.movd(X);
}

void chip8_test::movs(Chip8& cpu, uint8_t X)
{
    cpu.movs(X);
}

void chip8_test::addri(Chip8& cpu, uint8_t X)
{
    cpu.addri(X);
}

void chip8_test::spri(Chip8& cpu, uint8_t X)
{
    cpu.spri(X);
}

void chip8_test::bcd(Chip8& cpu, uint8_t X)
{
    cpu.bcd(X);
}

void chip8_test::store(Chip8& cpu, uint8_t X)
{
    cpu.store(X);
}

void chip8_test::load(Chip8& cpu, uint8_t X)
{
    cpu.load(X);
}
