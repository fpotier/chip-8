project("Chip-8 SDL2" VERSION 0.1 LANGUAGES CXX)

add_executable(chip-8-sdl
    app.cpp
    main.cpp
    sdl_helper.cpp
    widget/chip8_screen.cpp
    widget/label.cpp
    widget/panel.cpp
    widget/widget.cpp
)

if (WIN32)
    set_target_properties(chip-8-sdl PROPERTIES WIN32_EXECUTABLE TRUE)
    target_link_libraries(chip-8-sdl PRIVATE SDL2main)
endif()
target_include_directories(chip-8-sdl PRIVATE ${CHIP8_INCLUDE_DIRS} ".")
target_include_directories(chip-8-sdl SYSTEM PRIVATE "${cxxopts_SOURCE_DIR}/include")
target_link_libraries(chip-8-sdl PRIVATE chip-8 "$<IF:$<BOOL:${BUILD_SHARED_LIBS}>,SDL2,SDL2-static>" SDL2_ttf yaml-cpp fmt::fmt)
enable_warnings(chip-8-sdl)
copy_dll(chip-8-sdl "SDL2;SDL2_ttf;chip-8;fmt;yaml-cpp")

get_filename_component(SOUND_FILE_PATH "res/audio/beep-02.wav" ABSOLUTE)
get_filename_component(FONT_FILE_PATH "res/fonts/PressStart2P-Regular.ttf" ABSOLUTE)
configure_file(configuration.yaml.in configuration.yaml)
