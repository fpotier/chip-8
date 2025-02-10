cmake_minimum_required(VERSION 3.15)
project(PackageTest CXX)

find_package(chip-8 CONFIG REQUIRED)

add_executable(example src/example.cpp)
target_link_libraries(example chip-8::chip-8)
