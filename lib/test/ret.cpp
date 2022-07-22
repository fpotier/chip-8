#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#include "chip8.h"
#include "chip8_test.h"
#include "opcode.h"

TEST_CASE("ret 0x00EE")
{
    SUBCASE("Decode")
    {
        opcode_info info = opcode_info::decode(0x00, 0xEE);
        CHECK(info.id == opcode_id::RET);
    }
}
