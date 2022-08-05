#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#include "chip8.h"
#include "chip8_test.h"
#include "opcode.h"

TEST_CASE("addi 0x7XNN")
{
    SUBCASE("Decode")
    {
        opcode_info info = opcode_info::decode(0x71, 0x34);
        CHECK(info.id == opcode_id::ADDI);
        CHECK(info.X == 0x1);
        CHECK(info.NN == 0x34);
    }

    SUBCASE("Simple execution")
    {
        chip8 cpu = chip8(nullptr, 0);
        CHECK(cpu.get_registers()[0x3] == 0);
        chip8_test::addi(cpu, 0x3, 0xAB);
        CHECK(cpu.get_registers()[0x3] == 0xAB);
    }

    SUBCASE("Overflow (VF must not change)")
    {
        chip8 cpu = chip8(nullptr, 0);
        CHECK(cpu.get_registers()[0xD] == 0);
        chip8_test::movi(cpu, 0xD, 0xFF);
        CHECK(cpu.get_registers()[0xD] == 0xFF);
        uint8_t initial_vf = cpu.get_registers()[0xF];
        chip8_test::addi(cpu, 0xD, 0x10);
        CHECK(cpu.get_registers()[0xF] == initial_vf);
    }
}
