cmake_minimum_required(VERSION 3.8)

project("Lib Chip-8"
    VERSION 0.1
    LANGUAGES CXX
)
set(CMAKE_CXX_STANDARD 20)

option(BUILD_SHARED_LIBS "Build shared libs" TRUE)
option(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS "Export all symbols for Windows" TRUE)

set(CHIP8_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/include" CACHE INTERNAL "")
set(CHIP8_LIBRARIES $<TARGET_FILE:chip-8> CACHE INTERNAL "")

add_subdirectory(src)
add_subdirectory(test)