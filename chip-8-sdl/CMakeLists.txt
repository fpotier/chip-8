cmake_minimum_required(VERSION 3.8)

project("Chip-8 SDL2" VERSION 0.1 LANGUAGES CXX)

option(BUILD_SHARED_LIBS "Build shared libs" TRUE)

if (PROJECT_IS_TOP_LEVEL)
    add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/../lib" ${CMAKE_BINARY_DIR}/libchip-8)
    include(${CMAKE_CURRENT_LIST_DIR}/../cmake/copy_dll.cmake)
    include(${CMAKE_CURRENT_LIST_DIR}/../cmake/compiler_flags.cmake)
endif()

add_executable(chip-8-sdl
    app.cpp
    main.cpp)

target_include_directories(chip-8-sdl PRIVATE ${CHIP8_INCLUDE_DIRS})
target_link_libraries(chip-8-sdl PRIVATE chip-8)

find_package(SDL2 QUIET)
if (${SDL2_FOUND})
    target_include_directories(chip-8-sdl PRIVATE ${SDL2_INCLUDE_DIRS})
    target_link_libraries(chip-8-sdl PRIVATE ${SDL2_LIBRARIES})
else()
    include(FetchContent)
    set(FETCHCONTENT_QUIET FALSE)
    FetchContent_Declare(SDL2
        GIT_REPOSITORY "https://github.com/libsdl-org/SDL"
        GIT_TAG 2.0.22
        GIT_PROGRESS TRUE
    )
    message(STATUS "Fetching SDL2")
    if (${BUILD_SHARED_LIBS})
        set(SDL_SHARED ON)
        set(SDL_STATIC OFF)
    else()
        set(SDL_SHARED OFF)
        set(SDL_STATIC ON)
    endif()
    FetchContent_MakeAvailable(SDL2)

    target_include_directories(chip-8-sdl PRIVATE "${SDL2_SOURCE_DIR}/include")
    if (${BUILD_SHARED_LIBS})
        target_link_libraries(chip-8-sdl PRIVATE SDL2)
    else()
        target_link_libraries(chip-8-sdl PRIVATE SDL2-static)
    endif()
    if (WIN32)
        set_target_properties(chip-8-sdl PROPERTIES WIN32_EXECUTABLE TRUE)
        target_link_libraries(chip-8-sdl PRIVATE SDL2main)
    endif()
endif()

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
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
message(STATUS "$Fetching cxxopts")
FetchContent_MakeAvailable(cxxopts)

target_include_directories(chip-8-sdl PRIVATE "${YAML_CPP_SOURCE_DIR}/include" "${cxxopts_SOURCE_DIR}/include")
target_link_libraries(chip-8-sdl PRIVATE yaml-cpp)

copy_dll(chip-8-sdl "SDL2;chip-8;fmt;yaml-cpp")
