import os

from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps
from conan.tools.files import copy


class Chip8GUIRecipe(ConanFile):
    name = "chip-8-gui"
    version = "0.1"
    package_type = "application"

    license = "MIT"
    author = "fpotier"
    url = "https://github.com/fpotier/chip-8/chip-8-gui"
    description = "Chip 8 emulator"
    topics = ("chip-8", "emulator")

    settings = "os", "compiler", "build_type", "arch"

    exports_sources = "CMakeLists.txt", "src/*"

    def requirements(self):
        self.requires("chip-8/0.1")
        self.requires("imgui/1.91.5")
        self.requires("glfw/3.3.8")
        self.requires("glew/2.2.0")

    def generate(self):
        copy(self, "*glfw*", os.path.join(self.dependencies["imgui"].package_folder,
             "res", "bindings"), os.path.join(self.source_folder, "bindings"))
        copy(self, "*opengl3*", os.path.join(self.dependencies["imgui"].package_folder,
             "res", "bindings"), os.path.join(self.source_folder, "bindings"))

    def layout(self):
        cmake_layout(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()
