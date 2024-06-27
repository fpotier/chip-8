project("Chip-8 SDL2" VERSION 0.1 LANGUAGES CXX)

add_executable(chip-8-sdl
    app.cpp
    main.cpp
    sdl_helper.cpp
    widget/button.cpp
    widget/chip8_screen.cpp
    widget/label.cpp
    widget/panel.cpp
    widget/widget.cpp
)

target_include_directories(chip-8-sdl PRIVATE ${CHIP8_INCLUDE_DIRS} ".")
target_include_directories(chip-8-sdl SYSTEM PRIVATE "${cxxopts_SOURCE_DIR}/include")
target_link_libraries(chip-8-sdl PRIVATE
    chip-8
    SDL2::SDL2main
    SDL2::SDL2-static
    SDL2_ttf::SDL2_ttf-static
    yaml-cpp
    fmt::fmt
    cxxopts::cxxopts
)
enable_warnings(chip-8-sdl)
get_filename_component(SOUND_FILE_PATH "res/audio/beep-02.wav" ABSOLUTE)
get_filename_component(FONT_FILE_PATH "res/fonts/PressStart2P-Regular.ttf" ABSOLUTE)
configure_file(configuration.yaml.in configuration.yaml)
