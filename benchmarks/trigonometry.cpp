#include <benchmark/benchmark.h>
#include <fpm/fixed.hpp>
#include <fpm/math.hpp>
#include <fixmath.h>

#define BENCHMARK_TEMPLATE1_CAPTURE(func, test_case_name, a, ...)   \
  BENCHMARK_PRIVATE_DECLARE(func) =                                 \
      (::benchmark::internal::RegisterBenchmarkInternal(            \
          new ::benchmark::internal::FunctionBenchmark(             \
              #func "<" #a ">/" #test_case_name,					\
              [](::benchmark::State& st) { func<a>(st, __VA_ARGS__); })))

template <Fix16 (Fix16::*func)() const>
static Fix16 fix16_func1(Fix16 f)
{
    return (f.*func)();
}

template <Fix16 (Fix16::*func)(const Fix16&) const>
static Fix16 fix16_func2(Fix16 f, Fix16 y)
{
    return (f.*func)(y);
}

// Constant for our trigonometry function argument.
// Stored as volatile to force the compiler to read them and
// not optimize the entire expression into a constant.
static volatile int16_t s_x = 174;

template <typename TValue>
static void trigonometry(benchmark::State& state, TValue (*func)(TValue))
{
    for (auto _ : state)
    {
        TValue x{ static_cast<TValue>(s_x / 256.0) };
        benchmark::DoNotOptimize(func(x));
    }
}

template <typename TValue, TValue (*func)(TValue, TValue)>
static TValue func2_proxy(TValue value)
{
    // Add a 'random' offset for the second argument
    return func(value, value + 2);
}

BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, sin, float, &std::sin);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, cos, float, &std::cos);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, tan, float, &std::tan);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, asin, float, &std::asin);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, acos, float, &std::acos);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, atan, float, &std::atan);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, atan2, float, &func2_proxy<float, &std::atan2>);

BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, sin, double, &std::sin);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, cos, double, &std::cos);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, tan, double, &std::tan);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, asin, double, &std::asin);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, acos, double, &std::acos);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, atan, double, &std::atan);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, atan2, double, &func2_proxy<double, &std::atan2>);

BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, sin, fpm::fixed_16_16, &fpm::sin);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, cos, fpm::fixed_16_16, &fpm::cos);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, tan, fpm::fixed_16_16, &fpm::tan);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, asin, fpm::fixed_16_16, &fpm::asin);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, acos, fpm::fixed_16_16, &fpm::acos);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, atan, fpm::fixed_16_16, &fpm::atan);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, atan2, fpm::fixed_16_16, &func2_proxy<fpm::fixed_16_16, &fpm::atan2>);

BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, sin,  Fix16, fix16_func1<&Fix16::sin>);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, cos,  Fix16, fix16_func1<&Fix16::cos>);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, tan,  Fix16, fix16_func1<&Fix16::tan>);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, asin, Fix16, fix16_func1<&Fix16::asin>);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, acos, Fix16, fix16_func1<&Fix16::acos>);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, atan, Fix16, fix16_func1<&Fix16::atan>);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, atan2, Fix16, &func2_proxy<Fix16, &fix16_func2<&Fix16::atan2>>);

