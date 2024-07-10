#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#include "chip8.h"
#include "chip8_test.h"
#include "opcode.h"

TEST_CASE("movi 0x6XNN")
{
    SUBCASE("Decode")
    {
        Opcode info = Opcode::decode(0x61, 0x34);
        CHECK(info.id == opcode_id::MOVI);
        CHECK(info.X == 0x1);
        CHECK(info.NN == 0x34);
    }

    SUBCASE("Empty register")
    {
        Chip8 cpu = Chip8(std::vector<uint8_t>());
        CHECK(cpu.get_registers()[0x3] == 0);
        chip8_test::movi(cpu, 0x3, 0xFF);
        CHECK(cpu.get_registers()[0x3] == 0xFF);
    }

    SUBCASE("Overwrite register")
    {
        Chip8 cpu = Chip8(std::vector<uint8_t>());
        CHECK(cpu.get_registers()[0xD] == 0);
        chip8_test::movi(cpu, 0xD, 0xFF);
        CHECK(cpu.get_registers()[0xD] == 0xFF);
        chip8_test::movi(cpu, 0xD, 0x45);
        CHECK(cpu.get_registers()[0xD] == 0x45);
    }
}
