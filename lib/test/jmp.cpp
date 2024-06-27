#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#include "chip8.h"
#include "chip8_test.h"
#include "opcode.h"

TEST_CASE("jmp 0x1NNN")
{
    SUBCASE("Decode")
    {
        Opcode info = Opcode::decode(0x12, 0x34);
        CHECK(info.id == opcode_id::JMP);
    }
}
