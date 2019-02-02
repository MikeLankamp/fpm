#include <benchmark/benchmark.h>
#include <fpm/fixed.h>

// Constants for our arithmetic operands.
// Stored as volatile to force the compiler to read them and 
// not optimize the entire expression into a constant.
static volatile int s_x = 1;
static volatile int s_y = 2;

template <typename TValue>
static void arithmetic_addition(benchmark::State& state)
{
	TValue x{ static_cast<TValue>(s_x) }, y{ static_cast<TValue>(s_y) };
	for (auto _ : state)
	{
		benchmark::DoNotOptimize(x + y);
	}
}

template <typename TValue>
static void arithmetic_subtraction(benchmark::State& state)
{
	TValue x{ static_cast<TValue>(s_x) }, y{ static_cast<TValue>(s_y) };
	for (auto _ : state)
	{
		benchmark::DoNotOptimize(x - y);
	}
}

template <typename TValue>
static void arithmetic_multiplication(benchmark::State& state)
{
	TValue x{ static_cast<TValue>(s_x) }, y{ static_cast<TValue>(s_y) };
	for (auto _ : state)
	{
		benchmark::DoNotOptimize(x * y);
	}
}

template <typename TValue>
static void arithmetic_division(benchmark::State& state)
{
	TValue x{ static_cast<TValue>(s_x) }, y{ static_cast<TValue>(s_y) };
	for (auto _ : state)
    {
		benchmark::DoNotOptimize(x / y);
    }
}

BENCHMARK_TEMPLATE(arithmetic_addition, float);
BENCHMARK_TEMPLATE(arithmetic_subtraction, float);
BENCHMARK_TEMPLATE(arithmetic_multiplication, float);
BENCHMARK_TEMPLATE(arithmetic_division, float);

BENCHMARK_TEMPLATE(arithmetic_addition, double);
BENCHMARK_TEMPLATE(arithmetic_subtraction, double);
BENCHMARK_TEMPLATE(arithmetic_multiplication, double);
BENCHMARK_TEMPLATE(arithmetic_division, double);

BENCHMARK_TEMPLATE(arithmetic_addition, fpm::fixed_24_8);
BENCHMARK_TEMPLATE(arithmetic_subtraction, fpm::fixed_24_8);
BENCHMARK_TEMPLATE(arithmetic_multiplication, fpm::fixed_24_8);
BENCHMARK_TEMPLATE(arithmetic_division, fpm::fixed_24_8);

BENCHMARK_TEMPLATE(arithmetic_addition, fpm::fixed_16_16);
BENCHMARK_TEMPLATE(arithmetic_subtraction, fpm::fixed_16_16);
BENCHMARK_TEMPLATE(arithmetic_multiplication, fpm::fixed_16_16);
BENCHMARK_TEMPLATE(arithmetic_division, fpm::fixed_16_16);

BENCHMARK_TEMPLATE(arithmetic_addition, fpm::fixed_8_24);
BENCHMARK_TEMPLATE(arithmetic_subtraction, fpm::fixed_8_24);
BENCHMARK_TEMPLATE(arithmetic_multiplication, fpm::fixed_8_24);
BENCHMARK_TEMPLATE(arithmetic_division, fpm::fixed_8_24);
