cmake_minimum_required(VERSION 3.9)

set(CMAKE_CXX_STANDARD 11)

add_library(fpm INTERFACE)

target_include_directories(fpm INTERFACE include)

enable_testing()
include(GoogleTest)

add_executable(fpm-test
  tests/arithmetic.cpp
  tests/basic_math.cpp
  tests/detail.cpp
  tests/manip.cpp
  tests/nearest.cpp
  tests/power.cpp
  tests/trigonometry.cpp
)

target_link_libraries(fpm-test PRIVATE fpm gtest_main)
gtest_add_tests(TARGET fpm-test)


add_executable(fpm-benchmark
	benchmarks/arithmetic.cpp
	benchmarks/power.cpp
	benchmarks/trigonometry.cpp
)
target_link_libraries(fpm-benchmark PRIVATE fpm benchmark benchmark_main)

add_subdirectory(3rdparty/googlebench)
add_subdirectory(3rdparty/googletest)
