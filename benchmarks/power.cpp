#include <benchmark/benchmark.h>
#include <fpm/fixed.h>
#include <fixmath.h>

#define BENCHMARK_TEMPLATE1_CAPTURE(func, test_case_name, a, ...)   \
  BENCHMARK_PRIVATE_DECLARE(func) =                                 \
      (::benchmark::internal::RegisterBenchmarkInternal(            \
          new ::benchmark::internal::FunctionBenchmark(             \
              #func "<" #a ">/" #test_case_name,					\
		   	  [](::benchmark::State& st) { func<a>(st, __VA_ARGS__); })))

template <Fix16 (Fix16::*func)() const>
static Fix16 fix16_func(Fix16 f)
{
	return (f.*func)();
}

// Constant for our power function argument.
// Stored as volatile to force the compiler to read them and 
// not optimize the entire expression into a constant.
static volatile double s_x = 0.678;

template <typename TValue>
static void power(benchmark::State& state, TValue (*func)(TValue))
{
	for (auto _ : state)
	{
		TValue x{ static_cast<TValue>(s_x) };
		benchmark::DoNotOptimize(func(x));
	}
}

BENCHMARK_TEMPLATE1_CAPTURE(power, sqrt, float, &std::sqrt);
BENCHMARK_TEMPLATE1_CAPTURE(power, sqrt, double, &std::sqrt);
BENCHMARK_TEMPLATE1_CAPTURE(power, sqrt, Fix16, fix16_func<&Fix16::sqrt>);
BENCHMARK_TEMPLATE1_CAPTURE(power, sqrt, fpm::fixed_16_16, &fpm::sqrt);
