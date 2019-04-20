cmake_minimum_required(VERSION 3.9)

set(CMAKE_CXX_STANDARD 11)

add_library(fpm INTERFACE)

target_include_directories(fpm INTERFACE include)

#
# Test suite
#
enable_testing()
include(GoogleTest)

add_executable(fpm-test
  tests/arithmetic.cpp
  tests/arithmetic_int.cpp
  tests/basic_math.cpp
  tests/detail.cpp
  tests/manip.cpp
  tests/nearest.cpp
  tests/power.cpp
  tests/trigonometry.cpp
)

target_link_libraries(fpm-test PRIVATE fpm gtest_main)
gtest_add_tests(TARGET fpm-test)

#
# libfixmath for alternative comparison
#
include(ExternalProject)

# Libfixmath uses a Makefile with hardcoded compiler (gcc).
# So we need to define our own build rules.
set(LIBFIXMATH_ROOT "${CMAKE_CURRENT_BINARY_DIR}/libfixmath")

ExternalProject_Add(libfixmath-external
  GIT_REPOSITORY "https://github.com/PetteriAimonen/libfixmath.git"
  GIT_TAG bada934981a5961569ad2cb6b9006ed94542cc9c
  PREFIX ${LIBFIXMATH_ROOT}
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
)

set(LIBFIXMATH_SOURCES
  ${LIBFIXMATH_ROOT}/src/libfixmath-external/libfixmath/fix16.c
  ${LIBFIXMATH_ROOT}/src/libfixmath-external/libfixmath/fix16_exp.c
  ${LIBFIXMATH_ROOT}/src/libfixmath-external/libfixmath/fix16_sqrt.c
  ${LIBFIXMATH_ROOT}/src/libfixmath-external/libfixmath/fix16_str.c
  ${LIBFIXMATH_ROOT}/src/libfixmath-external/libfixmath/fix16_trig.c
  ${LIBFIXMATH_ROOT}/src/libfixmath-external/libfixmath/fract32.c
  ${LIBFIXMATH_ROOT}/src/libfixmath-external/libfixmath/uint32.c
)
set_source_files_properties(${LIBFIXMATH_SOURCES} PROPERTIES GENERATED TRUE)
add_library(libfixmath ${LIBFIXMATH_SOURCES})
target_include_directories(libfixmath PUBLIC ${LIBFIXMATH_ROOT}/src/libfixmath-external/libfixmath)
add_dependencies(libfixmath libfixmath-external)

#
# Benchmark tool.
# Runs benchmarks of FPM operations and dumps results to standard output.
#
add_executable(fpm-benchmark
	benchmarks/arithmetic.cpp
	benchmarks/power.cpp
	benchmarks/trigonometry.cpp
)
target_link_libraries(fpm-benchmark PRIVATE fpm libfixmath benchmark benchmark_main)

#
# Accuracy tool.
# Dumps the accuracy of approximated FPM functions to JSON files.
#
add_executable(fpm-accuracy
	accuracy/accuracy.cpp
)
set_target_properties(fpm-accuracy PROPERTIES CXX_STANDARD 14)
target_link_libraries(fpm-accuracy PRIVATE fpm libfixmath)


include(FindGnuplot)
if (GNUPLOT_FOUND)
  # Create accuracy data
  set(DATA_DIR ${CMAKE_CURRENT_BINARY_DIR})

  set(DATA_FILES_ACCURACY "")
  set(IMG_FILES_ACCURACY "")
  foreach(DATA sin-trig cos-trig tan-trig asin-invtrig acos-invtrig atan-invtrig atan2-trig sqrt-auto)
    string(REGEX MATCHALL "[^-]+" M ${DATA})
    list(GET M 0 SERIES)
    list(GET M 1 TYPE)

    set(IMG_FILE ${DATA_DIR}/accuracy-${SERIES}.png)
    list(APPEND DATA_FILES_ACCURACY ${DATA_DIR}/${SERIES}.csv)
    list(APPEND IMG_FILES_ACCURACY ${IMG_FILE})

    add_custom_command(
      OUTPUT ${IMG_FILE}
      COMMAND ${GNUPLOT_EXECUTABLE} -c ${PROJECT_SOURCE_DIR}/accuracy/accuracy.gnuplot ${SERIES} ${TYPE}
      DEPENDS ${DATA_DIR}/${SERIES}.csv accuracy/accuracy.gnuplot
      WORKING_DIRECTORY ${DATA_DIR}
      VERBATIM
      COMMENT "Plotting ${SERIES} accuracy data"
    )
  endforeach(DATA)

  add_custom_command(
    OUTPUT ${DATA_FILES_ACCURACY}
    COMMAND fpm-accuracy
    DEPENDS fpm-accuracy
    WORKING_DIRECTORY ${DATA_DIR}
    VERBATIM
    COMMENT "Generating accuracy data with fpm-accuracy"
  )

  add_custom_target(fpm-accuracy-images DEPENDS ${IMG_FILES_ACCURACY})
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Release")
  # Enable the performance data-generation command only for Release builds.

  set(DATA_FILE_PERFORMANCE_JSON ${DATA_DIR}/performance.json)
  add_custom_command(
    OUTPUT ${DATA_FILE_PERFORMANCE_JSON}
    COMMAND fpm-benchmark --benchmark_out=${DATA_FILE_PERFORMANCE_JSON} --benchmark_out_format=json
    DEPENDS fpm-benchmark
    WORKING_DIRECTORY ${DATA_DIR}
    VERBATIM
    COMMENT "Generating performance data with fpm-benchmark"
  )

  set(DATA_FILE_PERFORMANCE_GNUPLOT ${DATA_DIR}/performance.csv)
  add_custom_command(
    OUTPUT ${DATA_FILE_PERFORMANCE_GNUPLOT}
    COMMAND python ${PROJECT_SOURCE_DIR}/benchmarks/benchmark.py ${DATA_FILE_PERFORMANCE_JSON} ${DATA_FILE_PERFORMANCE_GNUPLOT}
    DEPENDS ${DATA_FILE_PERFORMANCE_JSON} benchmarks/benchmark.py
    WORKING_DIRECTORY ${DATA_DIR}
    VERBATIM
    COMMENT "Converting performance data for GnuPlot"
  )

  set(IMG_FILE_PERFORMANCE "{$DATA_DIR}/performance.png")
  add_custom_command(
    OUTPUT ${IMG_FILE_PERFORMANCE}
    COMMAND ${GNUPLOT_EXECUTABLE} ${PROJECT_SOURCE_DIR}/benchmarks/benchmark.gnuplot
    DEPENDS ${DATA_FILE_PERFORMANCE_GNUPLOT} benchmarks/benchmark.gnuplot
    WORKING_DIRECTORY ${DATA_DIR}
    VERBATIM
    COMMENT "Plotting performance data"
  )

  add_custom_target(fpm-performance-images DEPENDS ${IMG_FILE_PERFORMANCE})
endif()

add_subdirectory(3rdparty/googlebench)
add_subdirectory(3rdparty/googletest)
