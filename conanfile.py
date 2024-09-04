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
        self.tool_requires("cmake/[>=3.30.1]")

    def requirements(self):
        # 'force' is required to bump sdl version because the sdl_ttf recipe uses a hard coded sdl version
        self.requires("sdl/2.30.7", force=True)
        self.requires("sdl_ttf/2.22.0")
        self.requires("fmt/10.2.1")
        self.requires("yaml-cpp/0.8.0")
        self.requires("cxxopts/3.2.0")

        self.test_requires("doctest/2.4.11")

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
        if not self.conf.get("tools.build:skip_test", default=False):
            cmake.ctest()

    def package(self):
        cmake = CMake(self)
        cmake.install()
