include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON)

FetchContent_Declare(SDL2
    GIT_REPOSITORY "https://github.com/libsdl-org/SDL"
    GIT_TAG 2.0.22
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(SDL2)

set(SDL2TTF_VENDORED TRUE)
FetchContent_Declare(SDL2_ttf
    GIT_REPOSITORY "https://github.com/libsdl-org/SDL_ttf.git"
    GIT_TAG release-2.20.1
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(SDL2_ttf)

FetchContent_Declare(fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 8.1.1
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(fmt)

FetchContent_Declare(doctest
    GIT_REPOSITORY https://github.com/doctest/doctest.git
    GIT_TAG v2.4.8
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(doctest)


FetchContent_Declare(yaml-cpp
    GIT_REPOSITORY "https://github.com/jbeder/yaml-cpp"
    GIT_TAG yaml-cpp-0.7.0
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(yaml-cpp)

FetchContent_Declare(cxxopts
    GIT_REPOSITORY "https://github.com/jarro2783/cxxopts.git"
    GIT_TAG v3.0.0
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(cxxopts)
