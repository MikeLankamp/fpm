cmake_minimum_required(VERSION 3.9)

project(fpm)

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 11)

add_library(fpm INTERFACE)

target_include_directories(fpm INTERFACE include)

install(FILES
  include/fpm/fixed.hpp
  include/fpm/ios.hpp
  include/fpm/math.hpp
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/fpm)

OPTION(BUILD_ACCURACY  "fpm accuracy"  ON)
OPTION(BUILD_BENCHMARK "fpm benchmark" ON)
OPTION(BUILD_TESTS     "fpm tests"     ON)

# only build tests & benchmarks if a top-level project
if (NOT CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(BUILD_ACCURACY  OFF)
  set(BUILD_BENCHMARK OFF)
  set(BUILD_TESTS     OFF)
endif()

#
# Test suite
#
if (BUILD_TESTS)
enable_testing()
include(GoogleTest)

add_executable(fpm-test
  tests/arithmetic.cpp
  tests/arithmetic_int.cpp
  tests/basic_math.cpp
  tests/constants.cpp
  tests/conversion.cpp
  tests/classification.cpp
  tests/customizations.cpp
  tests/detail.cpp
  tests/input.cpp
  tests/manip.cpp
  tests/nearest.cpp
  tests/output.cpp
  tests/power.cpp
  tests/trigonometry.cpp
)
set_target_properties(fpm-test PROPERTIES CXX_STANDARD 11)
target_link_libraries(fpm-test PRIVATE fpm gtest_main)
gtest_add_tests(TARGET fpm-test)

endif()

#
# libfixmath for alternative comparison
#
include(ExternalProject)

if (BUILD_ACCURACY OR BUILD_BENCHMARK)

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

# Set libfixmath compile options to match fpm functionality
target_compile_definitions(libfixmath
  PUBLIC
    FIXMATH_NO_CACHE        # No caching of results

    #FIXMATH_NO_OVERFLOW    # We want no runtime-overflow detection, but it fails to compile
    #FIXMATH_NO_ROUNDING    # We do want rounding
    #FIXMATH_NO_64BIT       # We do want 64-bit integer support
    #FIXMATH_OPTIMIZE_8BIT  # No special 8-bit architecture optimizations
    #FIXMATH_SIN_LUT        # No lookup table for sin
    #FIXMATH_FAST_SIN       # Prefer accuracy over speed
)

target_include_directories(libfixmath PUBLIC ${LIBFIXMATH_ROOT}/src/libfixmath-external/libfixmath)
add_dependencies(libfixmath libfixmath-external)

endif()

if (BUILD_ACCURACY OR BUILD_BENCHMARK)
#
# CNL for alternative comparison
#
set(LIBCNL_ROOT "${CMAKE_CURRENT_BINARY_DIR}/libcnl")

ExternalProject_Add(libcnl-external
  GIT_REPOSITORY "https://github.com/johnmcfarlane/cnl"
  GIT_TAG 4d445566fe7c6c8939fffc145a2f30fd587796a6
  PREFIX ${LIBCNL_ROOT}
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
)

add_library(libcnl INTERFACE)
target_include_directories(libcnl INTERFACE ${LIBCNL_ROOT}/src/libcnl-external/include)
add_dependencies(libcnl libcnl-external)
endif()

if (BUILD_BENCHMARK)
#
# Benchmark tool.
# Runs benchmarks of FPM operations and dumps results to standard output.
#
add_executable(fpm-benchmark
	benchmarks/arithmetic.cpp
	benchmarks/power.cpp
	benchmarks/trigonometry.cpp
)
target_link_libraries(fpm-benchmark PRIVATE fpm libfixmath libcnl benchmark benchmark_main)
endif()

if (BUILD_ACCURACY)
#
# Accuracy tool.
# Dumps the accuracy of approximated FPM functions to JSON files.
#
add_executable(fpm-accuracy
	accuracy/accuracy.cpp
)
set_target_properties(fpm-accuracy PROPERTIES CXX_STANDARD 14)
target_link_libraries(fpm-accuracy PRIVATE fpm libfixmath)
endif()

set(DATA_DIR ${CMAKE_CURRENT_BINARY_DIR})

include(FindGnuplot)
if (GNUPLOT_FOUND AND BUILD_ACCURACY)
  # Create accuracy data
  set(DATA_FILES_ACCURACY "")
  set(IMG_FILES_ACCURACY "")
  foreach(DATA sin-trig cos-trig tan-trig asin-invtrig acos-invtrig atan-invtrig atan2-trig sqrt-auto cbrt-auto pow-auto exp-auto exp2-auto log-auto log2-auto log10-auto)
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

if (CMAKE_BUILD_TYPE STREQUAL "Release" AND GNUPLOT_FOUND AND BUILD_BENCHMARK)
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
    DEPENDS ${DATA_FILE_PERFORMANCE_JSON} ${PROJECT_SOURCE_DIR}/benchmarks/benchmark.py
    WORKING_DIRECTORY ${DATA_DIR}
    VERBATIM
    COMMENT "Converting performance data for GnuPlot"
  )

  set(IMG_FILE_PERFORMANCE "{$DATA_DIR}/performance.png")
  add_custom_command(
    OUTPUT ${IMG_FILE_PERFORMANCE}
    COMMAND ${GNUPLOT_EXECUTABLE} ${PROJECT_SOURCE_DIR}/benchmarks/benchmark.gnuplot
    DEPENDS ${DATA_FILE_PERFORMANCE_GNUPLOT} ${PROJECT_SOURCE_DIR}/benchmarks/benchmark.gnuplot
    WORKING_DIRECTORY ${DATA_DIR}
    VERBATIM
    COMMENT "Plotting performance data"
  )

  add_custom_target(fpm-performance-images DEPENDS ${IMG_FILE_PERFORMANCE})
endif()

if (BUILD_BENCHMARK)
  add_subdirectory(3rdparty/googlebench)
endif()

if (BUILD_TESTS)
  add_subdirectory(3rdparty/googletest)
endif()
