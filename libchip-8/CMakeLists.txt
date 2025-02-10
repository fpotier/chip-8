cmake_minimum_required(VERSION 3.15)
project(chip-8 LANGUAGES CXX VERSION ${PROJECT_VERSION})

find_package(fmt REQUIRED)
find_package(doctest REQUIRED)

add_library(chip-8 
    src/chip-8.cpp
    src/opcode.cpp
)
target_compile_definitions(chip-8 PUBLIC LIBCHIP8_VERSION=\"${PROJECT_VERSION}\")
target_include_directories(chip-8 PUBLIC include)
target_link_libraries(chip-8 fmt::fmt)

enable_testing()
add_subdirectory(test)

set_target_properties(chip-8 PROPERTIES PUBLIC_HEADER "include/chip-8.h;include/opcode.h;include/cpu_quirks.h")
install(TARGETS chip-8
    PUBLIC_HEADER
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/chip-8
)
