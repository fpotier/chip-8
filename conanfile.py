from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps

class Chip8Recipe(ConanFile):
    name = "chip-8"
    version = "0.1"

    author = "fpotier"
    url = "https://github.com/fpotier/chip-8"
    description = "Chip 8 emulator"
    topics = ("chip-8", "emulator")

    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "fPIC": [True, False]
    }
    default_options = {
        "shared": True,
        "fPIC": True
    }

    exports_sources = "CMakeLists.txt", "lib/*", "chip-8-sdl/*", "chip-8-disassembler/*", "cmake/*"

    def build_requirements(self):
        self.tool_requires("cmake/[>=3.20.0]")

    def requirements(self):
        self.requires("sdl/2.28.3")
        self.requires("sdl_ttf/2.20.2")
        self.requires("fmt/10.2.1")
        self.requires("doctest/2.4.11")
        self.requires("yaml-cpp/0.8.0")
        self.requires("cxxopts/3.0.0")

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

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
