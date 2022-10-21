#include <fpm/fixed.hpp>
#include <fpm/ios.hpp>
#include <fpm/math.hpp>

#include <iostream>

int main()
{
    std::cout << cos(fpm::fixed_16_16(2)) << std::endl;
    return 0;
}
