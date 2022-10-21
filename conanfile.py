from conans import ConanFile
from conan.tools.cmake import CMake, cmake_layout, CMakeToolchain


class FpmConan(ConanFile):
    name = "fpm"
    license = "MIT"
    homepage = "https://github.com/MikeLankamp/fpm"
    url = "https://github.com/MikeLankamp/fpm"
    description = "C++ header-only fixed-point math library"
    settings = "os", "compiler", "arch", "build_type"
    generators = "CMakeDeps"

    build_requires = [
        ("gtest/[>=1.0 <2.0]"),
        ("benchmark/[>=1.0 <2.0]"),
    ]

    exports_sources = "CMakeLists.txt", "accuracy/*", "benchmarks/*", "include/*", "tests/*"

    def layout(self):
        cmake_layout(self)

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        cmake.test()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_id(self):
        self.info.clear()