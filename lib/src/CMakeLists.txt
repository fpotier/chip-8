add_library(chip-8
    chip8.cpp
    opcode.cpp
)
target_include_directories(chip-8 PRIVATE ${CHIP8_INCLUDE_DIRS})
target_link_libraries(chip-8 PRIVATE fmt::fmt)
enable_warnings(chip-8)
