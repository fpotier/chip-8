project("Lib Chip-8" VERSION 0.1 LANGUAGES CXX)

set(CHIP8_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/include" CACHE INTERNAL "")
set(CHIP8_LIBRARIES $<TARGET_FILE:chip-8> CACHE INTERNAL "")

add_subdirectory(src)
# add_subdirectory(test)
