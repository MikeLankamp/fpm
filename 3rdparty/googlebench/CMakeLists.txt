project (benchmark)

add_library(benchmark
  src/benchmark.cc
  src/benchmark_register.cc
  src/colorprint.cc
  src/commandlineflags.cc
  src/complexity.cc
  src/console_reporter.cc
  src/counter.cc
  src/csv_reporter.cc
  src/json_reporter.cc
  src/reporter.cc
  src/sleep.cc
  src/statistics.cc
  src/string_util.cc
  src/sysinfo.cc
  src/timers.cc
)
target_include_directories(benchmark PUBLIC include)

# Ensure we have pthreads
find_package(Threads REQUIRED)

# Link threads.
target_link_libraries(benchmark Threads::Threads)
find_library(LIBRT rt)
if(LIBRT)
  target_link_libraries(benchmark ${LIBRT})
endif()

# We need extra libraries on Windows
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  target_compile_definitions(benchmark PRIVATE _CRT_SECURE_NO_WARNINGS)
  target_link_libraries(benchmark Shlwapi)
endif()

# Benchmark main library
add_library(benchmark_main
  src/benchmark_main.cc
)
target_link_libraries(benchmark_main benchmark)