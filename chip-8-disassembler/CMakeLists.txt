cmake_minimum_required(VERSION 3.8)

project("Chip-8 Disassembler" VERSION 0.1 LANGUAGES CXX)

if (PROJECT_IS_TOP_LEVEL)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../lib ${CMAKE_BINARY_DIR}/libchip-8)
    include(${CMAKE_CURRENT_LIST_DIR}/../cmake/copy_dll.cmake)
    include(${CMAKE_CURRENT_LIST_DIR}/../cmake/compiler_flags.cmake)
endif()

add_executable(chip-8-disassembler disas.cpp)

target_include_directories(chip-8-disassembler PRIVATE ${CHIP8_INCLUDE_DIRS})
target_link_libraries(chip-8-disassembler PRIVATE chip-8 fmt::fmt)
copy_dll(chip-8-disassembler chip-8)
copy_dll(chip-8-disassembler fmt)
