cmake_minimum_required(VERSION 3.15)
project(chip-8-gui CXX)

find_package(imgui REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glew REQUIRED)

add_executable(chip-8-gui
    src/main.cpp
    ${imgui_INCLUDE_DIR}/../res/bindings/imgui_impl_glfw.cpp
    ${imgui_INCLUDE_DIR}/../res/bindings/imgui_impl_opengl3.cpp
)
target_include_directories(chip-8-gui PRIVATE ${imgui_INCLUDE_DIR}/../res/bindings)
target_link_libraries(chip-8-gui imgui::imgui GLEW::GLEW glfw)

install(TARGETS chip-8-gui DESTINATION "."
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
)
