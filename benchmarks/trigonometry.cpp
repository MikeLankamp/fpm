#include <benchmark/benchmark.h>
#include <fpm/fixed.h>

#define BENCHMARK_TEMPLATE1_CAPTURE(func, test_case_name, a, ...)   \
  BENCHMARK_PRIVATE_DECLARE(func) =                                 \
      (::benchmark::internal::RegisterBenchmarkInternal(            \
          new ::benchmark::internal::FunctionBenchmark(             \
              #func "<" #a ">/" #test_case_name,					\
		   	  [](::benchmark::State& st) { func<a>(st, __VA_ARGS__); })))

// Constant for our trigonometry function argument.
// Stored as volatile to force the compiler to read them and 
// not optimize the entire expression into a constant.
static volatile double s_x = 0.678;

template <typename TValue>
static void trigonometry(benchmark::State& state, TValue (*func)(TValue))
{
	TValue x{ static_cast<TValue>(s_x) };
	for (auto _ : state)
	{
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

BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, sin, fpm::fixed_24_8, &fpm::sin);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, cos, fpm::fixed_24_8, &fpm::cos);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, tan, fpm::fixed_24_8, &fpm::tan);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, asin, fpm::fixed_24_8, &fpm::asin);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, acos, fpm::fixed_24_8, &fpm::acos);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, atan, fpm::fixed_24_8, &fpm::atan);

BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, sin, fpm::fixed_16_16, &fpm::sin);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, cos, fpm::fixed_16_16, &fpm::cos);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, tan, fpm::fixed_16_16, &fpm::tan);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, asin, fpm::fixed_16_16, &fpm::asin);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, acos, fpm::fixed_16_16, &fpm::acos);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, atan, fpm::fixed_16_16, &fpm::atan);

BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, sin, fpm::fixed_8_24, &fpm::sin);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, cos, fpm::fixed_8_24, &fpm::cos);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, tan, fpm::fixed_8_24, &fpm::tan);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, asin, fpm::fixed_8_24, &fpm::asin);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, acos, fpm::fixed_8_24, &fpm::acos);
BENCHMARK_TEMPLATE1_CAPTURE(trigonometry, atan, fpm::fixed_8_24, &fpm::atan);
