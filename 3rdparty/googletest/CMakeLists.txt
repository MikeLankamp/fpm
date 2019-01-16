find_package(Threads REQUIRED)

add_library(gtest STATIC
  src/gtest-all.cc
)

target_include_directories(gtest
  PUBLIC
    include
  PRIVATE
    .
)

target_link_libraries(gtest
  PRIVATE
    Threads::Threads
)

add_library(gtest_main STATIC
  src/gtest_main.cc
)

target_link_libraries(gtest_main
  PUBLIC
    gtest
)

