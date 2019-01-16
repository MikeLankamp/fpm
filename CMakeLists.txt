cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 11)

add_library(fpm INTERFACE)

target_include_directories(fpm INTERFACE include)

enable_testing()

add_executable(fpm-test
  tests/arithmethic.cpp
  tests/basic_math.cpp
  tests/manip.cpp
)

target_link_libraries(fpm-test PRIVATE fpm gtest_main)
add_test(NAME fpm-test COMMAND fpm-test)

add_subdirectory(3rdparty/googletest)