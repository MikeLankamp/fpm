#include <benchmark/benchmark.h>
#include <fpm/fixed.h>

#include <fixmath.h>

#define BENCHMARK_TEMPLATE1_CAPTURE(func, test_case_name, a, ...)   \
  BENCHMARK_PRIVATE_DECLARE(func) =                                 \
      (::benchmark::internal::RegisterBenchmarkInternal(            \
          new ::benchmark::internal::FunctionBenchmark(             \
              #func "<" #a ">/" #test_case_name,					\
              [](::benchmark::State& st) { func<a>(st, __VA_ARGS__); })))

// Constants for our arithmetic operands.
// Stored as volatile to force the compiler to read them and 
// not optimize the entire expression into a constant.
static volatile int s_x = 1;
static volatile int s_y = 2;

template <typename TValue>
static void arithmetic(benchmark::State& state, TValue (*func)(TValue, TValue))
{
	for (auto _ : state)
	{
		TValue x{ static_cast<TValue>(s_x) }, y{ static_cast<TValue>(s_y) };
		benchmark::DoNotOptimize(func(x, y));
	}
}

#define FUNC(TYPE, OP) \
	[](TYPE x, TYPE y) { return x OP y; }

BENCHMARK_TEMPLATE1_CAPTURE(arithmetic, add, float, FUNC(float, +));
BENCHMARK_TEMPLATE1_CAPTURE(arithmetic, sub, float, FUNC(float, -));
BENCHMARK_TEMPLATE1_CAPTURE(arithmetic, mul, float, FUNC(float, *));
BENCHMARK_TEMPLATE1_CAPTURE(arithmetic, div, float, FUNC(float, /));

BENCHMARK_TEMPLATE1_CAPTURE(arithmetic, add, double, FUNC(double, +));
BENCHMARK_TEMPLATE1_CAPTURE(arithmetic, sub, double, FUNC(double, -));
BENCHMARK_TEMPLATE1_CAPTURE(arithmetic, mul, double, FUNC(double, *));
BENCHMARK_TEMPLATE1_CAPTURE(arithmetic, div, double, FUNC(double, /));

BENCHMARK_TEMPLATE1_CAPTURE(arithmetic, add, Fix16, FUNC(Fix16, +));
BENCHMARK_TEMPLATE1_CAPTURE(arithmetic, sub, Fix16, FUNC(Fix16, -));
BENCHMARK_TEMPLATE1_CAPTURE(arithmetic, mul, Fix16, FUNC(Fix16, *));
BENCHMARK_TEMPLATE1_CAPTURE(arithmetic, div, Fix16, FUNC(Fix16, /));

BENCHMARK_TEMPLATE1_CAPTURE(arithmetic, add, fpm::fixed_16_16, FUNC(fpm::fixed_16_16, +));
BENCHMARK_TEMPLATE1_CAPTURE(arithmetic, sub, fpm::fixed_16_16, FUNC(fpm::fixed_16_16, -));
BENCHMARK_TEMPLATE1_CAPTURE(arithmetic, mul, fpm::fixed_16_16, FUNC(fpm::fixed_16_16, *));
BENCHMARK_TEMPLATE1_CAPTURE(arithmetic, div, fpm::fixed_16_16, FUNC(fpm::fixed_16_16, /));
