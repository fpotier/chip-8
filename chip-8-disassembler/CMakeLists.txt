project("Chip-8 Disassembler" VERSION 0.1 LANGUAGES CXX)

add_executable(chip-8-disassembler disas.cpp)
target_include_directories(chip-8-disassembler PRIVATE ${CHIP8_INCLUDE_DIRS})
target_link_libraries(chip-8-disassembler PRIVATE chip-8 fmt::fmt)
enable_warnings(chip-8-disassembler)
