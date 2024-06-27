#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
#include <vector>

#include "chip8.h"
#include "chip8_test.h"
#include "opcode.h"

TEST_CASE("clear 0x00E0")
{
    std::vector<uint8_t> program = { 0x00, 0xE0 };
    Chip8 cpu = Chip8(program);
    SUBCASE("Decode")
    {
        Opcode info = Opcode::decode(program[0], program[1]);
        CHECK(info.id == opcode_id::CLEAR);
    }

    SUBCASE("Clean vram")
    {
        bool pixel_on = false;
        for (uint8_t pixel : cpu.get_vram())
            pixel_on = pixel_on || pixel;
        chip8_test::clear(cpu);
        for (uint8_t pixel : cpu.get_vram())
            pixel_on = pixel_on || pixel;
        CHECK(pixel_on == false);
    }

    SUBCASE("Dirty vram")
    {
        chip8_test::write_vram(cpu, 2, 3, true);
        bool pixel_on = false;
        for (uint8_t pixel : cpu.get_vram())
            pixel_on = pixel_on || pixel;
        CHECK(pixel_on == true);
        pixel_on = false;
        chip8_test::clear(cpu);
        for (uint8_t pixel : cpu.get_vram())
            pixel_on = pixel_on || pixel;
        CHECK(pixel_on == false);
    }
}
