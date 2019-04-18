#include <fpm/fixed.h>
#include <fixmath.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>

static constexpr double PI = 3.1415926535897932384626433832795;

using fpm::fixed_8_24;
using fpm::fixed_16_16;
using fpm::fixed_24_8;
using fixed_20_12 = fpm::fixed<std::int32_t, std::int64_t, 12>;

using std::sin;
using std::cos;
using std::tan;
using std::asin;
using std::acos;
using std::atan;
using std::atan2;
using std::sqrt;

static Fix16 sin(Fix16 x) { return x.sin(); }
static Fix16 cos(Fix16 x) { return x.cos(); }
static Fix16 tan(Fix16 x) { return x.tan(); }
static Fix16 asin(Fix16 x) { return x.asin(); }
static Fix16 acos(Fix16 x) { return x.acos(); }
static Fix16 atan(Fix16 x) { return x.atan(); }
static Fix16 atan2(Fix16 x, Fix16 y) { return x.atan2(y); }
static Fix16 sqrt(Fix16 x) { return x.sqrt(); }

class csv_output
{
public:
    explicit csv_output(const std::string& filename)
        : m_stream(filename)
    {
        m_stream.setf(std::ios::fixed);
        m_stream << "x,real,Q24.8,Q20.12,Q16.16,Q8.24,fix16\n";
    }

    void write_row(double x, double y_real, double y_q24_8, double y_q20_12, double y_q16_16, double y_q8_24, double y_fix16)
    {
        m_stream << x << "," << y_real << "," << y_q24_8 << "," << y_q20_12 << "," << y_q16_16 << "," << y_q8_24 << "," << y_fix16 << "\n";
    }

private:
    std::ofstream m_stream;
};

template <typename Callable, typename... Args>
static void check(csv_output& output, double value, Callable&& callable, Args&& ...args)
{
    output.write_row(value,
        callable(std::forward<Args>(args)...),
        static_cast<double>(callable(fixed_24_8(std::forward<Args>(args))...)),
        static_cast<double>(callable(fixed_20_12(std::forward<Args>(args))...)),
        static_cast<double>(callable(fixed_16_16(std::forward<Args>(args))...)),
        static_cast<double>(callable(fixed_8_24(std::forward<Args>(args))...)),
        static_cast<double>(callable(Fix16(std::forward<Args>(args))...)));
}

int main()
{
    csv_output out_sin("sin.csv");
    csv_output out_cos("cos.csv");
    csv_output out_tan("tan.csv");
    csv_output out_asin("asin.csv");
    csv_output out_acos("acos.csv");
    csv_output out_atan("atan.csv");
    csv_output out_atan2("atan2.csv");
    csv_output out_sqrt("sqrt.csv");

    for (int angle = -179; angle <= 180; ++angle)
    {
        const double val = angle * PI / 180.0;

        check(out_sin, val, [](auto x) { return sin(x); }, val);
        check(out_cos, val, [](auto x) { return cos(x); }, val);
        if ((angle + 90) % 180 != 0)
        {
            check(out_tan, val, [](auto x) { return tan(x); }, val);
        }

        const auto y = std::sin(val);
        const auto x = std::cos(val);
        check(out_atan2, val, [](auto y, auto x) { return atan2(y, x); }, y, x);
    }

    for (int value = -100; value <= 100; ++value)
    {
        const double val = value / 100.0;
        check(out_asin, val, [](auto x) { return asin(x); }, val);
        check(out_acos, val, [](auto x) { return acos(x); }, val);
    }

    for (int value = -5000; value <= 5000; value += 5)
    {
        const double val = value / 1000.0;
        check(out_atan, val, [](auto x) { return atan(x); }, val);
    }

    for (int i = 0; i < 1000; ++i)
    {
        const auto val = i / 10.0;
        check(out_sqrt, val, [](auto x) { return sqrt(x); }, val);
    }
}
