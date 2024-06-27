cmake_minimum_required(VERSION 3.20)

project("Chip-8" VERSION 0.1 LANGUAGES CXX)

option(BUILD_SHARED_LIBS "Build shared libs" ON)
option(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS "Export Windows symbols" ON)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(fmt REQUIRED)
find_package(doctest REQUIRED)
find_package(cxxopts REQUIRED)
find_package(yaml-cpp REQUIRED)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(cmake/compiler_flags.cmake)
enable_testing() # needed to create the target lib/test
add_subdirectory(lib)
add_subdirectory(chip-8-sdl)
add_subdirectory(chip-8-disassembler)
