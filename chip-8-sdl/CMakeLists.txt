cmake_minimum_required(VERSION 3.8)
include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON)

project("Chip-8 SDL2" VERSION 0.1 LANGUAGES CXX)

option(BUILD_SHARED_LIBS "Build shared libs" TRUE)

if (PROJECT_IS_TOP_LEVEL)
    add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/../lib" ${CMAKE_BINARY_DIR}/libchip-8)
    include(${CMAKE_CURRENT_LIST_DIR}/../cmake/copy_dll.cmake)
    include(${CMAKE_CURRENT_LIST_DIR}/../cmake/compiler_flags.cmake)
endif()

add_executable(chip-8-sdl
    app.cpp
    main.cpp
    sdl_helper.cpp
    widget/chip8_screen.cpp
    widget/panel.cpp
    widget/widget.cpp
)

FetchContent_Declare(SDL2
    GIT_REPOSITORY "https://github.com/libsdl-org/SDL"
    GIT_TAG 2.0.22
    GIT_PROGRESS TRUE
)
message(STATUS "Fetching SDL2")
FetchContent_MakeAvailable(SDL2)

set(SDL2TTF_VENDORED TRUE)
FetchContent_Declare(SDL2_ttf
    GIT_REPOSITORY "https://github.com/libsdl-org/SDL_ttf.git"
    GIT_TAG release-2.20.1
    GIT_PROGRESS TRUE
)
message(STATUS "Fetching SDL2_ttf")
FetchContent_MakeAvailable(SDL2_ttf)

FetchContent_Declare(yaml-cpp
    GIT_REPOSITORY "https://github.com/jbeder/yaml-cpp"
    GIT_TAG yaml-cpp-0.7.0
    GIT_PROGRESS TRUE
)
message(STATUS "Fetching yaml-cpp")
FetchContent_MakeAvailable(yaml-cpp)

FetchContent_Declare(cxxopts
    GIT_REPOSITORY "https://github.com/jarro2783/cxxopts.git"
    GIT_TAG v3.0.0
    GIT_PROGRESS TRUE
)
message(STATUS "Fetching cxxopts")
FetchContent_MakeAvailable(cxxopts)

if (WIN32)
    set_target_properties(chip-8-sdl PROPERTIES WIN32_EXECUTABLE TRUE)
    target_link_libraries(chip-8-sdl PRIVATE SDL2main)
endif()
target_include_directories(chip-8-sdl PRIVATE ${CHIP8_INCLUDE_DIRS} "." "widget")
target_include_directories(chip-8-sdl SYSTEM PRIVATE "${cxxopts_SOURCE_DIR}/include")
target_link_libraries(chip-8-sdl PRIVATE chip-8 "$<IF:$<BOOL:${BUILD_SHARED_LIBS}>,SDL2,SDL2-static>" SDL2_ttf yaml-cpp fmt::fmt)
enable_warnings(chip-8-sdl)
copy_dll(chip-8-sdl "SDL2;SDL2_ttf;chip-8;fmt;yaml-cpp")

get_filename_component(SOUND_FILE_PATH "res/audio/beep-02.wav" ABSOLUTE)
get_filename_component(FONT_FILE_PATH "res/fonts/PressStart2P-Regular.ttf" ABSOLUTE)
configure_file(configuration.yaml.in configuration.yaml)
