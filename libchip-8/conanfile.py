from conan import ConanFile
from conan.tools.build import check_min_cppstd
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps


class Chip8Recipe(ConanFile):
    name = "chip-8"
    version = "0.1"
    package_type = "library"

    license = "MIT"
    author = "fpotier"
    url = "https://github.com/fpotier/chip-8/libchip-8"
    description = "Chip 8 emulation library"
    topics = ("chip-8", "emulator")

    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}

    exports_sources = "CMakeLists.txt", "src/*", "include/*", "test/*"

    def build_requirements(self):
        self.tool_requires("cmake/[>=3.30.1]")
        self.test_requires("doctest/2.4.11")

    def requirements(self):
        self.requires("fmt/11.1.1")

    def config_options(self):
        if self.settings.os == "Windows":
            self.options.rm_safe("fPIC")

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")

    def validate(self):
        check_min_cppstd(self, 17)

    def layout(self):
        cmake_layout(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure(variables={'PROJECT_VERSION': self.version})
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()
        if not self.conf.get("tools.build:skip_test", default=False):
            cmake.ctest()

    def package_info(self):
        self.cpp_info.libs = ["chip-8"]
