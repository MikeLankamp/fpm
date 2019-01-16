cmake_minimum_required(VERSION 3.9)

set(CMAKE_CXX_STANDARD 11)

add_library(fpm INTERFACE)

target_include_directories(fpm INTERFACE include)

enable_testing()
include(GoogleTest)

add_executable(fpm-test
  tests/arithmethic.cpp
  tests/basic_math.cpp
  tests/manip.cpp
)

target_link_libraries(fpm-test PRIVATE fpm gtest_main)
gtest_add_tests(TARGET fpm-test)

add_subdirectory(3rdparty/googletest)
