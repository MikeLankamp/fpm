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

// Constant for our trigonometry function argument.
// Stored as volatile to force the compiler to read them and 
// not optimize the entire expression into a constant.
volatile double s_x = 0.678;

template <typename TValue>
static void trigonometry(benchmark::State& state, TValue (*func)(TValue))
{
	for (auto _ : state)
	{
		TValue x{ static_cast<TValue>(s_x) };
		benchmark::DoNotOptimize(func(x));
	}
}

BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, sin, float, &std::sin);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, cos, float, &std::cos);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, tan, float, &std::tan);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, asin, float, &std::asin);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, acos, float, &std::acos);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, atan, float, &std::atan);

BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, sin, double, &std::sin);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, cos, double, &std::cos);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, tan, double, &std::tan);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, asin, double, &std::asin);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, acos, double, &std::acos);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, atan, double, &std::atan);

BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, sin, fpm::fixed_16_16, &fpm::sin);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, cos, fpm::fixed_16_16, &fpm::cos);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, tan, fpm::fixed_16_16, &fpm::tan);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, asin, fpm::fixed_16_16, &fpm::asin);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, acos, fpm::fixed_16_16, &fpm::acos);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, atan, fpm::fixed_16_16, &fpm::atan);

BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, sin,  Fix16, fix16_func<&Fix16::sin>);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, cos,  Fix16, fix16_func<&Fix16::cos>);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, tan,  Fix16, fix16_func<&Fix16::tan>);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, asin, Fix16, fix16_func<&Fix16::asin>);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, acos, Fix16, fix16_func<&Fix16::acos>);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, atan, Fix16, fix16_func<&Fix16::atan>);
