#include <fpm/fixed.h>
#include <iomanip>
#include <ostream>

namespace fpm
{
template <typename B, unsigned int F>
void PrintTo(const fpm::fixed<B, F>& val, ::std::ostream* os)
{
    auto f = os->flags();
    *os << static_cast<double>(val)
        << " (0x" << std::hex << std::setw(sizeof(B) * 2) << std::setfill('0') << val.raw_value() << ")";
    os->flags(f);
}
}
