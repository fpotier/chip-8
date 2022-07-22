include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
FetchContent_Declare(fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 8.1.1
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(fmt)

add_library(chip-8
    chip8.cpp
    opcode.cpp
)

target_include_directories(chip-8 PRIVATE ${CHIP8_INCLUDE_DIRS})
target_link_libraries(chip-8 PRIVATE fmt::fmt)