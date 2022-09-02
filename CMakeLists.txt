cmake_minimum_required(VERSION 3.20)

project("Chip-8" VERSION 0.1 LANGUAGES CXX C)

option(BUILD_SHARED_LIBS "Build shared libs" ON)
option(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS "Export Windows symbols" ON)

include(cmake/dependencies.cmake)
include(cmake/copy_dll.cmake)
include(cmake/compiler_flags.cmake)
enable_testing() # needed to create the target lib/test
add_subdirectory(lib)
add_subdirectory(chip-8-sdl)
add_subdirectory(chip-8-disassembler)
