#include <filesystem>
#include <fmt/core.h>
#include <fstream>
#include <iostream>
#include <vector>

#include "opcode.h"

std::vector<uint8_t> load_rom(std::filesystem::path rom_path);

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage : chip-8-disassembler rom_file\n";
        exit(EXIT_FAILURE);
    }
    std::vector<uint8_t> program = load_rom(std::filesystem::path(argv[1]));
    std::cout << "Program size : " << program.size() << "\n";
    std::cout << "Number of instructions : " << program.size() / 2 << "\n";
    uint16_t addr = 0x200;
    for (std::size_t i = 0; i < program.size() && i + 1 < program.size(); i += 2)
    {
        uint16_t opcode = program[i] << 8 | program[i + 1];
        opcode_info info = opcode_info::decode(opcode);
        std::cout << fmt::format("{:#06x}     {}\n", addr, info.string_repr());
        addr += 2;
    }

    return 0;
}

std::vector<uint8_t> load_rom(std::filesystem::path rom_path)
{
    std::ifstream rom_file(rom_path.string(), std::ios::binary);
    std::streampos rom_size;

    rom_file.seekg(0, std::ios::end);
    rom_size = rom_file.tellg();
    rom_file.seekg(0, std::ios::beg);

    std::vector<uint8_t> rom_content(rom_size);
    rom_file.read(reinterpret_cast<char*>(rom_content.data()), rom_size);

    return rom_content;
}
