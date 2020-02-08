#include <benchmark/benchmark.h>
#include <fpm/fixed.hpp>
#include <fpm/math.hpp>
#include <cnl/fixed_point.h>
#include <fixmath.h>

#define BENCHMARK_TEMPLATE1_CAPTURE(func, test_case_name, a, ...)   \
  BENCHMARK_PRIVATE_DECLARE(func) =                                 \
      (::benchmark::internal::RegisterBenchmarkInternal(            \
          new ::benchmark::internal::FunctionBenchmark(             \
              #func "<" #a ">/" #test_case_name,					\
              [](::benchmark::State& st) { func<a>(st, __VA_ARGS__); })))

template <fix16_t (*func)(fix16_t)>
static Fix16 fix16_func(Fix16 f)
{
    return (*func)(f);
}

// Constants for our power function arguments.
// Stored as volatile to force the compiler to read them and
// not optimize the entire expression into a constant.
static volatile int16_t s_x = 2734;
static volatile int16_t s_y =  174;

template <typename TValue>
static void power1(benchmark::State& state, TValue (*func)(TValue))
{
    for (auto _ : state)
    {
        TValue x{ static_cast<TValue>(s_x / 256.0) };
        benchmark::DoNotOptimize(func(x));
    }
}

template <typename TValue>
static void power1(benchmark::State& state, TValue (*func)(const TValue&))
{
    for (auto _ : state)
    {
        TValue x{ static_cast<TValue>(s_x / 256.0) };
        benchmark::DoNotOptimize(func(x));
    }
}

template <typename TValue>
static void power2(benchmark::State& state, TValue (*func)(TValue, TValue))
{
    for (auto _ : state)
    {
        TValue x{ static_cast<TValue>(s_x / 256.0) };
        TValue y{ static_cast<TValue>(s_y / 256.0) };
        benchmark::DoNotOptimize(func(x, y));
    }
}

using CnlFixed16 = cnl::fixed_point<std::int32_t, -16>;

BENCHMARK_TEMPLATE1_CAPTURE(power1, sqrt, float, &std::sqrt);
BENCHMARK_TEMPLATE1_CAPTURE(power1, sqrt, double, &std::sqrt);
BENCHMARK_TEMPLATE1_CAPTURE(power1, sqrt, fpm::fixed_16_16, &fpm::sqrt);
BENCHMARK_TEMPLATE1_CAPTURE(power1, sqrt, Fix16, fix16_func<&fix16_sqrt>);
BENCHMARK_TEMPLATE1_CAPTURE(power1, sqrt, CnlFixed16, &cnl::sqrt);

BENCHMARK_TEMPLATE1_CAPTURE(power1, cbrt, float, &std::cbrt);
BENCHMARK_TEMPLATE1_CAPTURE(power1, cbrt, double, &std::cbrt);
BENCHMARK_TEMPLATE1_CAPTURE(power1, cbrt, fpm::fixed_16_16, &fpm::cbrt);

BENCHMARK_TEMPLATE1_CAPTURE(power1, log, float, &std::log);
BENCHMARK_TEMPLATE1_CAPTURE(power1, log, double, &std::log);
BENCHMARK_TEMPLATE1_CAPTURE(power1, log, fpm::fixed_16_16, &fpm::log);

BENCHMARK_TEMPLATE1_CAPTURE(power1, log2, float, &std::log2);
BENCHMARK_TEMPLATE1_CAPTURE(power1, log2, double, &std::log2);
BENCHMARK_TEMPLATE1_CAPTURE(power1, log2, fpm::fixed_16_16, &fpm::log2);
BENCHMARK_TEMPLATE1_CAPTURE(power1, log2, Fix16, fix16_func<&fix16_log2>);

BENCHMARK_TEMPLATE1_CAPTURE(power1, log10, float, &std::log10);
BENCHMARK_TEMPLATE1_CAPTURE(power1, log10, double, &std::log10);
BENCHMARK_TEMPLATE1_CAPTURE(power1, log10, fpm::fixed_16_16, &fpm::log10);

BENCHMARK_TEMPLATE1_CAPTURE(power1, exp, float, &std::exp);
BENCHMARK_TEMPLATE1_CAPTURE(power1, exp, double, &std::exp);
BENCHMARK_TEMPLATE1_CAPTURE(power1, exp, fpm::fixed_16_16, &fpm::exp);
BENCHMARK_TEMPLATE1_CAPTURE(power1, exp, Fix16, fix16_func<&fix16_exp>);
BENCHMARK_TEMPLATE1_CAPTURE(power1, exp, CnlFixed16, &cnl::exp);

BENCHMARK_TEMPLATE1_CAPTURE(power1, exp2, float, &std::exp2);
BENCHMARK_TEMPLATE1_CAPTURE(power1, exp2, double, &std::exp2);
BENCHMARK_TEMPLATE1_CAPTURE(power1, exp2, fpm::fixed_16_16, &fpm::exp2);

BENCHMARK_TEMPLATE1_CAPTURE(power2, pow, float, &std::pow);
BENCHMARK_TEMPLATE1_CAPTURE(power2, pow, double, &std::pow);
BENCHMARK_TEMPLATE1_CAPTURE(power2, pow, fpm::fixed_16_16, &fpm::pow);
